#ifndef FINITESTATEMACHINE_HPP
#define FINITESTATEMACHINE_HPP

#include <SFML/System/Time.hpp>

#include "Core/CommandQueue.hpp"
namespace AI {

template<class T>
struct State {
    virtual void update( sf::Time dt, CommandQueue& commands, T* owner ) = 0;
    virtual void onEnter( T* owner ) = 0;
    virtual void onExit( T* owner ) = 0;
};

template<class T>
class FiniteStateMachine
{
public:
    FiniteStateMachine( T* owner, State<T>* startingState );

    void update( sf::Time dt, CommandQueue& commands );
    void changeState( State<T>* state );
    void  revertState( void );

    T* getOwner( void ) { return mOwner; }
    State<T>* getCurrentState( void ) { return mCurrentState; }

private:
    T*        mOwner;
    State<T>* mCurrentState;
    State<T>* mPrevState;
};
}
#include "Core/FiniteStateMachine.inl"

#endif // FINITESTATEMACHINE_HPP
