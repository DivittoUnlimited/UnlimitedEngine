#include "KeyBinding.hpp"

#include <string>
#include <algorithm>


KeyBinding::KeyBinding( int ) // int controlPreconfiguration
: mKeyMap( )
{
    /*
     * Demo for when you want to add camera later
     *
     *
	// Set initial key bindings for player 1
    if( controlPreconfiguration == 1 )
	{
        mKeyMap[sf::Keyboard::Left]	 = PlayerAction::MoveUnit;
        mKeyMap[sf::Keyboard::Right] = PlayerAction::AttackUnit;
        mKeyMap[sf::Keyboard::Up]    = PlayerAction::AttackBuilding;
        mKeyMap[sf::Keyboard::Space] = PlayerAction::CaptureBuilding;
	}
    else if( controlPreconfiguration == 2 )
	{
		// Player 2
        mKeyMap[sf::Keyboard::A] = PlayerAction::MoveUnit;
        mKeyMap[sf::Keyboard::D] = PlayerAction::AttackUnit;
        mKeyMap[sf::Keyboard::W] = PlayerAction::AttackBuilding;
        mKeyMap[sf::Keyboard::LShift] = PlayerAction::CaptureBuilding;
	}
    */
}

void KeyBinding::assignKey( Action action, sf::Keyboard::Key key )
{
	// Remove all keys that already map to action
    for( auto itr = mKeyMap.begin( ); itr != mKeyMap.end(); )
	{
        if( itr->second == action )
            mKeyMap.erase( itr++ );
		else
			++itr;
	}

	// Insert new binding
	mKeyMap[key] = action;
}

sf::Keyboard::Key KeyBinding::getAssignedKey( Action action ) const
{
    for( auto pair : mKeyMap )
	{
		if (pair.second == action)
			return pair.first;
	}

	return sf::Keyboard::Unknown;
}

bool KeyBinding::checkAction(sf::Keyboard::Key key, Action& out) const
{
    auto found = mKeyMap.find( key );
    if( found == mKeyMap.end( ) )
	{
		return false;
	}
	else
	{
		out = found->second;
		return true;
	}
}

std::vector<KeyBinding::Action> KeyBinding::getRealtimeActions() const
{
	// Return all realtime actions that are currently active.
	std::vector<Action> actions;

    for( auto pair : mKeyMap  )
	{
		// If key is pressed and an action is a realtime action, store it
        if( sf::Keyboard::isKeyPressed( pair.first ) && isRealtimeAction( pair.second ) )
            actions.push_back( pair.second );
	}

	return actions;
}

bool isRealtimeAction( PlayerAction::Type )
{
    /*
	switch (action)
	{
		case PlayerAction::MoveLeft:
		case PlayerAction::MoveRight:
		case PlayerAction::MoveUp:
		case PlayerAction::Fire:
			return true;

		default:
			return false;
	}
    */
    return false; // this game has no realtime actions yet
}
