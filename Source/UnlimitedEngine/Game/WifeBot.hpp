#ifndef WIFEBOT_HPP
#define WIFEBOT_HPP

#include "Game/Unit.hpp"
#include "Core/PathFinder.hpp"

class World;

class Zone
{
public:
    Zone( Zone* parent, float x, float y, float width, float height, int depth = 0 );

    float offensiveInfluence( void );
    float defensiveInfluence( void );

    /// \brief tacticalInfluence
    /// Designed to denote things like bards, and scouts that don't provide much in attack and defense but are still very important
    /// \return
    float tacticalInfluence( void );

    /// \brief importance
    /// the total of all unit and location values returned as a single score
    /// \return
    float importance( void );

    Category::Type ownedBy( void );

    /// \brief addObj
    /// Adds a obj to this node, or it's children if any and if they fit inside there boundries
    ///  \param obj
    bool addObj( Unit* obj );

    /// \brief split
    /// Splits the node into 4 new smaller nodes
    /// Method gets called when the node is carrying to many objs
    void split( void );

    /// \brief clear
    /// This method is meant to be called by the root node to clear the entire
    /// tree it can of course also be called by any node to clear all objs and
    /// childern before it.
    void clear( void );

    float heuristic( void );

    // Attributes
    unsigned int id;

    Zone* NE;
    Zone* NW;
    Zone* SE;
    Zone* SW;
    Zone* parent;
    /// \brief localObjs
    /// list of objects that fit inside this zone
    std::vector<Unit*> localObjs;
    /// \brief innerGrid
    /// list of squares on the grid that fit in this zone
    std::vector<Square> innerGrid;
    sf::Rect<float> rect;
    float depth; // depth from the root of the stucture
    bool hasKids; // flag to tell if the node has been split yet or not
    const unsigned int mMaxDepth = 2; // the max depth a node can be before the tree ends
    const unsigned int mMaxHeuristic = 25; // the max number of objs a node cn carry before it must be split.
};

///
/// \brief The WifeBot class
/// Wired Intelligence For Enemy Bot
class WifeBot
{
public:
    WifeBot( World* world, std::vector<Unit*>* mUnits);
    ~WifeBot( void );

    void update( sf::Time dt );
    void handleEvent( const sf::Event &event );
    void selectUnit( Unit* unit );

    void recalculate( bool flag ) { mRecalculate = flag; }
    unsigned int getCategory( ) const { return mCategory; }
    bool isMarkedForRemoval( )  const { return false; }
    bool isDestroyed( )         const { return false; }
    void initializeZones( Zone *rootZone );
    void findTargetZone( std::vector<unsigned int> excludedZones = std::vector<unsigned int>() );

    Category::Type mCategory;
    World* mWorld;
    //std::function<float(Square*, Square*)> mHeuristic;
    std::vector<Unit*>* mUnits;
    /// \brief mRecalculate
    /// Flag to signal whether or not to rerun logic for unit or to continue on current path
    bool mRecalculate;
private:
    /// \brief mRootZone
    /// A Quad tree structure used to divide up the map based on influences the troops impose on their surroundings
    Zone* mRootZone;

    /// \brief mZones
    /// A shortcut list of pointers to all the created children of mRootZone
    std::vector<Zone*> mZones;

    /// \brief mTargetZone
    /// The calculated most important zone to conquer in order to advance goals
    unsigned int mTargetZone;

    /// \brief mPathFinders
    /// Key is the unitId that the pathfinder belongs to.
    std::map<std::string, PathFinder<Square>*> mUnitPathFinders;

    /// \brief mAnimationTimer
    /// Allows the units enough time to move to their new squares
    sf::Time mAnimationTimer;

    /// \brief mAllUnitsMoved
    /// flag to tell whether or not to end the turn
    bool mAllUnitsMoved;
    /// \brief mUnit
    ///
    Unit* mUnit;
};

#endif // WIFEBOT_HPP
