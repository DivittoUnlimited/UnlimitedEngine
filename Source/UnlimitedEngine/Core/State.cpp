#include "State.hpp"
#include "StateStack.hpp"




State::State( States::ID stateID, StateStack& stack, Context context )
: mStack(&stack)
, mContext(context)
, mStateID( stateID )
{
}

State::~State()
{
}

void State::requestStackPush(States::ID stateID)
{
	mStack->pushState(stateID);
}

void State::requestStackPop()
{
	mStack->popState();
}

void State::requestStateClear()
{
	mStack->clearStates();
}

States::ID State::getStateID() const
{
    return mStateID;
}

State::Context State::getContext() const
{
	return mContext;
}

