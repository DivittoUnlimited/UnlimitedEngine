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
class Unit;

class Square
{
public:
    Square( unsigned int terrainType, sf::Vector2i gridIndex, sf::Vector2f pos, float size, bool isOccupied = false )
        : terrainType( terrainType )
        , gridIndex( gridIndex )
        , isOccupied( isOccupied )
        , isPossibleNewLocation( false )
        , isPossibleAttackPosition( false )
        , mBounds( pos, sf::Vector2f( size, size ) )
        , unitID( -1 )
        , buildingID( -1 )
        , influence( 0.0f )
        , threatLevel( 0.0f )
        , costSoFar( 0.0f )
        , heuristicCost( 0.0f )
        , parent( nullptr )
    {
    }

    bool operator==( const Square* other ) { return this->gridIndex == other->gridIndex; }
    bool operator<( const Square* other ) { return( this->costSoFar + this->heuristicCost < other->costSoFar + other->heuristicCost ); }

    unsigned int terrainType;
    sf::Vector2i gridIndex;
    bool isOccupied;
    bool isPossibleNewLocation;
    bool isPossibleAttackPosition;
    sf::Rect<float> mBounds;
    int unitID;
    int buildingID;
    ///
    /// \brief influence
    /// range of -1.0 -> 1.0
    /// positive values represent blue control and vice-versa 0 if unneeded or out of range of both teams
    float influence;
    ///
    /// \brief influence
    /// range of -1.0 -> 1.0
    /// positive values represent blue control and vice-versa 0 if unneeded or out of range of both teams
    float threatLevel;


    // used by WifeBot
    float costSoFar;
    float heuristicCost;
    Square* parent;
};

class Grid : public SceneNode {
public:
    Grid(World *world);
    ~Grid( void );

    virtual unsigned int	getCategory( ) const;
    virtual bool			isMarkedForRemoval( ) const;
    virtual bool			isDestroyed( ) const;
    virtual void			updateCurrent( sf::Time dt, CommandQueue& commands );
    virtual void			drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const;

    void handleLeftClick(sf::Vector2i pos );


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

    bool handleEvent(sf::Event event);
    unsigned int getMoveCost( int unitType, unsigned int terrainType );


    // Attributes
    World* mWorld;
    ///
    /// \brief mData
    /// The main data structure for the whole battle has info on unit and building positions as well as the terrain type
    /// of the grid squares, the influence each team has on a square, and how much of a threat a square is to each team.
    std::vector<std::vector<Square>> mData;
    ///
    /// \brief mCurrentUnits
    /// List of all the units currently on the field
    std::map<unsigned int, Unit*>                  mCurrentUnits;
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
    bool mUpdateInfluenceMap;
    bool mUpdateThreatLevelMap;
    bool mShowInfluenceMap;
    bool mShowThreatLevelMap;

private:
    void buildPossiblePositions(std::vector<sf::Vector2i> *mPossiblePositions, sf::Vector2i startingPoint, unsigned int unitType, int distanceLeft );
    void buildPossibleTargets( std::vector<sf::Vector2i> *mPossiblePositions, sf::Vector2i startingPoint, int distanceLeft );
    void updateInfluenceMap();
    void updateThreatLevelMap();

};

#endif // GRID_H_
