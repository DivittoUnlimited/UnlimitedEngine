#include "Grid.hpp"
#include "Game/DataTables.hpp"
#include <SFML/Window/Event.hpp>
#include "Core/Globals.hpp"
#include "Game/World.hpp"

namespace
{
    const std::vector<std::vector<int>> MoveCostTable = inititializeUnitMovementCostTable;
}

Grid::Grid( World* world )
    : mWorld( world )
{

}

Grid::~Grid( void )
{

}

bool Grid::buildGrid(  Tiled::TileSet tileSet, Tiled::Layer layer )
{
    unsigned int counter = 0;
    for( unsigned int i = 0; i < layer.height; ++i )
    {
        mData.push_back( std::vector<Square>( ) );
        for( unsigned int j = 0; j < layer.width; ++j )
        {
            mData.back().push_back( Square( TerrainTypeMap.at( tileSet.tiles.at( layer.data[counter]-1 ).at( "type" ) ),
                                            sf::Vector2f( j, i ), sf::Vector2f( j * 64, i * 64), 64, false ) );
            counter++;
        }
    }
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
    bool flag = false;
    for( unsigned int i = 0; i < mData.size(); ++i )
    {
        for( unsigned int j = 0; j < mData[i].size(); ++j )
        {
            if( mData[i][j].mBounds.contains( unit->getPosition().x, unit->getPosition().y + 32 ) )
            {
                mData[i][j].isOccupied = true;
                mData[i][j].unitID = unit->mID;
                mCurrentUnits.push_back( unit );
                flag = true;
                break;
            }
            if( flag ) break;
        }
    }
}

void Grid::addBuilding( Building* building )
{
    bool flag = false;
    for( unsigned int i = 0; i < mData.size(); ++i )
    {
        for( unsigned int j = 0; j < mData[i].size(); ++j )
        {
            if( mData[i][j].mBounds.contains( building->getPosition().x, building->getPosition().y ) )
            {
                mData[i][j].buildingID = building->mID;
                building->mGridIndex = sf::Vector2i( i, j );
                mCurrentBuildings.push_back( building );
                flag = true;
                break;
            }
            if( flag ) break;
        }
    }
}

void Grid::removeUnit( sf::Vector2i position )
{
    mData[position.x][position.y].unitID = -1;
    mData[position.x][position.y].isOccupied = false;
}

void Grid::selectUnit( unsigned int i, unsigned int j )
{
    clearGrid();
    Unit* unit = mCurrentUnits[mData[i][j].unitID];
    if( unit && (unit->mCategory & CURRENT_TURN) ) // is it this unit's turn?
    {
        if( !unit->mHasMoved ) // units can only move once per turn
        {
            std::vector<sf::Vector2i> possibleLocations = getPossiblePositions(
                                                        sf::Vector2i( static_cast<int>( i ), static_cast<int>( j ) ),
                                                        unit->mUnitType, static_cast<unsigned int>( unit->mDexterity ) );
            for( auto loc : possibleLocations )
            {
                mData[static_cast<unsigned int>( loc.x )][static_cast<unsigned int>( loc.y )].isPossibleNewLocation = true;
                mDrawableGrid[static_cast<unsigned int>( loc.x )][static_cast<unsigned int>( loc.y )]->getSprite()->setFillColor( sf::Color::Cyan );
            }
            mSelectedGridIndex = sf::Vector2i( i, j );
            mWorld->setSelectedUnit( unit->mID );
            mUnitAwaitingOrders = true;
        }
    }
}

bool Grid::moveUnit( sf::Vector2i currentPos, sf::Vector2i newPos )
{
    mData[newPos.x][newPos.y].unitID = mData[currentPos.x][currentPos.y].unitID;
    mData[currentPos.x][currentPos.y].unitID = 0;
    mData[currentPos.x][currentPos.y].isOccupied = false;
    mData[newPos.x][newPos.y].isOccupied = true;
    mCurrentUnits[mWorld->getSelectedUnit()]->setPosition( newPos.y * 64, newPos.x * 64 - 32 );
    mCurrentUnits[mWorld->getSelectedUnit()]->mHasMoved = true;

    // remove movement squares
    for( unsigned int i = 0; i < mData.size(); ++i )
        for( unsigned int j = 0; j < mData[i].size(); ++j )
        {
            mData[i][j].isPossibleNewLocation = false;
            mDrawableGrid[i][j]->getSprite()->setFillColor( sf::Color( 0, 0, 0, 0 ) );
        }
    getTartgets( newPos.x, newPos.y );
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
        if( (loc.x > 0 && static_cast<unsigned int>( loc.x ) < mData.size()) && (loc.y > 0 && static_cast<unsigned int>( loc.y ) < mData[loc.x].size()) )
        {
            Square location = mData[static_cast<unsigned int>( loc.x )][static_cast<unsigned int>( loc.y )];
            if( location.isOccupied && mCurrentUnits[location.unitID]->mCategory != mCurrentUnits[mWorld->getSelectedUnit()]->mCategory )
            {
                mData[static_cast<unsigned int>( loc.x )][static_cast<unsigned int>( loc.y )].isPossibleAttackPosition = true;
                mDrawableGrid[static_cast<unsigned int>( loc.x )][static_cast<unsigned int>( loc.y )]->getSprite()->setFillColor( sf::Color::Red );
                flag = true;
            }
            else if( location.buildingID != -1 )
            {
                mData[static_cast<unsigned int>( loc.x )][static_cast<unsigned int>( loc.y )].isPossibleAttackPosition = true;
                mDrawableGrid[static_cast<unsigned int>( loc.x )][static_cast<unsigned int>( loc.y )]->getSprite()->setFillColor( sf::Color::Red );
                flag = true;
            }
        }
    }
    if( flag )
    {
        mSelectedGridIndex = sf::Vector2i( i, j );
        mWorld->setSelectedUnit( mData[i][j].unitID );
        mUnitAwaitingOrders = true;
    }
    else // no targets
        clearGrid( );
}

void Grid::clearGrid( void )
{
    for( unsigned int i = 0; i < mData.size(); ++i )
    {
        for( unsigned int j = 0; j < mData[i].size(); ++j )
        {
            mData[i][j].isPossibleNewLocation = false;
            mData[i][j].isPossibleAttackPosition = false;
            mDrawableGrid[i][j]->getSprite()->setFillColor( sf::Color( 0, 0, 0, 0 ) );
        }
    }
    mWorld->setSelectedUnit( -1 );
    mWorld->setSelectedBuilding( -1 );
    mSelectedGridIndex = sf::Vector2i( 0, 0 );
    mUnitAwaitingOrders = false;
}

bool Grid::handleEvent( sf::Event event )
{
    if( event.mouseButton.button == sf::Mouse::Button::Left )
    {
        bool flag = false;
        // Find Which Square on the grid the user clicked
        for( unsigned int i = 0; i < mData.size(); ++i )
        {
            for( unsigned int j = 0; j < mData[i].size(); ++j )
            {
                if( mData[i][j].mBounds.contains( sf::Mouse::getPosition(*mWindow).x, sf::Mouse::getPosition(*mWindow).y ) )
                {
                    if( !mUnitAwaitingOrders && mData[i][j].isOccupied ) selectUnit( i, j );
                    else if( !mUnitAwaitingOrders && mWorld->getSelectedBuilding( ) > -1 ) //this is dumb....
                    {
                        // building selected but not unit!!!
                        if( mData[i][j].isPossibleNewLocation )
                        {
                            mWorld->spawnUnit( static_cast<unsigned int>(mWorld->getSelectedUnit()), sf::Vector2i( static_cast<int>(i), static_cast<int>(j) ) );
                            mCurrentUnits.at( mCurrentUnits.size() - 1 )->mHasMoved = true;
                            clearGrid( );
                        }
                    }
                    // Units previously selected
                    else if( mData[i][j].isPossibleNewLocation )
                    {
                        if( mData[i][j].isOccupied || mData[i][j].buildingID != -1 ) clearGrid( );
                        else moveUnit( mSelectedGridIndex, sf::Vector2i( static_cast<int>( i ), static_cast<int>( j ) ) );
                    }
                    else if( mData[i][j].isOccupied && mData[i][j].isPossibleAttackPosition && ( mCurrentUnits.at( static_cast<unsigned int>(mData[i][j].unitID) )->mCategory !=
                                                                   mCurrentUnits.at( static_cast<unsigned int>(mData[static_cast<unsigned int>(mSelectedGridIndex.x)][static_cast<unsigned int>(mSelectedGridIndex.y)].unitID) )->mCategory ) )
                    {
                        ///
                        // ENTER COMBAT ALGORITHM HERE!!!!!!!!!!!!
                        ///
                        mCurrentUnits.at( static_cast<unsigned int>(mData[i][j].unitID) )->mConstitution = 0;
                        mCurrentUnits[static_cast<unsigned int>(mWorld->getSelectedUnit())]->mHasSpentAction = true;
                        removeUnit( sf::Vector2i( static_cast<int>( i ), static_cast<int>( j ) ) );
                        clearGrid();
                    }
                    else if( mData[i][j].buildingID != -1 )
                    {
                        if( mCurrentBuildings.at( static_cast<unsigned int>(mData[i][j].buildingID) )->mType == "SpawnPoint" )
                        {
                            if( mCurrentBuildings.at( static_cast<unsigned int>(mData[i][j].buildingID) )->mCategory & CURRENT_TURN )
                            {
                                clearGrid( );
                                mWorld->setSelectedBuilding( mData[i][j].buildingID );
                                mWorld->mStateStack->pushState( States::SpawnPointMenuState );

                                // where to spawn the unit??
                                std::vector<sf::Vector2i> possibleLocations = getPossiblePositions( mCurrentBuildings.at( static_cast<unsigned int>(mWorld->getSelectedBuilding()) )->mGridIndex, static_cast<unsigned int>(mWorld->getSelectedUnit()), 1 );

                                for( auto loc : possibleLocations )
                                {
                                    mData[static_cast<unsigned int>( loc.x )][static_cast<unsigned int>( loc.y )].isPossibleNewLocation = true;
                                    mDrawableGrid[static_cast<unsigned int>( loc.x )][static_cast<unsigned int>( loc.y )]->getSprite()->setFillColor( sf::Color::Cyan );
                                }
                            }
                            else // enemy spawn point
                            {
                                auto building = mCurrentBuildings.at( static_cast<unsigned int>(mData[i][j].buildingID) );
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
    else if( event.mouseButton.button == sf::Mouse::Button::Right )
    {
        clearGrid( );
        // Find Which Square on the grid the user clicked
        for( unsigned int i = 0; i < mData.size(); ++i )
            for( unsigned int j = 0; j < mData[i].size(); ++j )
               if( mData[i][j].isOccupied && mData[i][j].mBounds.contains( sf::Mouse::getPosition(*mWindow).x, sf::Mouse::getPosition(*mWindow).y )  )
               {
                   std::vector<sf::Vector2i> loc = getPossibleAttackPositions( sf::Vector2i( i, j ), mCurrentUnits.at( mData[i][j].unitID )->mRange.x, mCurrentUnits.at( mData[i][j].unitID )->mRange.y  );
                   // Show Squares without making them clickable
                   for( unsigned int k = 0; k < loc.size( ); ++k ) if( static_cast<unsigned int>( loc[k].x ) != i || static_cast<unsigned int>(loc[k].y) != j ) mDrawableGrid[loc[k].x][loc[k].y]->getSprite()->setFillColor( sf::Color( 255, 0, 0, 255 ) );
               }
    }
    return true;
}

void Grid::buildPossiblePositions( std::vector<sf::Vector2i>* mPossiblePositions, sf::Vector2i startingPoint, unsigned int unitType, int distanceLeft )
{
    if( distanceLeft >= 0 && startingPoint.x >= 0 && startingPoint.y >= 0 && static_cast<unsigned int>(startingPoint.x) < mData.size() && static_cast<unsigned int>(startingPoint.y) < mData[startingPoint.x].size() )
    {
        if(  mData[startingPoint.x][startingPoint.y].buildingID == -1 || !(mCurrentBuildings.at( mData[startingPoint.x][startingPoint.y].buildingID )->mCategory & CURRENT_TURN) )
            mPossiblePositions->push_back( startingPoint );
        distanceLeft -= getMoveCost( unitType, mData[startingPoint.x][startingPoint.y].terrainType );

        buildPossiblePositions( mPossiblePositions, sf::Vector2i( startingPoint.x, startingPoint.y - 1 ), unitType, distanceLeft );
        buildPossiblePositions( mPossiblePositions, sf::Vector2i( startingPoint.x, startingPoint.y + 1 ), unitType, distanceLeft );
        buildPossiblePositions( mPossiblePositions, sf::Vector2i( startingPoint.x - 1, startingPoint.y ), unitType, distanceLeft );
        buildPossiblePositions( mPossiblePositions, sf::Vector2i( startingPoint.x + 1, startingPoint.y ), unitType, distanceLeft );
    }
}

unsigned int Grid::getMoveCost( int unitType, unsigned int terrainType )
{
    if( unitType == -1 ) return 1;
    return MoveCostTable[unitType][terrainType];
}



