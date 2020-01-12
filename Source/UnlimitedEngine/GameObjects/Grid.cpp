#include "Grid.hpp"

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
            mData[i].push_back( Square( TerrainTypeMap.at( tileSet.tiles.at( layer.data[counter]-1 ).at( "type" ) ), sf::Vector2f( i, j ) ) );
            counter++;
        }
    }
    return true;
}

std::vector<sf::Vector2i> Grid::getPossiblePositions( sf::Vector2i startingPoint, UnitTypeData unitType, unsigned int distanceLeft )
{
    std::vector<sf::Vector2i> positions;
    positions.push_back( startingPoint );

    if( distanceLeft > 0 )
    {
        std::vector<sf::Vector2i> top        = getPossiblePositions( sf::Vector2i( startingPoint.x, startingPoint.y + 1 ), unitType, distanceLeft - 1 );
        std::vector<sf::Vector2i> bottom     = getPossiblePositions( sf::Vector2i( startingPoint.x, startingPoint.y - 1 ), unitType, distanceLeft - 1 );
        std::vector<sf::Vector2i> left       = getPossiblePositions( sf::Vector2i( startingPoint.x - 1, startingPoint.y ), unitType, distanceLeft - 1 );
        std::vector<sf::Vector2i> right      = getPossiblePositions( sf::Vector2i( startingPoint.x + 1, startingPoint.y ), unitType, distanceLeft - 1 );

        // add new elements to current set.
        while( top.size() > 0 )
        {
            positions.push_back( top.back() );
            top.pop_back( );
        }
        while( bottom.size() > 0 )
        {
            positions.push_back( top.back() );
            bottom.pop_back( );
        }
        while( left.size() > 0 )
        {
            positions.push_back( top.back() );
            left.pop_back( );
        }
        while( right.size() > 0 )
        {
            positions.push_back( top.back() );
            right.pop_back( );
        }
    }
    // remove duplicates
    std::vector<sf::Vector2i> final;
    while( positions.size() > 0 )
    {
        bool flag = false;
        for( unsigned int i = 0; i < final.size(); ++i )
            if( positions.back() == final[i] || ( positions.back().x == final[i].y && positions.back().y == final[i].x ) )
            {
                flag = true;
                break;
            }
        if( !flag ) final.push_back( positions.back( ) );
        positions.pop_back();
    }

    return final;
}
