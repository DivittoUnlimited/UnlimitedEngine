#include "WifeBot.hpp"
#include "Game/World.hpp"
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

WifeBot::WifeBot( World* world, std::vector<Unit *> *units )
    : mCategory( Category::WifeBot )
    , mWorld( world )
    , mUnits( units )
    , mRecalculate( false )
    , mTargetZone( 0 )
    , mAnimationTimer( sf::Time::Zero )
    , mAllUnitsMoved( false )
    , mUnit( nullptr )
{
    for( auto unit : (*mUnits) )
        if( unit->mCategory & Category::RedUnit )
            mUnitPathFinders.insert( std::pair<std::string, PathFinder<Square>*> ( unit->mID, nullptr ) );
    mRootZone = new Zone( nullptr, 0, 0, mWorld->mGridWidth * TILE_SIZE, mWorld->mGridHeight * TILE_SIZE );
}

WifeBot::~WifeBot( void )
{
}

void WifeBot::update( sf::Time )
{
    if( mUnit == nullptr || mUnit->mID != mWorld->mSelectedUnit )
    {
        for( auto unit : (*mUnits) )
        {
            if( unit && unit->mID == mWorld->mSelectedUnit && (unit->mCategory & Category::RedUnit) ) // wifeBot is always red
            {
                mUnit = unit;
                mRecalculate = true;
                break;
            }
        }
    }

    // update logic if needed
    if( mRecalculate )
    {

        //std::cout << "WifeBot is calculating!" << std::endl;

        // build quadtree to build and find the most important zones on the map
        std::vector<Unit*> objRects;
        for( unsigned int i = 0; i < mWorld->mCurrentUnits.size( ); ++i )
            objRects.push_back( mWorld->mCurrentUnits[i] );

        // Clear logic for next turn
        mRootZone->clear( );
        mZones.clear( );

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


        // only find target square for the mUnit if it isn't already in the best possible zone else run local zone logic on mUnit
        // Check for collision between mUnit and target zone
        bool flag = true;
        for( auto obj : mZones[mTargetZone]->localObjs ) if( obj->mID == mUnit->mID ) flag = false;
        if( flag )
        {
            // Change to the most important square in the grid that isnt already occupied by another mUnit
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
            mUnitPathFinders.at( mUnit->mID ) = new PathFinder<Square>( mWorld->mGrid, sf::Vector2i( mWorld->mGridWidth, mWorld->mGridHeight ), &mWorld->mGrid[mUnit->mGridIndex.y * mWorld->mGridWidth + mUnit->mGridIndex.x],
                                                                  &mWorld->mGrid.at( targetIndex.y * mWorld->mGridWidth + targetIndex.x ),
             [=]( Square* start, Square* goal )->float {
                 float distance = std::abs( start->gridIndex.x - goal->gridIndex.x )+std::abs( start->gridIndex.y - goal->gridIndex.y );
                 if( distance > 1 )
                 {
                      distance *= .5;
                      distance += mWorld->getMoveCost( mUnit->mUnitType, start->terrainType );
                 }
                 //distance += mZones.at( mTargetZone )->heuristic();
                 return distance;
             } );
        }
        else // mUnit is at best possible area run local logic to decide what to do (move towards target in grid, attack target thats in range)
        {
           std::cout << "mUnit " << mUnit->mID << " has already found the best zone and requires further instruction from the WifeBot!" << std::endl;
        }

        if( mUnitPathFinders.at( mUnit->mID ) && mUnitPathFinders.at( mUnit->mID )->size() )
        {
            std::vector<sf::Vector2i> posMoves = mWorld->getPossiblePositions( mUnit->mGridIndex, mUnit->mUnitType, mUnit->mSpeed );
            auto path = mUnitPathFinders.at( mUnit->mID );
            sf::Vector2i bestMove;

            // look ahead in pathfinder to mUnits dex distance or last element in path and find match in pos Moves
            if( path->size() > mUnit->mSpeed ) for( unsigned int i = 0; i < mUnit->mSpeed-1; ++i ) path->getNextMove( );

            // path less then the distance the mUnit can move look at last element in path list.
            else while( path->mPath.size() > 1 ) path->getNextMove();

            // traverse posMoves and find the closest square to the farthest point in the pathfinder
            sf::Vector2i farthestPos = path->getNextMove( );
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
            mWorld->moveUnit( mUnit->mGridIndex, bestMove );
            mUnit->mIsSelectedUnit = false;
            mUnit->mHasMoved = true;
            mAnimationTimer = sf::seconds(4);

            /// THIS IS FOR DEMO ONLY
            ///  ERASE ONCE WIFE BOT CAN ATTACK!!!!!!!!!!!!!!!!
            ///
            mUnit->mHasSpentAction = true;
        }
        // if mUnit is done moving and attacking then end turn!
        if( mUnit->mHasMoved && mUnit->mHasSpentAction )
        {
            mWorld->endTurn( );
            mRecalculate = false;
        }
        // Spawn units based on high level goals
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
        for( unsigned int i = 0; i < mWorld->mGrid.size(); ++i )
        {
            //std::cout << "Tile Pos: " << mGrid->mData[i].rect->getSprite()->getPosition().x << ",  " << mGrid->mData[i].rect->getSprite()->getPosition().y << std::endl;
            if( rootZone->rect.contains( mWorld->mGrid[i].rect->getSprite()->getPosition( ) ) ) // &&
                   // mZones.back( )->rect.contains( mGrid->mData[i].rect->getWorldPosition().x + mGrid->mData[i].rect->getBoundingRect().width, mGrid->mData[i].rect->getWorldPosition().y + mGrid->mData[i].rect->getBoundingRect().height ) )
            {
                rootZone->innerGrid.push_back( mWorld->mGrid[i] );
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
        if( flag ) if( mZones[i]->importance( ) >= mZones[mTargetZone]->importance( ) ) mTargetZone = i;
    }
}
