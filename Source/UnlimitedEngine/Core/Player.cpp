#include "Player.hpp"
#include "CommandQueue.hpp"

#include "NetworkProtocol.hpp"
#include "Core/Globals.hpp"

#include <SFML/Network/Packet.hpp>
#include <map>
#include <string>
#include <algorithm>
#include <math.h>


Player::Player( World* world, sf::TcpSocket* socket, sf::Int32 identifier, const KeyBinding* binding )
    : mKeyBinding( binding )
    , mSocket( socket )
    , mWorld( world )
    , mIdentifier( identifier )
    {
        // Set initial action bindings
        initializeActions( );

        // Assign all categories to player's aircraft
        if( mIdentifier == Category::Player )
            for( auto& pair : mActionBinding )
                pair.second.category = Category::Player;
        else if( mIdentifier == Category::Player2 ) {
            for( auto& pair : mActionBinding )
                pair.second.category = Category::Player2;
        }

    }

void Player::handleEvent( const sf::Event& event, CommandQueue& commands )
{
    if( event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left )
    {
        // Network connected -> send event over network
        if( mSocket )
        {
            sf::Packet packet;
            float x = event.mouseButton.x;
            float y = event.mouseButton.y;
            //float x = sf::Mouse::getPosition(*mWindow).x;
            //float y = sf::Mouse::getPosition(*mWindow).y;
            packet << static_cast<sf::Int32>( Client::LeftClick );
            packet << mIdentifier;
            packet << x;
            packet << y;
            mSocket->send( packet );
        }
        // Network disconnected -> local event
        else
        {
            Command com;
            com.category = Category::Grid;
            mWorld->handleLeftClick( sf::Vector2i( event.mouseButton.x, event.mouseButton.y ) );
            commands.push( com );
        }
    }
    /*
    else if( event.type == sf::Event::KeyPressed )
    {
        PlayerAction::Type action;
        if( mKeyBinding && mKeyBinding->checkAction( event.key.code, action ) && !isRealtimeAction( action ) )
        {
            // Network connected -> send event over network
            if( mSocket )
            {
                sf::Packet packet;
                packet << static_cast<sf::Int32>( Client::PlayerEvent );
                packet << mIdentifier;
                packet << static_cast<sf::Int32>( action );
                mSocket->send( packet );
            }

            // Network disconnected -> local event
            else
            {
                commands.push( mActionBinding[action] );
            }
        }
        // Realtime change (network connected)
        if( ( event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased ) && mSocket )
        {
            PlayerAction::Type action;
            if( mKeyBinding && mKeyBinding->checkAction( event.key.code, action ) && isRealtimeAction( action ) )
            {
                // Send realtime change over network
                sf::Packet packet;
                packet << static_cast<sf::Int32>(Client::PlayerRealtimeChange);
                packet << mIdentifier;
                packet << static_cast<sf::Int32>(action);
                packet << (event.type == sf::Event::KeyPressed);
                mSocket->send(packet);
            }
        }
    }
    */
}

bool Player::isLocal( void ) const
{
    // No key binding means this player is remote
    return mKeyBinding != nullptr;
}

void Player::disableAllRealtimeActions( void )
{
    for( auto& action : mActionProxies )
    {
        sf::Packet packet;
        packet << static_cast<sf::Int32>(Client::PlayerRealtimeChange);
        packet << mIdentifier;
        packet << static_cast<sf::Int32>(action.first);
        packet << false;
        mSocket->send(packet);
    }
}

void Player::handleRealtimeInput( CommandQueue& commands )
{
    // Check if this is a networked game and local player or just a single player game
    if( ( mSocket && isLocal( ) ) || !mSocket )
    {
        //std::cout << "ERROR MESSAGE FROM PLAYER CLASS HANDLEREALTIMEINPUT" << std::endl;
        //for( unsigned int i=0; i< sf::Joystick::Count; ++i )
        //{
            //if(sf::Joystick::isConnected(i))
            //{
                 //std::cout << "Joystick " << i << " is connected!" << std::endl;
                 //std::cout << sf::Joystick::getIdentification( i ).name.toAnsiString() << ", " << sf::Joystick::getIdentification( i ).productId << std::endl;
            //}
            // else
                 // std::cout << "Joystick " << i << " is NOT connected!" << std::endl;
         //}

        /*
         *
         *  Demo saved for when you want to add camera control
        if( this->mIdentifier == Category::Player )
        {
            if( sf::Joystick::getAxisPosition( 0, sf::Joystick::Y ) < 0 ) // move forward
                commands.push( mActionBinding[PlayerAction::MoveUnit] );
            else if( sf::Joystick::getAxisPosition( 0, sf::Joystick::X ) > 0 ) // rotate right
                commands.push( mActionBinding[PlayerAction::AttackUnit] );
            else if( sf::Joystick::getAxisPosition( 0, sf::Joystick::X ) < 0 )
                commands.push( mActionBinding[PlayerAction::AttackBuilding] );
            else if( sf::Joystick::isButtonPressed( 0, 1 ) )
                commands.push( mActionBinding[PlayerAction::CaptureBuilding] );
        }
        else if( this->mIdentifier == Category::Player2 )
        {
            if( sf::Joystick::getAxisPosition( 1, sf::Joystick::Y ) < 0 ) // move forward
                commands.push( mActionBinding[PlayerAction::MoveUnit] );
            else if( sf::Joystick::getAxisPosition( 1, sf::Joystick::X ) > 0 ) // rotate right
                commands.push( mActionBinding[PlayerAction::AttackUnit] );
            else if( sf::Joystick::getAxisPosition( 1, sf::Joystick::X ) < 0 )
                commands.push( mActionBinding[PlayerAction::AttackBuilding] );
            else if( sf::Joystick::isButtonPressed( 1, 1 ) )
                commands.push( mActionBinding[PlayerAction::CaptureBuilding] );
        }
*/
        // Lookup all actions and push corresponding commands to queue
        std::vector<PlayerAction::Type> activeActions = mKeyBinding->getRealtimeActions( );
        for( PlayerAction::Type action : activeActions )
            commands.push( mActionBinding[action] );
   }
}

void Player::handleRealtimeNetworkInput(CommandQueue& commands)
{
    if ( mSocket && !isLocal( ) )
    {
       // Traverse all realtime input proxies. Because this is a networked game, the input isn't handled directly
        for( auto pair : mActionProxies )
        {
            if( pair.second && isRealtimeAction( pair.first ) )
                commands.push( mActionBinding[pair.first] );
        }
    }
}

void Player::handleNetworkEvent(PlayerAction::Type action, CommandQueue& commands, sf::Vector2i pos )
{
    if( action == PlayerAction::LeftClick )
    {
        Command com;
        com.category = Category::Grid;
        std::cout << "Player class handleNetworkEvent incomplete!" << std::endl;
        //com.action = derivedAction<World>( [pos] ( World& g, sf::Time ){ g.handleLeftClick( pos ); } );
        commands.push( com );
    }
}

void Player::handleNetworkRealtimeChange(PlayerAction::Type action, bool actionEnabled ) { mActionProxies[action] = actionEnabled; }
void Player::initializeActions( ) { }
