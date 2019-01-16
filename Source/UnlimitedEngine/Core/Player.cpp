#include "Player.hpp"
#include "CommandQueue.hpp"


#include <map>
#include <string>
#include <algorithm>
#include "Objects/Actor.hpp"

//static const unsigned int maxSpeed = 10;


///
/// \brief The ActorMover struct
/// Functor to move the player based on velocity and the delta time for this frame.
struct ActorMover
{
    ActorMover( float vx, float vy )
    : velocity( vx, vy )
	{
	}
    void operator( )( Actor& actor, sf::Time ) const
    {
        actor.accelerate( velocity * actor.speed( ) );
	}
	sf::Vector2f velocity;
};

Player::Player( )
{
    // Set initial key bindings
    mKeyBinding[sf::Keyboard::Left]     = MoveLeft;
    mKeyBinding[sf::Keyboard::Right]    = MoveRight;
    mKeyBinding[sf::Keyboard::Up]       = MoveUp;
    mKeyBinding[sf::Keyboard::Down]     = MoveDown;
    //mKeyBinding[sf::Keyboard::Space]    = Fire;
    //mKeyBinding[sf::Keyboard::M]        = LaunchMissile;
    //mKeyBinding[sf::Keyboard::LShift]   = TurnAround;

    // Set initial action bindings
    initializeActions( );

    // Assign all categories to player's aircraft

    for( auto& pair : mActionBinding )
        pair.second.category = Category::Player;
}

void Player::handleEvent( const sf::Event& event, CommandQueue& commands )
{  
    if( event.joystickMove.axis == sf::Joystick::Y && event.joystickMove.position > 0 )
        commands.push( mActionBinding[MoveUp] );
    else if( event.joystickMove.axis == sf::Joystick::Y && event.joystickMove.position < 0 )
        commands.push( mActionBinding[MoveDown] );
    if( event.joystickMove.axis == sf::Joystick::X && event.joystickMove.position > 0 )
        commands.push( mActionBinding[MoveRight] );
    else if( event.joystickMove.axis == sf::Joystick::X && event.joystickMove.position < 0 )
        commands.push( mActionBinding[MoveLeft] );

    if( event.type == sf::Event::JoystickButtonReleased )
    {
        switch( event.joystickButton.button )
        {
        case 0:
            //commands.push( mActionBinding[LaunchMissile] );
            // std::cout << "Coin button (Y) activated" << std::endl;
            break;
        case 1:
            // commands.push( mActionBinding[TurnAround] );
            //std::cout << "button 1(A) activated" << std::endl;
            break;
        case 2:
            // commands.push( mActionBinding[Fire] );
             //std::cout << "button 2(B) activated" << std::endl;
            break;
        case 3:
            //commands.push( mActionBinding[Fire] );
           // std::cout << "button 3(X) activated" << std::endl;
            break;
        case 4:
            //commands.push( mActionBinding[TurnAround] );
            //std::cout << "button 4 (LeftTrigger) activated" << std::endl;
            break;
        case 5:
            //commands.push( mActionBinding[LaunchMissile] );
            //std::cout << "button 5 (RightTrigger) activated" << std::endl;
            break;
        case 6:
            //commands.push( mActionBinding[Fire] );
            //std::cout << "button 6 activated" << std::endl;
            break;
        case 7:
            //commands.push( mActionBinding[Fire] );
            //std::cout << "button 7 activated" << std::endl;
        break;
        case 8:
            //commands.push( mActionBinding[Fire] );
            // std::cout << "button 8 activated(SELECT BUTTON pause game)" << std::endl;
        break;
        case 9:
            std::cout << "button 9 activated(START BUTTON pause game!!)" << std::endl;
        break;
        }
    }
    else if( event.type == sf::Event::KeyPressed )
    {
        // Check if pressed key appears in key binding, trigger command if so
        auto found = mKeyBinding.find( event.key.code );
        if( found != mKeyBinding.end( ) && !isRealtimeAction( found->second ) )
            commands.push( mActionBinding[found->second] );
    }
}

void Player::handleRealtimeInput( CommandQueue& commands )
{
    // Traverse all assigned keys and check if they are pressed
    for( auto pair : mKeyBinding )
    {
        // If key is pressed, lookup action and trigger corresponding command
        if( sf::Keyboard::isKeyPressed( pair.first ) && isRealtimeAction( pair.second ) )
            commands.push( mActionBinding[pair.second] );
    }    
}

void Player::assignKey( Action action, sf::Keyboard::Key key )
{
    // Remove all keys that already map to action
    for( auto itr = mKeyBinding.begin( ); itr != mKeyBinding.end( ); )
    {
        if( itr->second == action )
            mKeyBinding.erase( itr++ );
        else
            ++itr;
    }
    // Insert new binding
    mKeyBinding[key] = action;
}

sf::Keyboard::Key Player::getAssignedKey( Action action ) const
{
    for( auto pair : mKeyBinding )
    {
        if( pair.second == action )
            return pair.first;
    }
    return sf::Keyboard::Unknown;
}

void Player::initializeActions( )
{
    mActionBinding[MoveLeft].action      = derivedAction<Actor>( ActorMover( -1,  0 ) );
    mActionBinding[MoveRight].action     = derivedAction<Actor>( ActorMover( +1,  0 ) );
    mActionBinding[MoveUp].action        = derivedAction<Actor>( ActorMover(  0, +1 ) );
    mActionBinding[MoveDown].action      = derivedAction<Actor>( ActorMover(  0, -1 ) );
    // mActionBinding[Fire].action          = derivedAction<Aircraft>( [] ( Aircraft& a, sf::Time) { a.fire( ); } );
    // mActionBinding[LaunchMissile].action = derivedAction<Aircraft>( [] ( Aircraft& a, sf::Time ) { a.launchMissile( ); } );
    // mActionBinding[TurnAround].action    = derivedAction<Aircraft>( []( Aircraft&, sf::Time ) { if( FLIP_GAMEPLAY ) FLIP_GAMEPLAY = false; else FLIP_GAMEPLAY = true; } );
}

bool Player::isRealtimeAction( Action action )
{
    switch( action )
    {
        case MoveLeft:
        case MoveRight:
        case MoveDown:
        case MoveUp:
        //case Fire:
        //case TurnAround:
            return true;
        default:
            return false;
    }
}
