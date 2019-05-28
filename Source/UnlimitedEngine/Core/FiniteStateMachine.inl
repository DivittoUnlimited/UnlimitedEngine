#include "FiniteStateMachine.hpp"

template<class T>
AI::FiniteStateMachine<T>::FiniteStateMachine( T* owner, AI::State<T>* startingState )
    : mOwner( owner )
    , mCurrentState( startingState )
{
    mCurrentState->onEnter( mOwner );
}

template<class T>
void AI::FiniteStateMachine<T>::update( sf::Time dt, CommandQueue& commands )
{
    mCurrentState->update( dt, commands, mOwner );
}

template<class T>
void AI::FiniteStateMachine<T>::changeState( State<T>* state )
{
    assert( state );
    mCurrentState->onExit( mOwner );
    mPrevState = nullptr;
    mPrevState = mCurrentState;
    mCurrentState = state;
    mCurrentState->onEnter( mOwner );
}

template<class T>
void AI::FiniteStateMachine<T>::revertState( void )
{
    assert( mPrevState && mCurrentState );
    mCurrentState = nullptr;
    mCurrentState = mPrevState;
}
