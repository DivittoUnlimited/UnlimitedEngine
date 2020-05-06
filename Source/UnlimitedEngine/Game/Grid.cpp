#include "Grid.hpp"
#include "Game/DataTables.hpp"
#include <SFML/Window/Event.hpp>
#include "Core/Globals.hpp"
#include "Game/Unit.hpp"
#include "Game/World.hpp"
#include "Core/PathFinder.hpp"

namespace
{
    const std::vector<std::vector<float>> MoveCostTable = inititializeUnitMovementCostTable;
}

Grid::Grid( World* world, unsigned int gridWidth, unsigned int gridHeight )
    : mWorld( world )
    , mGridWidth( gridWidth )
    , mGridHeight( gridHeight )
    , mUpdateFogOfWar( true )
    , mEndTurn( true )
    , mUpdateInfluenceMap( true )
    , mUpdateThreatLevelMap( false )
    , mShowInfluenceMap( true )
    , mShowThreatLevelMap( false )
{

}

Grid::~Grid( void )
{
}

unsigned int Grid::getCategory( ) const
{
    return Category::Grid;
}
bool Grid::isMarkedForRemoval( ) const
{
   return false;
}
bool Grid::isDestroyed( ) const
{
    return false;
}
void Grid::updateCurrent( sf::Time dt, CommandQueue& commands )
{
    if( mUpdateFogOfWar )
    {
        updateFogOfWar( );
        mUpdateFogOfWar = false;
    }
    if( mEndTurn ) this->endTurn();
    else
        mWorld->mStateStack->pushState( States::ActionMenuState );

    // The onscreen debugging view of influence set to show offensive influence by default
    if( mUpdateInfluenceMap ) updateInfluenceMap( );

    // Handle relevant commands
    while( !commands.isEmpty() )
    {
        Command com = commands.pop();
        if( com.category & getCategory() )
        {
            com.action( *this, dt );
        }
    }
}
void Grid::drawCurrent( sf::RenderTarget&, sf::RenderStates ) const
{
    // do nothing
}

void Grid::handleLeftClick( sf::Vector2i pos )
{
    bool flag = false;
    // Find Which Square on the grid the user clicked
    for( unsigned int j = 0; j < mGridHeight; ++j )
    {
        for( unsigned int i = 0; i < mGridWidth; ++i )
        {
            Square* square = &mData[j * mGridWidth + i];
            if( square->mBounds.contains( pos.x, pos.y ) ) // pos refers to the mouse pos when it was clicked.
            {
                flag = true; // break out loop, the square has been found
                if( square->isVisible )
                {
                    /* OLD WAY REMOVE ONCE INITATIVE SYSTEM COMPLETE
                    if( square->isOccupied && !this->mUnitAwaitingOrders )
                    {
                        if( mCurrentUnits.at( square->unitID )->mCategory & mWorld->mCurrentTurn )
                        {   // Friendly unit
                            clearGrid( );
                            this->mSelectedGridIndex = sf::Vector2i( i, j );
                            mWorld->mStateStack->pushState( States::ActionMenuState );
                        }
                        else
                        {   // Enemy unit show move/attack range and stats or something

                        }
                    }
                    else */ if( square->isPossibleNewLocation )
                    {
                        if( mWorld->getSelectedBuilding() > -1 )  // a spawn was selected before so the player wants to drop a new unit this could be done better
                        {
                            mWorld->spawnUnit( static_cast<unsigned int>(mWorld->getSelectedUnit()), sf::Vector2i( static_cast<int>(i), static_cast<int>(j) ) );
                            if( mWorld->mNetworkedWorld )
                                mWorld->mNetworkNode->notifyGameAction( GameActions::Type::SpawnUnit, sf::Vector2f( i, j ) );
                            mCurrentUnits.at( mCurrentUnits.size() - 1 )->mHasMoved = true;
                            clearGrid( );
                        }
                        else moveUnit( mCurrentUnits.at( mWorld->getSelectedUnit() )->mGridIndex, square->gridIndex  );
                    }
                    else if( square->isPossibleAttackPosition  )
                    {
                        // An Ability is being used and this square has been selected as the origin
                        Unit* unit = mCurrentUnits[static_cast<unsigned int>( mWorld->mSelectedUnit )];
                        unit->mAbilities.at( unit->mSelectedAbility ).origin = square->gridIndex;
                        if( unit->mAbilities.at( unit->mSelectedAbility ).hasRotation ) // if attack must be rotated
                            // Get rotation from player call attack from there
                            this->mWorld->mStateStack->pushState( States::RotationSelectMenuState );
                        else
                        {
                            unit->mAbilities.at( unit->mSelectedAbility ).origin = square->gridIndex;
                            for( auto t : unit->mAbilities.at( unit->mSelectedAbility ).AOE.at( "only" ) ) // use abililty on all units inside the ability AOE[0]
                            {
                                // get all units inside AOE from grid
                                int id = this->mData.at( (t.y+square->gridIndex.y) * (this->mGridWidth) + (t.x+square->gridIndex.x) ).unitID;
                                if( id > 0 && id != static_cast<int>( unit->mID ) )
                                {
                                    unit->useAbility( unit->mSelectedAbility, this->mCurrentUnits.at( id ) );
                                }
                            }
                            unit->mSelectedAbility = "NONE";
                            unit->mHasSpentAction = true;
                            unit->mIsSelectedUnit = false;
                            this->clearGrid();
                        }
                    }
                    else if( square->buildingID > -1 && mCurrentBuildings.at( square->buildingID )->mCategory & mWorld->mCurrentTurn )
                    {   // square has a spawn point on it.
                        clearGrid( );
                        mWorld->setSelectedBuilding(square->buildingID );
                        mWorld->mStateStack->pushState( States::SpawnPointMenuState );
                        // where to spawn the unit??
                        std::vector<sf::Vector2i> possibleLocations = getPossiblePositions( mCurrentBuildings.at( static_cast<unsigned int>(mWorld->getSelectedBuilding()) )->mGridIndex, static_cast<unsigned int>(mWorld->getSelectedUnit()), 1 );
                        for( auto loc : possibleLocations )
                        {
                            mData[loc.y * mGridWidth + loc.x].isPossibleNewLocation = true;
                            mData[loc.y * mGridWidth + loc.x].rect->getSprite()->setFillColor( sf::Color::Cyan );
                        }
                    }
                }
            }
        }
        if( flag ) break;
    }
}

bool Grid::buildGrid(  Tiled::TileSet tileSet, Tiled::Layer layer )
{
    unsigned int counter = 0;
    for( unsigned int j = 0; j < mGridHeight; ++j )
    {
        for( unsigned int i = 0; i < mGridWidth; ++i )
        {
            mData.push_back( Square( TerrainTypeMap.at( tileSet.tiles.at( layer.data[counter]-1 ).at( "type" ) ),
                                            sf::Vector2i( i, j ), sf::Vector2f( i * TILE_SIZE, j * TILE_SIZE), TILE_SIZE, false ) );
            mData.back().gridIndex = sf::Vector2i( i, j );
            std::unique_ptr<RectangleShapeNode> rect(
                        new RectangleShapeNode( sf::IntRect( sf::Vector2i( static_cast<int>( i * tileSet.tileWidth ),
                                                                           static_cast<int>( j * tileSet.tileHeight ) ),
                                                             sf::Vector2i( static_cast<int>( tileSet.tileWidth ),
                                                                           static_cast<int>( tileSet.tileHeight ) ) ) ) );
            rect.get()->getSprite()->setFillColor( sf::Color( 0, 0, 0, 0 ) );
            rect.get()->getSprite()->setOutlineThickness( 1 );
            rect.get()->getSprite()->setOutlineColor( sf::Color( 0, 0, 0, 150 ) );

            ++counter;
            std::unique_ptr<TextNode> debugText( new TextNode( mWorld->mFonts, "" ) );
            debugText.get()->setPosition( rect.get()->getSprite()->getPosition().x + 25, rect.get()->getSprite()->getPosition().y + 25 );
            mData.back().debugText = debugText.get();
            mData.back().rect = rect.get();
            this->attachChild( std::move( rect ) );
            this->attachChild( std::move( debugText ) );
        }
    }
    // turn off fog of war if both players are using the same screen.
    if( mWorld->mLocalMultiplayerWorld )
        for( unsigned int i = 0; i < mData.size(); ++i )
            mData[i].isVisible = true;
    return true;
}

std::vector<sf::Vector2i> Grid::getPossiblePositions( sf::Vector2i startingPoint, unsigned int unitType, int distanceLeft )
{
    std::vector<sf::Vector2i> temp;
    buildPossiblePositions( &temp, startingPoint, unitType, distanceLeft );
    return temp;
}

std::vector<sf::Vector2i> Grid::getPossibleAttackPositions( sf::Vector2i startingPoint, int innerRange, int outerRange )
{
    std::vector<sf::Vector2i> innerList;
    buildPossiblePositions( &innerList, startingPoint, -1, innerRange );

    std::vector<sf::Vector2i> outerList;
    buildPossiblePositions( &outerList, startingPoint, -1, outerRange );

    while( innerList.size() )
    {
        for( auto i = outerList.begin(); i < outerList.end(); ++i )
        {
            if( innerList.back() == *i )
            {
                outerList.erase( i );
                break;
            }
        }
        innerList.pop_back();
    }
    return outerList;
}

void Grid::addUnit( Unit* unit )
{
    for( unsigned int i = 0; i < mData.size(); ++i )
    {
        if( mData[i].mBounds.contains( unit->getPosition().x, unit->getPosition().y + TILE_SIZE / 2 ) )
        {
            unit->mGridIndex = mData[i].gridIndex;
            mData[i].isOccupied = true;
            mData[i].unitID = unit->mID;
            mCurrentUnits.insert( std::pair<unsigned int, Unit*>( unit->mID, unit ) );
            // mData[i].debugText->setString( std::to_string( mData[i].unitID ) );
            // mData[i].debugText->setColor( sf::Color::Yellow );
            break;
        }
    }
    updateTurnOrderIndicators( );
}

void Grid::addBuilding( Building* building )
{
    for( unsigned int j = 0; j < mData.size(); ++j )
    {
        if( mData[j].mBounds.contains( building->getPosition().x, building->getPosition().y ) )
        {
            mData[j].buildingID = building->mID;
            mData[j].isVisible = true;
            building->mGridIndex = sf::Vector2i( building->getPosition().x / TILE_SIZE, building->getPosition().y / TILE_SIZE );
            mCurrentBuildings.push_back( building );
            break;
        }
    }
}

void Grid::removeUnit( sf::Vector2i position )
{
    mCurrentUnits.erase( mData[position.y * mGridWidth + position.x].unitID );
    mData[position.y * mGridWidth + position.x].unitID = -1;
    mData[position.y * mGridWidth + position.x].isOccupied = false;
    mUpdateFogOfWar = true;
    updateTurnOrderIndicators( );
}

void Grid::selectUnit( unsigned int i, unsigned int j )
{
    Unit* unit = mCurrentUnits[mData[j * mGridWidth + i].unitID];
    if( unit ) // units can only move once per turn
    {
        std::vector<sf::Vector2i> possibleLocations = getPossiblePositions(
                                                    sf::Vector2i( static_cast<int>( i ), static_cast<int>( j ) ),
                                                    unit->mUnitType, static_cast<unsigned int>( unit->mSpeed ) );
        for( auto loc : possibleLocations )
        {
            mData[loc.y * mGridWidth + loc.x].isPossibleNewLocation = true;
            if( unit->mIsVisible && (( !mWorld->mNetworkedWorld && !mWorld->mLocalMultiplayerWorld && mWorld->mCurrentTurn & Category::Blue ) ||
                    mWorld->mNetworkedWorld || mWorld->mLocalMultiplayerWorld ) )
                mData[loc.y * mGridWidth + loc.x].rect->getSprite()->setFillColor( sf::Color::Cyan );
        }
        mSelectedGridIndex = sf::Vector2i( i, j );
        mWorld->setSelectedUnit( unit->mID );
        unit->mIsSelectedUnit = true;
    }
}

bool Grid::moveUnit( sf::Vector2i currentPos, sf::Vector2i newPos )
{
    Square* newSquare = &mData[newPos.y * mGridWidth + newPos.x];
    Square* oldSquare = &mData[currentPos.y * mGridWidth + currentPos.x];
    if( newSquare->isOccupied ) clearGrid();
    else
    {
        oldSquare->isVisible = false;
        newSquare->isVisible = true;
        newSquare->unitID = oldSquare->unitID;
        newSquare->isOccupied = true;
        Unit* unit = mCurrentUnits[oldSquare->unitID];
        oldSquare->unitID = -1;
        oldSquare->isOccupied = false;
        newSquare->isOccupied = true;
        unit->mHasMoved = true;
        unit->mGridIndex = newPos;
        mSelectedGridIndex = newPos;
        unit->addModifier( StatModifier( "initiative", "10-25", 1 ) );
        // std::cout << "Grid::MoveUnit Unit Path:" << std::endl;
        unit->mPath = new PathFinder<Square>( mData, sf::Vector2i( mGridWidth, mGridHeight ), oldSquare, newSquare,
                [=]( Square* start, Square* goal )->float {
                float distance = std::abs( start->gridIndex.x - goal->gridIndex.x )+std::abs( start->gridIndex.y - goal->gridIndex.y );
                    if( distance > 1 )
                    {
                        distance *= .5;
                        distance += getMoveCost( unit->mUnitType, start->terrainType );
                    }
                    return distance;

            //return (std::abs( start->gridIndex.x - goal->gridIndex.x ) +
            //       std::abs( ( start->gridIndex.y - goal->gridIndex.y )));
                } );

        // remove movement squares
        for( unsigned int i = 0; i < mData.size(); ++i )
        {
            mData[i].isPossibleNewLocation = false;
            mData[i].rect->getSprite()->setFillColor( sf::Color( 0, 0, 0, 0 ) );
        }
        mUpdateFogOfWar = true;
        updateTurnOrderIndicators( );
    }
    return true;
}

void Grid::getTartgets( std::vector<sf::Vector2i> possibleAttackLocations )
{
    // std::cout << "Get Targets was called." << std::endl;
    for( auto loc : possibleAttackLocations )
        if( static_cast<unsigned int>( loc.y * mGridWidth + loc.x ) < mData.size() )
        {
            Square* location =  &mData[static_cast<unsigned int>( loc.y * mGridWidth + loc.x )];
            // if( ( location->isOccupied && mCurrentUnits[location->unitID]->mCategory != mCurrentUnits[mWorld->getSelectedUnit()]->mCategory ) )
                   // || ( location.buildingID > -1 && ( ( mCurrentBuildings.at( location.buildingID )->mCategory == Category::RedBuilding
                   //                                   && mCurrentUnits[mWorld->getSelectedUnit()]->mCategory == Category::RedUnit ))
                                                    //  || ( mCurrentBuildings.at( location.buildingID )->mCategory == Category::BlueBuilding
                                                    //  && mCurrentUnits[mWorld->getSelectedUnit()]->mCategory == Category::BlueUnit ) ) ) )
            {
                location->isPossibleAttackPosition = true;
                location->rect->getSprite( )->setFillColor( sf::Color( 255, 165, 0, 255 ) );
            }
        }
}

void Grid::clearGrid( void )
{
    for( unsigned int i = 0; i < mData.size(); ++i )
    {
        mData[i].isPossibleNewLocation = false;
        mData[i].isPossibleAttackPosition = false;
        mData[i].rect->getSprite()->setFillColor( sf::Color( 0, 0, 0, 0 ) );
        // clear influence map before update
        mData[i].offensiveInfluence = 0.0f;
        mData[i].defensiveInfluence = 0.0f;
        mData[i].tacticalInfluence = 0.0f;
    }
    mUpdateFogOfWar = true;
}

void Grid::endTurn( void )
{
    mEndTurn = false;
    updateTurnOrderIndicators( );

    int next = getNextUnit( );
    if( next > -1 )
    {
        mSelectedGridIndex = sf::Vector2i( mCurrentUnits.at( next )->mGridIndex.x, mCurrentUnits.at( next )->mGridIndex.y );
        mWorld->mWorldView.setCenter( mSelectedGridIndex.x * TILE_SIZE, mSelectedGridIndex.y * TILE_SIZE );

        mWorld->setSelectedUnit( mCurrentUnits.at( next )->mID );
        mCurrentUnits.at( next )->mIsSelectedUnit = true;
        mWorld->mStateStack->pushState( States::ActionMenuState );
    }
    // selectUnit( mCurrentUnits.at( next )->mGridIndex.x, mCurrentUnits.at( next )->mGridIndex.y );
}

bool Grid::handleEvent( sf::Event )
{
    return true;
}

void Grid::buildPossiblePositions( std::vector<sf::Vector2i>* mPossiblePositions, sf::Vector2i startingPoint, unsigned int unitType, int distanceLeft )
{
    if( distanceLeft >= 0 && static_cast<unsigned int>( startingPoint.y * mGridWidth + startingPoint.x ) < mData.size() )
    {
        Square* square = &mData[startingPoint.y * mGridWidth + startingPoint.x];
        mPossiblePositions->push_back( startingPoint );
        distanceLeft -= getMoveCost( unitType, square->terrainType );

        buildPossiblePositions( mPossiblePositions, sf::Vector2i( startingPoint.x, startingPoint.y - 1 ), unitType, distanceLeft );
        buildPossiblePositions( mPossiblePositions, sf::Vector2i( startingPoint.x, startingPoint.y + 1 ), unitType, distanceLeft );

        if( square != &mData[startingPoint.y * mGridWidth + 0] ) // square != farthest left square in a row
            buildPossiblePositions( mPossiblePositions, sf::Vector2i( startingPoint.x - 1, startingPoint.y ), unitType, distanceLeft );
        if( square != &mData[startingPoint.y * mGridWidth + (mGridWidth-1)] ) // square != farthest right squre in a row
            buildPossiblePositions( mPossiblePositions, sf::Vector2i( startingPoint.x + 1, startingPoint.y ), unitType, distanceLeft );
    }
}

unsigned int Grid::getMoveCost( int unitType, unsigned int terrainType )
{
    if( unitType == -1 ) return 1;
    return MoveCostTable[unitType][terrainType];
}

void Grid::updateFogOfWar( void )
{
    /*
    if( mUpdateFogOfWar && !mWorld->mLocalMultiplayerWorld )
    {
        // Fill fog completly
        for( unsigned int i = 0; i < mData.size(); ++i )
            mData[i].isVisible = false;

        // traverse units and buildPos using perception to mark visible squares
        for( auto unit : mCurrentUnits )
        {
            unit.second->mIsVisible = false;
            if( unit.second->mCategory & Category::Type::Blue )
            {
                std::vector<sf::Vector2i> visibleSquares;
                buildPossiblePositions( &visibleSquares, unit.second->mGridIndex, -1, unit.second->mPerception );
                for( auto loc : visibleSquares )
                    mData[loc.y * mGridWidth + loc.x].isVisible = true;
            }
            if( mData[unit.second->mGridIndex.y * mGridWidth + unit.second->mGridIndex.x].isVisible ) unit.second->mIsVisible = true;
        }

        // update fog based on new data
        for( auto square : mData )
            if( !square.isPossibleAttackPosition )
            {
                if( square.isVisible )
                {
                    square.rect->getSprite()->setFillColor( sf::Color( 0, 0, 0, 0 ) );
                    if( square.isOccupied ) mCurrentUnits.at( square.unitID )->mIsVisible = true;
                    else if( square.buildingID > -1 ) mCurrentBuildings.at( square.buildingID )->mIsVisible = true;
                }
                else
                {
                    if( square.buildingID > -1 ) mCurrentBuildings.at( square.buildingID )->mIsVisible = false;
                    square.rect->getSprite()->setFillColor( sf::Color( 0, 0, 0, 100 ) );
                }
            }
            else
            {
                if( square.isOccupied ) mCurrentUnits.at( square.unitID )->mIsVisible = true;
                else if( square.buildingID > -1 ) mCurrentBuildings.at( square.buildingID )->mIsVisible = true;
            }
    }
    */
}

int Grid::getNextUnit( void )
{
    Unit* unit = nullptr;
    while( true )
    {
        for( unsigned int i = 0; i < mCurrentUnits.size(); ++i )
            if( !unit || unit->mInitiative < mCurrentUnits.at( i )->mInitiative ) unit = mCurrentUnits.at(i);
        std::cout << "The Player has no available units to use. Open spawn point menu or end match here." << std::endl;
        assert( unit );
        if( unit->mInitiative < 99 )
            for( unsigned int i = 0; i < mCurrentUnits.size(); ++i ) mCurrentUnits.at(i)->mInitiative += randomInt( mCurrentUnits.at(i)->mSpeed * 10 ) + mCurrentUnits.at(i)->mSpeed * 2;
        else break;
    }
    return unit->mID;
}

void Grid::updateTurnOrderIndicators( void )
{
    std::vector<unsigned int> unitIDs;
    for( auto unit : mCurrentUnits )
    {
        bool flag = false;
        for( unsigned int i = 0; i < unitIDs.size(); ++i )
        {
            flag = unit.second->mInitiative < unitIDs[i];
            if( flag )
            {
                unitIDs.insert( unitIDs.begin() + i, unit.second->mID );
                break;
            }
        }
        if( !flag ) unitIDs.push_back( unit.second->mID );
    }
    int counter = 0;
    while( unitIDs.size( ) )
    {
        mCurrentUnits.at( unitIDs.back() )->mInitiativeHUD.setString( std::to_string( ++counter ) );
        unitIDs.pop_back( );
    }
}

void Grid::updateInfluenceMap( InfluenceType type)
{
    for( auto square : mData )
        switch( type )
        {
            case InfluenceType::Offensive:
                if( square.offensiveInfluence > 0 )
                    square.rect->getSprite()->setFillColor( sf::Color( 0, 0, 255, 255 * (-1*square.offensiveInfluence) ) ); // blue
                else if( square.offensiveInfluence < 0 )
                    square.rect->getSprite()->setFillColor( sf::Color( 255, 0, 0, 255 * square.offensiveInfluence ) ); // Red
                square.offensiveInfluence = 0;
            break;
            case InfluenceType::Defensive:
                if( square.defensiveInfluence > 0 )
                    square.rect->getSprite()->setFillColor( sf::Color( 0, 0, 255, 255 * (-1*square.defensiveInfluence) ) ); // blue
                else if( square.defensiveInfluence < 0 )
                    square.rect->getSprite()->setFillColor( sf::Color( 255, 0, 0, 255 * square.defensiveInfluence ) ); // Red
                square.defensiveInfluence = 0;
            break;
            case InfluenceType::BuidingInfluence:
                if( square.tacticalInfluence > 0 )
                    square.rect->getSprite()->setFillColor( sf::Color( 0, 0, 255, 255 * (-1*square.tacticalInfluence) ) ); // blue
                else if( square.tacticalInfluence < 0 )
                    square.rect->getSprite()->setFillColor( sf::Color( 255, 0, 0, 255 * square.tacticalInfluence ) ); // Red
                square.tacticalInfluence = 0;
            break;
        }
}


