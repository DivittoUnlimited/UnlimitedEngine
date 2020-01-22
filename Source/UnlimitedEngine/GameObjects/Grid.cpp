#include "Grid.hpp"
#include "Core/DataTables.hpp"

namespace
{
    const std::vector<std::vector<int>> MoveCostTable = inititializeUnitMovementCostTable;
}

Grid::Grid()
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
                                            sf::Vector2f( i, j ), sf::Vector2f( i * 32, j * 32 ), 32, false ) );
            counter++;
        }
    }
    mHasUpdated = true;
    return true;
}

std::vector<sf::Vector2i> Grid::getPossiblePositions( sf::Vector2i startingPoint, unsigned int unitType, int distanceLeft )
{
    std::vector<sf::Vector2i> positions;
    positions.push_back( startingPoint );

    distanceLeft -= MoveCostTable.at( unitType ).at( mData[startingPoint.x][startingPoint.y].terrainType );
    if( distanceLeft > 0 )
    {
        std::vector<sf::Vector2i> top       = getPossiblePositions( sf::Vector2i( startingPoint.x, startingPoint.y + 1 ), unitType,
                                                                     distanceLeft );
        std::vector<sf::Vector2i> bottom    = getPossiblePositions( sf::Vector2i( startingPoint.x, startingPoint.y - 1 ), unitType,
                                                                     distanceLeft );
        std::vector<sf::Vector2i> left      = getPossiblePositions( sf::Vector2i( startingPoint.x - 1, startingPoint.y ), unitType,
                                                                     distanceLeft );
        std::vector<sf::Vector2i> right     = getPossiblePositions( sf::Vector2i( startingPoint.x + 1, startingPoint.y ), unitType,
                                                                     distanceLeft );

        // add new elements to current set.
        while( top.size( ) > 0 )
        {
            positions.push_back( top.back( ) );
            top.pop_back( );
        }
        while( bottom.size( ) > 0 )
        {
            positions.push_back( bottom.back( ) );
            bottom.pop_back( );
        }
        while( left.size( ) > 0 )
        {
            positions.push_back( left.back( ) );
            left.pop_back( );
        }
        while( right.size( ) > 0 )
        {
            positions.push_back( right.back( ) );
            right.pop_back( );
        }
    }
    // remove duplicates
    std::vector<sf::Vector2i> final;
    while( positions.size() > 0 )
    {
        bool flag = false;
        for( unsigned int i = 0; i < final.size( ); ++i )
        {
            if( positions.back() == final[i] )
            {
                flag = true;
                break;
            }
        }
        if( !flag ) final.push_back( positions.back( ) );
        positions.pop_back( );
    }
    return final;
}

void Grid::addUnit( unsigned int unitID, float x, float y )
{
    bool flag = false;
    for( unsigned int i = 0; i < mData.size(); ++i )
    {
        for( unsigned int j = 0; j < mData[i].size(); ++j )
        {
            if( mData[j][i].mBounds.contains( x, y ) )
            {
                mData[j][i].isOccupied = true;
                mData[j][i].unitID = unitID;
                flag = true;
                break;
            }
            if( flag ) break;
        }
    }
    mHasUpdated = true;
}

bool Grid::moveUnit( sf::Vector2i currentPos, sf::Vector2i newPos )
{
    mData[newPos.y][newPos.x].unitID = mData[currentPos.x][currentPos.y].unitID;
    mData[currentPos.x][currentPos.y].unitID = 0;
    mData[currentPos.x][currentPos.y].isOccupied = false;
    mData[newPos.y][newPos.x].isOccupied = true;
    mHasUpdated = true;
    return true;
}

void Grid::clearGrid( void )
{
    for( unsigned int i = 0; i < mData.size(); ++i )
    {
        for( unsigned int j = 0; j < mData[i].size(); ++j )
        {
            mData[j][i].isPossibleNewLocation = false;
            mData[j][i].isPossibleAttackPosition = false;
        }
    }
    mHasUpdated = true;
}
