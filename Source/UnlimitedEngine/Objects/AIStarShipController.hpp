#ifndef AICONTROLLER_HPP
#define AICONTROLLER_HPP

#include <SFML/System/Time.hpp>

#include "Core/SceneNode.hpp"
#include "Core/CommandQueue.hpp"
#include "Core/FiniteStateMachine.hpp"
#include "Core/Globals.hpp"
#include "Core/KeyBinding.hpp"

template<class T>
class IdleState : public AI::State<T>
{
public:
    IdleState( void )
        : AI::State<T>( )
    {
    }

    void update( sf::Time, CommandQueue&, T* owner )
    {
        owner->rotateLeft();
    }

    void onEnter( T* )
    {
        std::cout << "IdleState onEnter( ) entered." << std::endl;
    }

    void onExit( T* )
    {
        std::cout << "IdleState onExit( ) entered" << std::endl;
    }
};

class AIStarShipController
{
public:
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

#endif // AICONTROLLER_HPP
