#include "WifeBot.hpp"
#include "Core/CommandQueue.hpp"
#include "Game/DataTables.hpp"
#include "Game/World.hpp"

namespace
{
    const std::vector<std::vector<float>> MoveCostTable = inititializeUnitMovementCostTable;
}

WifeBot::WifeBot( Grid* grid, std::map<unsigned int, Unit*>* units,
                 std::vector<Building*>* buildings, Category::Type* currentTurn )
    : mCategory( Category::WifeBot )
    , mGrid( grid )
    , mUnits( units )
    , mBuildings( buildings )
    , mRecalculate( true )
    , mAnimationTimer( sf::Time::Zero )
    , mCurrentTurn( currentTurn )
    , mAllUnitsMoved( false )
{
    for( auto unit : (*mUnits) )
        if( unit.second->mCategory & Category::RedUnit )
        mUnitPathFinders.insert( std::pair<unsigned int, PathFinder<Square>*> ( unit.second->mID, nullptr ) );
}

WifeBot::~WifeBot( void )
{
    mGrid = nullptr;
    mUnits = nullptr;
    mBuildings = nullptr;
    mCurrentTurn = nullptr;
}

void WifeBot::updateCurrent( sf::Time dt, CommandQueue& commands )
{
    if( *mCurrentTurn & Category::Red )
    {
        // update logic if needed
        if( mRecalculate )
        {
            mRecalculate = false;
            // update high level logic (pathfinding on zones)

            // update each unit controlled by wifeBot based on high level results

            for( auto unit : (*mUnits) )
            {
                if( unit.second && (unit.second->mCategory & Category::RedUnit) ) // wifeBot is always red
                {
                    mUnitPathFinders.at( unit.second->mID ) = new PathFinder<Square>( mGrid->mData, &mGrid->mData[unit.second->mGridIndex.y * (WINDOW_WIDTH / TILE_SIZE) + unit.second->mGridIndex.x],
                                                                                                    &mGrid->mData[4 * ( WINDOW_WIDTH / TILE_SIZE) + 3],
                        [=]( Square* start, Square* goal )->float {
                            mGrid->mData[start->gridIndex.y * (WINDOW_WIDTH / TILE_SIZE) + start->gridIndex.x].influence = .5;

                            float distance = std::abs( start->gridIndex.x - goal->gridIndex.x )+std::abs( start->gridIndex.y - goal->gridIndex.y );
                            if( distance > 1 )
                            {
                                distance *= .5;
                                distance += mGrid->getMoveCost( unit.second->mUnitType, start->terrainType );
                            }
                            return distance;
                         } );
                    for( auto a : mUnitPathFinders.at( unit.second->mID )->mPath )
                    {
                        //mGrid->mData[a.y * (WINDOW_WIDTH / TILE_SIZE) + a.x].debugText->setString( std::to_string( mGrid->mData[a.y * (WINDOW_WIDTH / TILE_SIZE) + a.x].costSoFar + mGrid->mData[a.y * (WINDOW_WIDTH / TILE_SIZE) + a.x].heuristicCost ) );
                        mGrid->mData[a.y * (WINDOW_WIDTH / TILE_SIZE) + a.x].influence = -.5;
                    }
                }
            }
            // update controlled buildings based on high level goals
        }
        if( mAnimationTimer <= sf::Time::Zero )
        {
            if( !mAllUnitsMoved )
            {
                mAllUnitsMoved = true;
                for( auto unit : (*mUnits) )
                {
                    if( unit.second && (unit.second->mCategory & Category::RedUnit) && !unit.second->mHasMoved )
                    {
                        if( unit.second->mIsSelectedUnit )
                        {
                            if( mUnitPathFinders.at( unit.second->mID ) && mUnitPathFinders.at( unit.second->mID )->size() )
                            {
                                std::vector<sf::Vector2i> posMoves = mGrid->getPossiblePositions( unit.second->mGridIndex, unit.second->mUnitType, unit.second->mDexterity );
                                auto path = mUnitPathFinders.at( unit.second->mID );
                                sf::Vector2i bestMove;

                                // look ahead in pathfinder to units dex distance or last element in path and find match in pos Moves
                                if( path->size() > unit.second->mDexterity )
                                    for( unsigned int i = 0; i < unit.second->mDexterity-1; ++i ) path->getNextMove( );
                                // path less then the distance the unit can move look at last element in path list.
                                else while( path->mPath.size() > 1 ) path->getNextMove();
                                sf::Vector2i farthestPos = path->getNextMove( );

                                // traverse posMoves and find the closest square to the farthest point in the pathfinder
                                bool flag = false;
                                for( auto move : posMoves )
                                    if( move == farthestPos )
                                    {
                                        bestMove = move;
                                        flag = true;
                                        break;
                                    }
                                if( !flag ) // optimal solution doesn't exist find closest option
                                {
                                    float distance = -1.0f;
                                    for( auto move : posMoves )
                                    {
                                        float temp = std::abs( move.x - farthestPos.x ) + std::abs( move.y - farthestPos.y );
                                        if( distance == -1.0f || temp < distance )
                                        {
                                            distance = temp;
                                            bestMove = move;
                                        }
                                    }
                                }

                                // create command to click on the destination square.
                                Command command;
                                command.category = Category::Grid;
                                command.action = derivedAction<Grid>( [unit, bestMove] ( Grid& g, sf::Time ){
                                        g.moveUnit( unit.second->mGridIndex, bestMove ); } );
                                commands.push( command );
                            }
                            unit.second->mIsSelectedUnit = false;
                            unit.second->mHasMoved = true;
                        }
                        else // select the next unit to update
                        {
                            if( mUnitPathFinders.at( unit.second->mID ) && mUnitPathFinders.at( unit.second->mID )->size() > 0 )
                            {
                                // create command to click on unit
                                Command command;
                                command.category = Category::Grid;
                                command.action = derivedAction<Grid>( [unit] ( Grid& g, sf::Time ){
                                        g.selectUnit( unit.second->mGridIndex.x, unit.second->mGridIndex.y ); } );
                                commands.push( command );

                                mAnimationTimer = sf::seconds(2);
                            }
                            else
                            {
                                unit.second->mHasMoved = true;
                            }
                        }
                        mAllUnitsMoved = false;
                        break; // only handle one unit per frame
                    }
                }
            }
        }
        else
            mAnimationTimer -= dt;
        // Activate buildings based on current plan

        if( mAllUnitsMoved && mAnimationTimer <= sf::Time::Zero )
        {
            mAllUnitsMoved = false;
            // end turn
            Command command;
            command.category = Category::Grid;
            command.action = derivedAction<Grid>( [] ( Grid& g, sf::Time ){
                g.mWorld->changeTurn( );
            } );
            commands.push( command );
        }

    }
}

void WifeBot::handleEvent( const sf::Event& )
{

}


