#ifndef WIFEBOT_HPP
#define WIFEBOT_HPP

#include "Graphics/SceneNode.hpp"
#include "Game/Grid.hpp"
#include "Core/PathFinder.hpp"

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

    Category::Type mCategory;
    Grid* mGrid;
    //std::function<float(Square*, Square*)> mHeuristic;
    std::map<unsigned int, Unit*>* mUnits;
    std::vector<Building*>* mBuildings;
    ///
    /// \brief mPathFinders
    /// Key is the unitId that the pathfinder belongs to.
    std::map<unsigned int, PathFinder<Square>*> mUnitPathFinders;
    bool mRecalculate;
    sf::Time mAnimationTimer;
    Category::Type* mCurrentTurn;
};

#endif // WIFEBOT_HPP
