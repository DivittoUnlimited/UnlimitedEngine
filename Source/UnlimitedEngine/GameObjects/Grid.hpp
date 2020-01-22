/*
 * Will have access to dataTables.hpp to help construct itself.
 * It will need to know about  unitTypes and their data as well as terrain types
 * and the unit - terrain difficulty table
 *
 * The grid will also need info from the tiled lua map file to know the grid size, tile size, etc...
 *  - there will be a gameState that passes all that info to the grid that already manipulates the tiled files anyway.....
 *
 * The grids drawable components should remain seperate from the logic.
 *
 * Grid's main purpose is to abstract the use of a 2D array to represent the game grid as tiles or squares.
 *
 * the squares are structs that have limited info such as its unique id, position, terrainType, and whether or not is currently occupied by something
 *
 * Grid will have interface methods for things like returning a list of possible places a unit can move or attack.
 *
 *
 */
#ifndef GRID_H_
#define GRID_H_

#include <set>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "Core/DataTables.hpp"
#include "Tiled/TiledManager.hpp"
#include "GameObjects/Unit.hpp"

struct Square
{
    Square( unsigned int terrainType, sf::Vector2f gridIndex, sf::Vector2f pos, float size, bool isOccupied = false )
        : terrainType( terrainType )
        , gridIndex( gridIndex )
        , isOccupied( isOccupied )
        , isPossibleNewLocation( false )
        , isPossibleAttackPosition( false )
        , mBounds( pos, sf::Vector2f( size, size ) )
        , unitID( -1 )
    {

    }

    unsigned int terrainType;
    sf::Vector2f gridIndex;
    bool isOccupied;
    bool isPossibleNewLocation;
    bool isPossibleAttackPosition;
    sf::Rect<float> mBounds;
    int unitID;
};


class Grid {
public:
    Grid();

    bool buildGrid( Tiled::TileSet tileSet, Tiled::Layer layer );
    std::vector<sf::Vector2i> getPossiblePositions(sf::Vector2i startingPoint, unsigned int unitType, int distanceLeft );
    void addUnit(unsigned int unitID, float x, float y );

    bool moveUnit( sf::Vector2i currentPos, sf::Vector2i newPos );
    void clearGrid( void );

    // Attributes
    std::vector<std::vector<Square>> mData;
    bool mHasUpdated;
};

#endif // GRID_H_
