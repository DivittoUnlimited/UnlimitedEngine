  #include "FiniteStateMachine.hpp"

template<class T>
AI::FiniteStateMachine<T>::FiniteStateMachine( T* owner, AI::State<T>* startingState )
    : mOwner( owner )
    , mCurrentState( startingState )
    , mPrevState( nullptr )
    , mBlipStates( std::stack<State<T>*>( ) )
{
    mCurrentState->onEnter( mOwner );
}

template<class T>
void AI::FiniteStateMachine<T>::update( sf::Time dt, CommandQueue& commands )
{
    mCurrentState->update( dt, commands, mOwner );
    std::stack<State<T>*> tempHolder;
    unsigned int size = mBlipStates.size();
    for( unsigned int i = 0; i < size; ++i )
    {
        mBlipStates.top( )->update( dt, commands, mOwner );
        tempHolder.push( mBlipStates.top( ) );
        mBlipStates.pop( );
    }
    for( unsigned int i =0; i < size; i++ )
    {
        mBlipStates.push( tempHolder.top( ) );
        tempHolder.pop( );
    }
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
    mBlipStates.push( state );
    mBlipStates.top( )->onEnter( mOwner, data );
}

template<class T>
void AI::FiniteStateMachine<T>::exitBlipState( void )
{
    if( mBlipStates.size() > 0 )
    {
        mBlipStates.top->onExit( mOwner );
        mBlipStates.pop( );
    }
}


template<class T>
void AI::FiniteStateMachine<T>::revertState( void )
{
    assert( mPrevState && mCurrentState );
    mCurrentState = nullptr;
    mCurrentState = mPrevState;
}
