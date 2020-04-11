#ifndef WIFEBOT_HPP
#define WIFEBOT_HPP

#include "Graphics/SceneNode.hpp"
#include "Game/Grid.hpp"
#include "Core/PathFinder.hpp"

class Zone
{
public:
    Zone( Zone* parent, float x, float y, float width, float height, int depth = 0 );

    float offensiveInfluence( void );
    float defensiveInfluence( void );
    ///
    /// \brief tacticalInfluence
    /// Designed to denote things like bards, and scouts that don't provide much in attack and defense but are still very important
    /// \return
    float tacticalInfluence( void );
    ///
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
    const unsigned int mMaxDepth = 3; // the max depth a node can be before the tree ends
    const unsigned int mMaxHeuristic = 24; // the max number of objs a node cn carry before it must be split.
};

///
/// \brief The WifeBot class
/// Wired Intelligence For Enemy Bot
class WifeBot : public SceneNode
{
public:
    WifeBot( Grid *grid, std::map<unsigned int, Unit*>* mUnits, std::vector<Building*> *mBuildings , Category::Type *currentTurn);
    ~WifeBot( void );

    void updateCurrent( sf::Time dt, CommandQueue &queue );
    void handleEvent( const sf::Event &event );
    void selectUnit( Unit* unit );

    void recalculate( bool flag ) { mRecalculate = flag; }
    unsigned int getCategory( ) const { return mCategory; }
    bool isMarkedForRemoval( )  const { return false; }
    bool isDestroyed( )         const { return false; }
    void initializeZones( Zone *rootZone );
    void findTargetZone( std::vector<unsigned int> excludedZones = std::vector<unsigned int>() );

    Category::Type mCategory;
    Grid* mGrid;
    //std::function<float(Square*, Square*)> mHeuristic;
    std::map<unsigned int, Unit*>* mUnits;
    std::vector<Building*>* mBuildings;

private:
    Zone* mRootZone;


    Zone* mCurrentZone;

    ///
    /// \brief mZones
    /// A shortcut list of pointers to all the created children of mRootZone
    std::vector<Zone*>       mZones;

    ///
    /// \brief mPathFinders
    /// Key is the unitId that the pathfinder belongs to.
    std::map<unsigned int, PathFinder<Square>*> mUnitPathFinders;
    bool mRecalculate;
    sf::Time mAnimationTimer;
    Category::Type* mCurrentTurn;
    bool mAllUnitsMoved;

    unsigned int mTargetZone;
};

#endif // WIFEBOT_HPP
