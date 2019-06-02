#ifndef AICONTROLLER_HPP
#define AICONTROLLER_HPP

#include <SFML/System/Time.hpp>

#include "Core/SceneNode.hpp"
#include "Core/CommandQueue.hpp"
#include "Core/FiniteStateMachine.hpp"
#include "Core/Globals.hpp"
#include "Core/KeyBinding.hpp"


class StarShip;
class AIStarShipController;

enum AIStarShipState
{
      None = 0
    , Idle
    , MoveTo
};

template<class T>
class IdleState : public AI::State<T>
{
public:
    IdleState( void )
        : AI::State<T>( )
    {
    }
    virtual ~IdleState( void ) {}
    void update( sf::Time, CommandQueue&, T* owner );
    void onEnter( T* owner, void* data );
    void onExit( T* owner );
};

template<class T>
class MoveToState : public AI::State<T>
{
public:
    MoveToState( void )
        : AI::State<T>( )
        , mStarShip( nullptr )
        , mTargetPos( 0.0f, 0.0f )
        , mTargetAngle( 0.0f )
    { }
    void update( sf::Time, CommandQueue&, T* owner );
    void onEnter( T* owner, void* data );
    void onExit( T* owner );
private:
    //## MoveTo State Attributes
    StarShip* mStarShip;
    sf::Vector2f mTargetPos;
    float        mTargetAngle;
};

class AIStarShipController
{
public:
    /// 1-800-552-8159 insurance number you need to call ASAP!!!!!!!!!
    typedef PlayerAction::Type Action;
    AIStarShipController( unsigned int identifier = 1 );

    void updateCurrent( sf::Time dt, CommandQueue& commands );
    unsigned int getCategory( void ) const;

    void rotateLeft( void ) { mMoveLeftFlag = true; }
    void rotateRight( void ) { mMoveRightFlag = true; }
    void thrust( void ) { mThrustFlag = true; }
    void fire( void ) { mFireFlag = true; }

    AI::FiniteStateMachine<AIStarShipController> mFsm;
    unsigned int mIdentifier;
    bool mMoveLeftFlag;
    bool mMoveRightFlag;
    bool mThrustFlag;
    bool mFireFlag;
    AIStarShipState mNextState;

    Command mMoveLeftCommand;
    Command mMoveRightCommand;
    Command mThrustCommand;
    Command mFireCommand;
};
#include "AIStarShipController.inl"
#endif // AICONTROLLER_HPP
