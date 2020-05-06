#include "WifeBot.hpp"
#include "Core/CommandQueue.hpp"
#include "Game/DataTables.hpp"
#include "Game/World.hpp"

namespace
{
    const std::vector<std::vector<float>> MoveCostTable = inititializeUnitMovementCostTable;
}

Zone::Zone( Zone* parent, float x, float y, float width, float height, int depth )
     : parent( parent )
     , rect( x, y, width, height )
     , depth( depth )
{
    NW = nullptr;
    NE = nullptr;
    SW = nullptr;
    SE = nullptr;
    this->hasKids = false;
}

float Zone::offensiveInfluence( void )
{
    float influence = 0.0f;

    for( auto unit : localObjs )
    {
        if( unit->mCategory & Category::BlueUnit )
            influence += unit->mAttack;
        //else if( unit->mCategory & Category::Red )
        //    influence -= unit->mAttack;
    }
    return influence;
}

float Zone::defensiveInfluence( void )
{
    float influence = 0.0f;
    for( auto unit : localObjs )
    {
        if( unit->mCategory & Category::BlueUnit )
            influence += unit->mArmour;
        //else if( unit->mCategory & Category::Red )
        //    influence -= unit->mDefense;
    }
    return influence;
}

float Zone::tacticalInfluence( void )
{
    float influence = 0.0f;
    //for( auto unit : localObjs )
    {
        //if( unit->mCategory & Category::Blue )
        //    influence += unit->mPerception + unit->mDexterity;
        //else if( unit->mCategory & Category::Red )
        //    influence -= unit->mPerception + unit->mDexterity;
    }
    return influence;
}

float Zone::importance( void )
{
    float importance = 0.0f;
    for( auto square : innerGrid )
    {
        importance += square.importance;
    }
    return importance;
}

Category::Type Zone::ownedBy( void )
{
    if( this->heuristic( ) < 0 )
        return Category::Red;
    return Category::Blue;
}

/// \brief addObj
/// Adds a obj to this node, or it's children if any and if they fit inside there boundries
///  \param obj
/// \return
bool Zone::addObj( Unit* obj )
{
    // object should be added to this zone or one of it's children if any.
    if( this->rect.contains( obj->getWorldPosition().x, obj->getWorldPosition().y + obj->getBoundingRect().height ) )// &&
    {
        //std::cout << "Unit " << obj->mID << " pos: " << obj->getWorldPosition().x << ", " << obj->getWorldPosition().y + obj->getBoundingRect().height << std::endl;
        if( this->depth <= this->mMaxDepth )
        {
            // if node has kids try to push obj to them else add to this node
            if( this->hasKids )
            {
                if( !NW->addObj( obj ) && !NE->addObj( obj ) && !SW->addObj( obj ) && !SE->addObj( obj ) )
                    this->localObjs.push_back( obj ); // obj fits in node but not in kids so leave here
            }
            else
            {   //  no kids add to node and attempt to split node if needed
                this->localObjs.push_back( obj );
                if( this->heuristic() > this->mMaxHeuristic ) this->split( );
                //if( this->localObjs.size() > this->mMaxHeuristic ) this->split( );
            }
        }
        else
            this->localObjs.push_back( obj );
        return true; // object has been succesfully added to either this node or one of it's kids.
    }
    return false; // object being added did not fit inside this node.
}
void Zone::split( void )
{
    // init kids of this zone
    NW = new Zone( this, rect.left, rect.top, rect.width / 2, rect.height / 2, this->depth + 1 );
    NE = new Zone( this, rect.left + rect.width / 2, rect.top, rect.width / 2, rect.height / 2, this->depth + 1 );
    SW = new Zone( this, rect.left, rect.top + rect.height / 2, rect.width / 2, rect.height / 2, this->depth + 1 );
    SE = new Zone( this, rect.left + rect.width / 2, rect.top + rect.height / 2, rect.width / 2, rect.height / 2, this->depth + 1 );
    this->hasKids = true;

    // attempt to move objects from this zone to the new kids
    std::vector<Unit*> tempList = std::vector<Unit*>();
    while( localObjs.size() )
    {
        if( !NW->addObj( localObjs.back() ) && !NE->addObj( localObjs.back() ) && !SW->addObj( localObjs.back() ) && !SE->addObj( localObjs.back() ) )
            tempList.push_back( localObjs.back() );
        localObjs.pop_back( );
    }
    if( tempList.size( ) ) this->localObjs = tempList;
}

void Zone::clear( void )
{
    if( hasKids ) // call clear on all children
    {
        NW->clear( ); NE->clear( ); SW->clear( ); SE->clear( );
        NW = nullptr; NE = nullptr; SW = nullptr; SE = nullptr;
    }
    // clear objs
    while( !localObjs.empty() ) localObjs.pop_back( );
    innerGrid.clear();
    // clear self
    parent = nullptr;
    hasKids = false;
}

float Zone::heuristic( void )
{
    return offensiveInfluence() + defensiveInfluence() + tacticalInfluence();
}

WifeBot::WifeBot( Grid* grid, std::map<unsigned int, Unit*>* units,
                 std::vector<Building*>* buildings, Category::Type* currentTurn )
    : mCategory( Category::WifeBot )
    , mGrid( grid )
    , mUnits( units )
    , mBuildings( buildings )
    , mTargetZone( 0 )
    , mRecalculate( true )
    , mAnimationTimer( sf::Time::Zero )
    , mCurrentTurn( currentTurn )
    , mAllUnitsMoved( false )
{
    for( auto unit : (*mUnits) )
        if( unit.second->mCategory & Category::RedUnit )
            mUnitPathFinders.insert( std::pair<unsigned int, PathFinder<Square>*> ( unit.second->mID, nullptr ) );
    mRootZone = new Zone( nullptr, 0, 0, mGrid->mGridWidth * TILE_SIZE, mGrid->mGridHeight * TILE_SIZE );
}

WifeBot::~WifeBot( void )
{
    mGrid = nullptr;
    mUnits = nullptr;
    mBuildings = nullptr;
}

void WifeBot::updateCurrent( sf::Time dt, CommandQueue& commands )
{
    if( *mCurrentTurn & Category::Red )
    {
        // update logic if needed
        if( mRecalculate )
        {
            mRecalculate = false;
            // build quadtree to build and find the most important zones on the map
            std::vector<Unit*> objRects;
            for( unsigned int i = 0; i < mGrid->mCurrentUnits.size(); ++i )
                objRects.push_back( mGrid->mCurrentUnits[i] );

            // Clear logic for next turn
            mRootZone->clear( );
            mZones.clear();

            // build the tree structure with this frame's obj list
            while( objRects.size( ) )
            {
                // if there are objs in the list then they need to be added to tree
                assert( mRootZone->addObj( objRects.back() ) );
                objRects.pop_back( );
            }

            // Build mZones from the quad tree structure
            initializeZones( mRootZone );

            // build best path from owned spawn points to all other spawn points
                // Consider Offensive and defensive influence of the known world when building best path
                // Boost the Importance of all the squares on the paths

            // Get the best zone(s) to focus on
            findTargetZone( );
            // std::cout << "WifeBot::update(): mTargetZone = " << mTargetZone << std::endl;

            // update each unit controlled by wifeBot based on high level results
            for( auto unit : (*mUnits) )
            {
                if( unit.second && (unit.second->mCategory & Category::RedUnit) ) // wifeBot is always red
                {
                    // only find target square for the unit if it isn't already in the best possible zone else run local zone logic on unit
                    // Check for collision between unit and target zone
                    bool flag = true;
                    for( auto obj : mZones[mTargetZone]->localObjs ) if( obj->mID == unit.second->mID ) flag = false;
                    if( flag )
                    {
                        // Change to the most important square in the grid that isnt already occupied by another unit
                        bool squareExists = false;
                        unsigned int targetSquare = 0;
                        std::vector<unsigned int> excludedZones;
                        while( !squareExists && excludedZones.size() < mZones.size() )
                        {
                            targetSquare = 0;
                            for( unsigned int i = 0; i < mZones[mTargetZone]->innerGrid.size(); ++i )
                            {
                                // if can't see through fog of war(might be available) or if can see the square through fog of war and it isn't occupied by anything
                                if( !mZones[mTargetZone]->innerGrid.at( i ).isVisible || (!mZones[mTargetZone]->innerGrid.at(i).isOccupied && mZones[mTargetZone]->innerGrid.at( i ).buildingID == -1) )
                                {
                                    // if this invisible and/or empty square is more important than the current target square.
                                    if( mZones[mTargetZone]->innerGrid.at( i ).importance > mZones[mTargetZone]->innerGrid.at( targetSquare ).importance ) targetSquare = i;
                                    // even if this isn't the most important square in the grid a square was still found.
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
                        sf::Vector2i targetIndex = mZones[mTargetZone]->innerGrid.at( targetSquare ).gridIndex;
                        mUnitPathFinders.at( unit.second->mID ) = new PathFinder<Square>( mGrid->mData, sf::Vector2i( mGrid->mGridWidth, mGrid->mGridHeight ), &mGrid->mData[unit.second->mGridIndex.y * mGrid->mGridWidth + unit.second->mGridIndex.x],
                                                                                                        &mGrid->mData.at( targetIndex.y * mGrid->mGridWidth + targetIndex.x ),
                            [=]( Square* start, Square* goal )->float {
                                float distance = std::abs( start->gridIndex.x - goal->gridIndex.x )+std::abs( start->gridIndex.y - goal->gridIndex.y );
                                if( distance > 1 )
                                {
                                    distance *= .5;
                                    distance += mGrid->getMoveCost( unit.second->mUnitType, start->terrainType );
                                }
                                //distance += mZones.at( mTargetZone )->heuristic();
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
                                std::vector<sf::Vector2i> posMoves = mGrid->getPossiblePositions( unit.second->mGridIndex, unit.second->mUnitType, unit.second->mSpeed );
                                auto path = mUnitPathFinders.at( unit.second->mID );
                                sf::Vector2i bestMove;

                                // look ahead in pathfinder to units dex distance or last element in path and find match in pos Moves
                                if( path->size() > unit.second->mSpeed )
                                    for( unsigned int i = 0; i < unit.second->mSpeed-1; ++i ) path->getNextMove( );
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

void WifeBot::initializeZones( Zone* rootZone )
{
    if( rootZone->hasKids )
    {
        initializeZones( rootZone->NW );
        initializeZones( rootZone->NE );
        initializeZones( rootZone->SW );
        initializeZones( rootZone->SE );
    }
    else
    {
        rootZone->id = mZones.size();
        //std::cout << "rootZone " << rootZone->id << " pos: " << rootZone->rect.left << ", " << rootZone->rect.top << " Size: " << rootZone->rect.width << ", " << rootZone->rect.height <<  std::endl;

        // Build list of squares that should be in this zone based on the zone's rect.
        for( unsigned int i = 0; i < mGrid->mData.size(); ++i )
        {
            //std::cout << "Tile Pos: " << mGrid->mData[i].rect->getSprite()->getPosition().x << ",  " << mGrid->mData[i].rect->getSprite()->getPosition().y << std::endl;
            if( rootZone->rect.contains( mGrid->mData[i].rect->getSprite()->getPosition( ) ) ) // &&
                   // mZones.back( )->rect.contains( mGrid->mData[i].rect->getWorldPosition().x + mGrid->mData[i].rect->getBoundingRect().width, mGrid->mData[i].rect->getWorldPosition().y + mGrid->mData[i].rect->getBoundingRect().height ) )
            {
                rootZone->innerGrid.push_back( mGrid->mData[i] );
                rootZone->innerGrid.back().debugText->setString( std::to_string( rootZone->id ) );
            }
        }
        mZones.push_back( rootZone );
    }
}

void WifeBot::findTargetZone( std::vector<unsigned int> excludedZones )
{
    // find the zone with the most importance
    for( unsigned int i = 0; i < mZones.size(); ++i )
    {
        // remove excludedZones from the test.
        bool flag = true;
        for( unsigned int j = 0; j < excludedZones.size(); ++j )
            if( i == excludedZones[j] )
            {
                flag = false;
                break;
            }

        if( flag )
        {
            unsigned int currentImportance = mZones[i]->importance( );
            unsigned int targetZoneImportance = mZones[mTargetZone]->importance( );

            // if( currentImportance > 0 ) std::cout << "Important zone Found! " << i << std::endl;
            if( currentImportance >= targetZoneImportance ) mTargetZone = i;
//            else if( currentImportance == targetZoneImportance )
//            {
//                if( mZones[i]->offensiveInfluence() > mZones[mTargetZone]->offensiveInfluence() )
//                    mTargetZone = i;
//                else if( mZones[i]->defensiveInfluence() > mZones[mTargetZone]->defensiveInfluence() ) mTargetZone = i;
//            }
        }
    }
}

