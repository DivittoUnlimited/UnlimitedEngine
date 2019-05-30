#ifndef AICONTROLLER_HPP
#define AICONTROLLER_HPP

#include <SFML/System/Time.hpp>

#include "Core/SceneNode.hpp"
#include "Core/CommandQueue.hpp"
#include "Core/FiniteStateMachine.hpp"
#include "Core/Globals.hpp"
#include "Core/KeyBinding.hpp"

class AIStarShipController;

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
    {
    }

    void update( sf::Time, CommandQueue&, T* owner );

    void onEnter( T* owner, void* data );
    void onExit( T* owner );
private:
    //## MoveTo State Attributes
    sf::Vector2f mTargetPos;
    std::map<sf::Time, Command> mCommandList;
};

class AIStarShipController
{
public:
    /// 1-800-552-8159 insurance number you need to call ASAP!!!!!!!!!
    typedef PlayerAction::Type Action;
    AIStarShipController( unsigned int identifier );

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
    std::map<PlayerAction::Type, Command>	mActionBinding;
    Command mMoveLeftCommand;
    Command mMoveRightCommand;
    Command mThrustCommand;
    Command mFireCommand;
};
#include "AIStarShipController.inl"
#endif // AICONTROLLER_HPP
