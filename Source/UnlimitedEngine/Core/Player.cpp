

///
/// IF YOU CAN READ THIS YOU STILL NEED TO REVERT INPUT BACK TO ONLY AVATAR CONTROLLER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/// DO IT!

#include "Player.hpp"
#include "CommandQueue.hpp"


#include <map>
#include <string>
#include <algorithm>
#include "Objects/Actor.hpp"
#include "Gui/MessageBoxNode.hpp"


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
    : mCurrentScheme( )
    , mSchemes( )
{

    for( unsigned int i = 0; i < Player::InputMode::InputCount; ++i )
    {
        InputData data;

        // Set initial key bindings
        data.mKeyBinding[sf::Keyboard::Left]     = MoveLeft;
        data.mKeyBinding[sf::Keyboard::Right]    = MoveRight;
        data.mKeyBinding[sf::Keyboard::Up]       = MoveUp;
        data.mKeyBinding[sf::Keyboard::Down]     = MoveDown;
        data.mKeyBinding[sf::Keyboard::Space]    = ActionButton;
        //mKeyBinding[sf::Keyboard::M]        = LaunchMissile;
        //mKeyBinding[sf::Keyboard::LShift]   = TurnAround;

        // Set initial action bindings
        initializeActions( (InputMode)i, data );

        // Assign all categories to player's aircraft

        for( auto& pair : data.mActionBinding )
            pair.second.category = Category::Player;

        mSchemes.push_back( data );
    }
    mCurrentScheme = &mSchemes[InputMode::AvatarController];
    mSchemeName = InputMode::AvatarController;
}

void Player::handleEvent( const sf::Event& event, CommandQueue& commands )
{  
    if( event.joystickMove.axis == sf::Joystick::Y && event.joystickMove.position > 0 )
        commands.push( mCurrentScheme->mActionBinding[MoveUp] );
    else if( event.joystickMove.axis == sf::Joystick::Y && event.joystickMove.position < 0 )
        commands.push( mCurrentScheme->mActionBinding[MoveDown] );
    if( event.joystickMove.axis == sf::Joystick::X && event.joystickMove.position > 0 )
        commands.push( mCurrentScheme->mActionBinding[MoveRight] );
    else if( event.joystickMove.axis == sf::Joystick::X && event.joystickMove.position < 0 )
        commands.push( mCurrentScheme->mActionBinding[MoveLeft] );

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
             commands.push( mCurrentScheme->mActionBinding[ActionButton] );
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
        auto found = mCurrentScheme->mKeyBinding.find( event.key.code );
        if( found != mCurrentScheme->mKeyBinding.end( ) && !isRealtimeAction( found->second ) )
            commands.push(mCurrentScheme-> mActionBinding[found->second] );
    }
}

void Player::handleRealtimeInput( CommandQueue& commands )
{
    // Traverse all assigned keys and check if they are pressed
    for( auto pair : mCurrentScheme->mKeyBinding )
    {
        // If key is pressed, lookup action and trigger corresponding command
        if( sf::Keyboard::isKeyPressed( pair.first ) && isRealtimeAction( pair.second ) )
            commands.push( mCurrentScheme->mActionBinding[pair.second] );
    }    
}

void Player::assignKey( Action action, sf::Keyboard::Key key )
{
    // Remove all keys that already map to action
    for( auto itr = mCurrentScheme->mKeyBinding.begin( ); itr != mCurrentScheme->mKeyBinding.end( ); )
    {
        if( itr->second == action )
            mCurrentScheme->mKeyBinding.erase( itr++ );
        else
            ++itr;
    }
    // Insert new binding
    mCurrentScheme->mKeyBinding[key] = action;
}

sf::Keyboard::Key Player::getAssignedKey( Action action ) const
{
    for( auto pair : mCurrentScheme->mKeyBinding )
    {
        if( pair.second == action )
            return pair.first;
    }
    return sf::Keyboard::Unknown;
}

void Player::changeInputMode( InputMode input  )
{
    // Debug only remove whole switch
    this->mCurrentScheme = &mSchemes[input];
    this->mSchemeName = input;
    switch( input ) {
        case AvatarController: std::cout << "InputMode set to: AvatarController"   << std::endl; break;
        case MenuNavogator:    std::cout << "InputMode set to: MennuNavigator"     << std::endl; break;
        case MessageBoxInput:  std::cout << "InputMode set to: MessageBox"         << std::endl; break;
        default:               std::cout << "Invalid input mode has been selectd!" << std::endl; break;
    }
}

void Player::initializeActions( InputMode mode, InputData& data )
{
    switch( mode )
    {
        case AvatarController:
            data.mActionBinding[MoveLeft].action      = derivedAction<Actor>( ActorMover( -1,  0 ) );
            data.mActionBinding[MoveRight].action     = derivedAction<Actor>( ActorMover( +1,  0 ) );
            data.mActionBinding[MoveUp].action        = derivedAction<Actor>( ActorMover(  0, -1 ) );
            data.mActionBinding[MoveDown].action      = derivedAction<Actor>( ActorMover(  0, +1 ) );
            data.mActionBinding[ActionButton].action  = derivedAction<Actor>( [] ( Actor& a, sf::Time) { a.handleAction( ); } );
        break;
        case MenuNavogator:
            data.mActionBinding[ActionButton].action  = derivedAction<Actor>( [] ( Actor&, sf::Time) { std::cout << "MenuNavigator action button in use!" << std::endl; } );
        break;
        case MessageBoxInput:

        break;
        case InputCount:
        default:
        std::cout << "Invalid Input Scheme Attempted. Player::initializeActions( )" << std::endl;
    }
}

bool Player::isRealtimeAction( Action action )
{
    if( mSchemeName == InputMode::AvatarController )
        switch( action )
        {
            case MoveLeft:
            case MoveRight:
            case MoveDown:
            case MoveUp:
            case ActionButton:
                return true;
            default:
                return false;
        }
    return false;
}
