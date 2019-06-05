#include "FiniteStateMachine.hpp"

template<class T>
AI::FiniteStateMachine<T>::FiniteStateMachine( T* owner, AI::State<T>* startingState )
    : mOwner( owner )
    , mCurrentState( startingState )
    , mPrevState( nullptr )
{
    mCurrentState->onEnter( mOwner );
}

template<class T>
void AI::FiniteStateMachine<T>::update( sf::Time dt, CommandQueue& commands )
{
    mCurrentState->update( dt, commands, mOwner );
}

template<class T>
void AI::FiniteStateMachine<T>::changeState( State<T>* state, void* data )
{
    assert( state );
    mCurrentState->onExit( mOwner );
    mPrevState = nullptr;
    *mPrevState = *mCurrentState;
    mCurrentState = nullptr;
    mCurrentState = state;
    mCurrentState->onEnter( mOwner, data );
}

template<class T>
void AI::FiniteStateMachine<T>::enterBlipState( State<T>* state, void* data )
{
    assert( state );
    mPrevState = mCurrentState;
    mCurrentState = state;
    mCurrentState->onEnter( mOwner, data );
}

template<class T>
void AI::FiniteStateMachine<T>::revertState( void )
{
    assert( mPrevState && mCurrentState );
    mCurrentState = nullptr;
    mCurrentState = mPrevState;
}
