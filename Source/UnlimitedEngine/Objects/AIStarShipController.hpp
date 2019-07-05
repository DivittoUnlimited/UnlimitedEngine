#ifndef AICONTROLLER_HPP
#define AICONTROLLER_HPP

#include <SFML/System/Time.hpp>

#include "Core/SceneNode.hpp"
#include "Core/CommandQueue.hpp"
#include "Core/FiniteStateMachine.hpp"
#include "Core/Globals.hpp"
#include "Core/KeyBinding.hpp"
#include "Core/Entity.hpp"
#include "Core/SteeringBehaviors.hpp"

class StarShip;
class AIStarShipController;

enum AIStarShipState
{
      None = 0
    , Idle
    , ShootTarget
    , CaptureFlag
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
class ShootTargetState : public AI::State<T>
{
public:
    ShootTargetState( void )
        : AI::State<T>( )
        , mStarShip( nullptr )
        , mDistanceToTarget( 1000.0f )
        , mRange( 200.0f )
    { }
    void update( sf::Time, CommandQueue&, T* owner );
    void onEnter( T* owner, void* data );
    void onExit( T* owner );
private:
    //## MoveTo State Attributes
    StarShip* mStarShip;
    float     mDistanceToTarget;
    float     mRange;
};

template<class T>
class CaptureFlagState : public AI::State<T>
{
public:
    CaptureFlagState( void )
        : AI::State<T>( )
        , mStarShip( nullptr )
        , mAlreadyHadFlag( false )
    { }
    void update( sf::Time, CommandQueue&, T* owner );
    void onEnter( T* owner, void* data );
    void onExit( T* owner );
private:
    StarShip* mStarShip;
    bool mAlreadyHadFlag; // prevents changing target to goal every frame after flag is in hand
};

class AIStarShipController
{
public:
    typedef PlayerAction::Type Action;
    AIStarShipController( unsigned int identifier = 1 );

    void updateCurrent( sf::Time dt, CommandQueue& commands );
    unsigned int getCategory( void ) const;

    void rotateLeft( void ) { mMoveLeftFlag = true; }
    void rotateRight( void ) { mMoveRightFlag = true; }
    void thrust( void ) { mThrustFlag = true; }
    void fire( void ) { mFireFlag = true; }
    unsigned int getNearestFlag( void );

    AI::FiniteStateMachine<AIStarShipController> mFsm;
    unsigned int mIdentifier;
    bool mMoveLeftFlag;
    bool mMoveRightFlag;
    bool mThrustFlag;
    bool mFireFlag;
    AIStarShipState mNextState;
    AIStarShipState mNextBlipState;
    StarShip* mStarShip;

    Command mMoveLeftCommand;
    Command mMoveRightCommand;
    Command mThrustCommand;
    Command mFireCommand;

    Entity* mTargetObject;

};
#include "AIStarShipController.inl"
#endif // AICONTROLLER_HPP
