#ifndef WIFEBOT_HPP
#define WIFEBOT_HPP

#include "Graphics/SceneNode.hpp"
#include "Game/Grid.hpp"
#include "Core/PathFinder.hpp"

class Zone
{
public:
    Zone( void );

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

    // Attributes
    unsigned int id;
    Zone* parent;
    Zone* NE;
    Zone* NW;
    Zone* SE;
    Zone* SW;

    std::vector<Square*> innerGrid;
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
    void initializeZones( void );
    void findTargetZone( std::vector<unsigned int> excludedZones = std::vector<unsigned int>() );

    Category::Type mCategory;
    Grid* mGrid;
    //std::function<float(Square*, Square*)> mHeuristic;
    std::map<unsigned int, Unit*>* mUnits;
    std::vector<Building*>* mBuildings;
    std::vector<Zone>       mZones;

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
