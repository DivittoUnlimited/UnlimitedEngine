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
    , mUnitAwaitingOrders( false )
    , mGridWidth( gridWidth )
    , mGridHeight( gridHeight )
    , mUpdateFogOfWar( true )
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
    //if( mUpdateFogOfWar )
    {
        //updateFogOfWar( );
        //mUpdateFogOfWar = false;
    }

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
                if( !mUnitAwaitingOrders && square->isOccupied )
                    selectUnit( i, j );
                else if( !mUnitAwaitingOrders && mWorld->getSelectedBuilding( ) > -1 ) //this is dumb....
                {
                    // building selected but not unit!!!
                    if( square->isPossibleNewLocation )
                    {
                        mWorld->spawnUnit( static_cast<unsigned int>(mWorld->getSelectedUnit()), sf::Vector2i( static_cast<int>(i), static_cast<int>(j) ) );
                        if( mWorld->mNetworkedWorld )
                            mWorld->mNetworkNode->notifyGameAction( GameActions::Type::SpawnUnit, sf::Vector2f( i, j ) );
                        mCurrentUnits.at( mCurrentUnits.size() - 1 )->mHasMoved = true;
                        clearGrid( );
                    }
                }
                // Units previously selected
                else if( square->isPossibleNewLocation )
                {
                    if( square->isOccupied || square->buildingID != -1 ) clearGrid( );
                    else moveUnit( mSelectedGridIndex, sf::Vector2i( static_cast<int>( i ), static_cast<int>( j ) ) );
                }
                else if( square->isOccupied && square->isPossibleAttackPosition && !( mCurrentUnits.at( static_cast<unsigned int>(square->unitID) )->mCategory &
                                                               mCurrentUnits.at( static_cast<unsigned int>(mData[mSelectedGridIndex.y * mGridWidth + mSelectedGridIndex.x].unitID) )->mCategory ) )
                {
                    ///
                    // ENTER COMBAT ALGORITHM HERE!!!!!!!!!!!!
                    ///
                    mCurrentUnits.at( static_cast<unsigned int>(square->unitID) )->takeDamage( 75 );
                    mCurrentUnits[static_cast<unsigned int>(mWorld->getSelectedUnit())]->mHasSpentAction = true;
                    mCurrentUnits[static_cast<unsigned int>(mWorld->getSelectedUnit())]->mIsSelectedUnit = false;
                    if( mCurrentUnits.at( static_cast<unsigned int>(square->unitID) )->mConstitution <= 0 )
                    {
                        mCurrentUnits.erase( static_cast<unsigned int>(square->unitID) );
                        removeUnit( sf::Vector2i( static_cast<int>( i ), static_cast<int>( j ) ) );
                    }
                    clearGrid();
                }
                else if( square->buildingID != -1 )
                {
                    if( mCurrentBuildings.at( static_cast<unsigned int>(square->buildingID) )->mType == "SpawnPoint" )
                    {
                        if( (mCurrentBuildings.at( static_cast<unsigned int>(square->buildingID) )->mCategory & mWorld->mCurrentTurn)
                                && ( !mWorld->mNetworkedWorld || mWorld->mCurrentTurn & mWorld->mClientTeamColor ) )
                        {
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
                        else if( !(mCurrentBuildings.at( static_cast<unsigned int>(square->buildingID) )->mCategory & mWorld->mCurrentTurn)
                                 && ( !mWorld->mNetworkedWorld || !(mWorld->mCurrentTurn & mWorld->mClientTeamColor) ) )

                        {
                            auto building = mCurrentBuildings.at( static_cast<unsigned int>(square->buildingID) );
                            building->mCapturePercentage += 0.5f;
                            if( building->mCapturePercentage >= 1.0f )
                            {
                                if( building->mCategory & Category::Red )
                                    building->mCategory = Category::BlueBuilding;
                                else if( building->mCategory & Category::Blue )
                                    building->mCategory = Category::RedBuilding;
                                building->mCapturePercentage = 0.0f;
                            }
                            clearGrid( );
                        }
                    }
                    else
                        clearGrid(); // untill i add more buildings
                }
                else clearGrid( );

                flag = true;
                break;
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
            std::unique_ptr<TextNode> debugText( new TextNode( mWorld->mFonts, "X" ) );
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
}

void Grid::selectUnit( unsigned int i, unsigned int j )
{
    Unit* unit = mCurrentUnits[mData[j * mGridWidth + i].unitID];
    if( unit && (unit->mCategory & mWorld->mCurrentTurn) ) // is it this unit's turn?
    {
        if( !unit->mHasMoved ) // units can only move once per turn
        {
            std::vector<sf::Vector2i> possibleLocations = getPossiblePositions(
                                                        sf::Vector2i( static_cast<int>( i ), static_cast<int>( j ) ),
                                                        unit->mUnitType, static_cast<unsigned int>( unit->mDexterity ) );
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
            mUnitAwaitingOrders = true;
        }
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
        getTartgets( newPos.x, newPos.y );
        mUpdateFogOfWar = true;
    }
    return true;
}

void Grid::getTartgets( unsigned int i, unsigned int j )
{
    // Show potential targets if any if none clear data for next click
    std::vector<sf::Vector2i> possibleAttackLocations = getPossibleAttackPositions(
                                                    sf::Vector2i( static_cast<int>( i ), static_cast<int>( j ) ),
                                                    static_cast<unsigned int>( mCurrentUnits[mWorld->getSelectedUnit()]->mRange.x ),
                                                    static_cast<unsigned int>( mCurrentUnits[mWorld->getSelectedUnit()]->mRange.y ) );
    bool flag = false;
    for( auto loc : possibleAttackLocations )
    {
        if( static_cast<unsigned int>( loc.y * mGridWidth + loc.x ) < mData.size() )
        {
            Square location =  mData[static_cast<unsigned int>( loc.y * mGridWidth + loc.x )];
            if( (location.unitID > -1 && mCurrentUnits[location.unitID]->mCategory != mCurrentUnits[mWorld->getSelectedUnit()]->mCategory) || ( location.buildingID != -1 ) )
            {
                mData[static_cast<unsigned int>( loc.y * mGridWidth + loc.x )].isPossibleAttackPosition = true;
                mData[static_cast<unsigned int>( loc.y * mGridWidth + loc.x )].rect->getSprite()->setFillColor( sf::Color( 255, 165, 0, 255 ) );
                flag = true;
            }
        }
    }
    if( flag )
    {
        mSelectedGridIndex = sf::Vector2i( i, j );
        mWorld->setSelectedUnit( mData[j * mGridWidth + i].unitID );
        mUnitAwaitingOrders = true;
    }
    else // no targets
        clearGrid( );
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
    mWorld->setSelectedUnit( -1 );
    mWorld->setSelectedBuilding( -1 );
    mSelectedGridIndex = sf::Vector2i( 0, 0 );
    mUnitAwaitingOrders = false;
    mUpdateFogOfWar = true;
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
        if(square->buildingID == -1 || !(mCurrentBuildings.at( square->buildingID )->mCategory & mWorld->mCurrentTurn) )
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
            if( !square.isPossibleAttackPosition && !square.isPossibleNewLocation )
            {
                if( square.isVisible ) square.rect->getSprite()->setFillColor( sf::Color( 0, 0, 0, 0 ) );
                else square.rect->getSprite()->setFillColor( sf::Color( 0, 0, 0, 100 ) );
            }
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


