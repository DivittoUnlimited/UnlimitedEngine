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

#include "Game/DataTables.hpp"
#include "Tiled/TiledManager.hpp"
#include "Game/Unit.hpp"
#include "Game/Building.hpp"
#include "Graphics/RectangleShapeNode.hpp"

class World;

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
        , buildingID( -1 )
    {

    }

    unsigned int terrainType;
    sf::Vector2f gridIndex;
    bool isOccupied;
    bool isPossibleNewLocation;
    bool isPossibleAttackPosition;
    sf::Rect<float> mBounds;
    int unitID;
    int buildingID;
};

class Grid {
public:
    Grid(World *world);
    ~Grid( void );
    bool buildGrid( Tiled::TileSet tileSet, Tiled::Layer layer );
    std::vector<sf::Vector2i> getPossiblePositions(sf::Vector2i startingPoint, unsigned int unitType, int distanceLeft );
    std::vector<sf::Vector2i> getPossibleAttackPositions( sf::Vector2i startingPoint,  int innerRange, int outerRange );
    void addUnit(Unit *unit);
    void addBuilding( Building* building );

    void removeUnit( sf::Vector2i position );
    void selectUnit( unsigned int i, unsigned int j );
    bool moveUnit( sf::Vector2i currentPos, sf::Vector2i newPos );
    void getTartgets(unsigned int i , unsigned int j);
    void clearGrid( void );

    bool handleEvent( sf::Event event, World* world );

    // Attributes
    World* mWorld;

    std::vector<std::vector<Square>> mData;

    ///
    /// \brief mCurrentUnits
    /// List of all the units currently on the field
    std::vector<Unit*>                  mCurrentUnits;
    ///
    /// \brief mCurrentBuildings
    /// List of all the buildings currently on the field.
    std::vector<Building*>              mCurrentBuildings;
    ///
    /// \brief mUnitAwaitingOrders
    /// flag for whether or not a unit has been selected by the player
    bool                                mUnitAwaitingOrders;



    ///
    /// \brief mSelectedGridIndex
    /// The index of the grid square that was previously selected, if any.
    /// allows for easy access to elements in the mMovementGrid structure
    sf::Vector2i                        mSelectedGridIndex;
    ///
    /// \brief mDrawableGrid
    /// The Graphical representation of the grid, (outline of squares, highlighting moves/attacks etc...)
    std::vector<std::vector<RectangleShapeNode*>>    mDrawableGrid;

private:
    void buildPossiblePositions(std::vector<sf::Vector2i> *mPossiblePositions, sf::Vector2i startingPoint, unsigned int unitType, int distanceLeft );
    void buildPossibleTargets( std::vector<sf::Vector2i> *mPossiblePositions, sf::Vector2i startingPoint, int distanceLeft );

    unsigned int getMoveCost( int unitType, unsigned int terrainType );

};

#endif // GRID_H_
