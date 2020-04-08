#include "WifeBot.hpp"
#include "Core/CommandQueue.hpp"
#include "Game/DataTables.hpp"
#include "Game/World.hpp"

namespace
{
    const std::vector<std::vector<float>> MoveCostTable = inititializeUnitMovementCostTable;
}

Zone::Zone( void )
{

}

float Zone::offensiveInfluence( void )
{
    float influence = 0.0f;
    for( auto square : innerGrid ) influence += square->offensiveInfluence;
    return influence;
}

float Zone::defensiveInfluence( void )
{
    float influence = 0.0f;
    for( auto square : innerGrid ) influence += square->defensiveInfluence;
    return influence;
}

float Zone::tacticalInfluence( void )
{
    float influence = 0.0f;
    for( auto square : innerGrid ) influence += square->tacticalInfluence;
    return influence;
}

float Zone::importance( void )
{
    float importance = 0.0f;
    for( auto square : innerGrid ) importance += square->importance;
    return importance;
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
    , mTargetZone( 0 )
{
    for( auto unit : (*mUnits) )
        if( unit.second->mCategory & Category::RedUnit )
            mUnitPathFinders.insert( std::pair<unsigned int, PathFinder<Square>*> ( unit.second->mID, nullptr ) );
    initializeZones();
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
            // update high level logic
            findTargetZone( );

            // update each unit controlled by wifeBot based on high level results
            for( auto unit : (*mUnits) )
            {
                if( unit.second && (unit.second->mCategory & Category::RedUnit) ) // wifeBot is always red
                {
                    // only find target square for the unit if it isn't already in the best possible zone else run local zone logic on unit
                    bool shouldMove = true;
                    for( auto squares : mZones.at( mTargetZone ).innerGrid ) if( unit.second->mGridIndex == squares->gridIndex ) shouldMove = false;

                    if( shouldMove )
                    {
                        // Change to the most important square in the grid that isnt already occupied by another unit
                        bool squareExists = false;
                        unsigned int targetSquare = 0;
                        std::vector<unsigned int> excludedZones;
                        while( !squareExists && excludedZones.size() < mZones.size() )
                        {
                            std::vector<Square*>* targetSquares = &mZones[mTargetZone].innerGrid;
                            for( unsigned int i = 0; i < targetSquares->size(); ++i )
                            {
                                // if can see the square through fog of war and it isn't occupied by anything
                                if( !targetSquares->at( i )->isVisible || (!targetSquares->at( i )->isOccupied && targetSquares->at( i )->buildingID == -1) )
                                {
                                    // if this empty square is more important than the current target square.
                                    if( targetSquares->at( i )->importance > targetSquares->at( targetSquare )->importance ) targetSquare = i;
                                    squareExists = true;
                                }
                            }
                            // if no square exists(Zone is full) rerun zone Importance algorithm above excluding the current target zone
                            if( !squareExists )
                            {
                                excludedZones.push_back( mTargetZone );
                                findTargetZone( excludedZones );
                            }
                        }
                        // AI is broken if no square can be found( or map is completly occupied )
                        assert( squareExists );

                        // Build best path to the target square
                        sf::Vector2i targetIndex = mZones[mTargetZone].innerGrid[targetSquare]->gridIndex;
                        mUnitPathFinders.at( unit.second->mID ) = new PathFinder<Square>( mGrid->mData, sf::Vector2i( mGrid->mGridWidth, mGrid->mGridHeight ), &mGrid->mData[unit.second->mGridIndex.y * mGrid->mGridWidth + unit.second->mGridIndex.x],
                                                                                                        &mGrid->mData.at( targetIndex.y * mGrid->mGridWidth + targetIndex.x ),
                            [=]( Square* start, Square* goal )->float {
                                float distance = std::abs( start->gridIndex.x - goal->gridIndex.x )+std::abs( start->gridIndex.y - goal->gridIndex.y );
                                if( distance > 1 )
                                {
                                    distance *= .5;
                                    distance += mGrid->getMoveCost( unit.second->mUnitType, start->terrainType );
                                }
                                return distance;
                             } );
                    }
                    else // unit is at best possible area run local logic to decide what to do (move towards target in grid, attack target thats in range)
                    {

                    }
                }
            }
            // Spawn units based on high level goals
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
                            mAnimationTimer = sf::seconds(4);

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

        // End Turn if all actions have been completed.
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

void WifeBot::initializeZones( void )
{
    unsigned int widthCounter = 0;
    unsigned int heightCounter = 0;
    for( unsigned int i = 0; i < ((mGrid->mGridWidth / mZoneWidth) * (mGrid->mGridHeight / mZoneHeight)); ++i )
    {
        mZones.push_back( Zone( ) );
        // loop through grid to insert squares into zones
        for( unsigned int y = heightCounter * mZoneHeight; y < heightCounter * mZoneHeight + mZoneHeight; ++y )
            for( unsigned int x = widthCounter * mZoneWidth; x < widthCounter * mZoneWidth + mZoneWidth; ++x )
                mZones.back().innerGrid.push_back( &mGrid->mData.at( y * mGrid->mGridWidth + x ) );
        if( ++widthCounter >= mGrid->mGridWidth / mZoneWidth )
        {
            ++heightCounter;
            widthCounter = 0;
        }
    }
    // Debugging
    for( unsigned int i = 0; i < mZones.size( ); ++i ) std::cout << "Zone " << i << ": Importance: " << mZones[i].importance() << std::endl;
}

void WifeBot::findTargetZone( std::vector<unsigned int> excludedZones )
{
    // find the zone with the most importance
    for( unsigned int i = 0; i < mZones.size(); ++i )
    {
        // remove excludedZones from the test.
        bool flag = true;
        for( unsigned int j = 0; j < excludedZones.size(); ++j )
            if( i == excludedZones[j] ) flag = false;

        if( flag )
        {
            unsigned int currentImportance = mZones[i].importance();
            unsigned int targetZoneImportance = mZones[mTargetZone].importance();

            if( currentImportance > targetZoneImportance ) mTargetZone = i;
            else if( currentImportance == targetZoneImportance )
            {
                if( mZones[i].offensiveInfluence() > mZones[mTargetZone].offensiveInfluence() ) mTargetZone = i;
                else if( mZones[i].defensiveInfluence() > mZones[mTargetZone].defensiveInfluence() ) mTargetZone = i;
            }
        }
    }
}

