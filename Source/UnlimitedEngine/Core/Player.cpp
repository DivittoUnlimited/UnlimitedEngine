#include "Player.hpp"
#include "CommandQueue.hpp"

#include <map>
#include <string>
#include <algorithm>
#include <math.h>

#include "Objects/StarShip.hpp"

///
/// \brief The ActorMover struct
/// Functor to move the player based on velocity and the delta time for this frame.
struct StarShipMover
{
    StarShipMover( float vx, float vy )
    : velocity( vx, vy )
    {
    }
    void operator( )( StarShip& starShip, sf::Time ) const
    {
        starShip.accelerate( velocity * starShip.speed( ) );
    }
    sf::Vector2f velocity;
};

Player::Player( void )
{
    // Set initial key bindings
    mKeyBinding[sf::Keyboard::Left]     = MoveLeft;
    mKeyBinding[sf::Keyboard::Right]    = MoveRight;
    mKeyBinding[sf::Keyboard::Up]       = MoveUp;
    mKeyBinding[sf::Keyboard::Down]     = MoveDown;
    //mKeyBinding[sf::Keyboard::Space]    = Fire;

    // Set initial action bindings
    initializeActions( );

    // Assign all categories to player's aircraft

    for( auto& pair : mActionBinding )
        pair.second.category = Category::Player;
}

void Player::handleEvent( const sf::Event& event, CommandQueue& commands )
{
    if( event.joystickMove.axis == sf::Joystick::Y && event.joystickMove.position > 0 )
        commands.push( mActionBinding[MoveDown] );
    else if( event.joystickMove.axis == sf::Joystick::Y && event.joystickMove.position < 0 )
        commands.push( mActionBinding[MoveUp] );

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
        {
            commands.push( mActionBinding[pair.second] );
        }
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
    mActionBinding[MoveLeft].action      = derivedAction<StarShip>( [] ( StarShip& a, sf::Time ){ a.rotate( -1 * a.speed( ) ); } );
    mActionBinding[MoveRight].action     = derivedAction<StarShip>( [] ( StarShip& a, sf::Time ){ a.rotate( a.speed( ) ); } );

    mActionBinding[MoveUp].action        = derivedAction<StarShip>( [] ( StarShip& a, sf::Time ){
            // break down angle to x and y
            double angle = a.getRotation() * (3.14 / 180);
            sf::Vector2f force;
            force.x = sin( angle );
            force.y = -cos( angle );
            // apply speed to components seperatly
            force *= a.speed( );
            // accellerate the ship with the results
            a.accelerate( force );

            // Validate ship isnt going to fast and correct if needed.
            float totalVelocity = (float)std::sqrt( (a.getVelocity( ).x*a.getVelocity( ).x) + (a.getVelocity( ).y*a.getVelocity( ).y) );
            if( totalVelocity > a.maximumVelocity( ) )
                a.setVelocity( a.getVelocity( ) * a.maximumVelocity( ) / totalVelocity );
    } );
/*
    mActionBinding[MoveDown].action      = derivedAction<StarShip>( [] ( StarShip& a, sf::Time ){
            // break down angle to x and y
            double angle = a.getRotation() * (3.14 / 180);
            sf::Vector2f force;
            force.x = sin( angle );
            force.y = -cos( angle );
            // apply speed to components seperatly
            force *= -a.speed( );
            // accellerate the ship with the results
            a.accelerate( force );

            // Validate ship isnt going to fast and correct if needed.
            float totalVelocity = (float)std::sqrt( (a.getVelocity( ).x*a.getVelocity( ).x) + (a.getVelocity( ).y*a.getVelocity( ).y) );
            if( totalVelocity > a.maximumVelocity( ) )
                a.setVelocity( a.getVelocity( ) * a.maximumVelocity( ) / totalVelocity );
    } );
*/
}

bool Player::isRealtimeAction( Action action )
{
    switch( action )
    {
        case MoveLeft:
        case MoveRight:
        case MoveDown:
        case MoveUp:
            return true;
        default:
            return false;
    }
}
