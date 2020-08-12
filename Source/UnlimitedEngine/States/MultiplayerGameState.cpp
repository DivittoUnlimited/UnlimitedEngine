#include "MultiplayerGameState.hpp"

#include "GameState.hpp"
#include "Core/MusicPlayer.hpp"
#include "Core/Globals.hpp"
#include "Core/Utility.hpp"
#include "Core/GameServer.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Network/IpAddress.hpp>

#include <stdio.h>
#include <fstream>

sf::IpAddress getAddressFromFile( void )
{
    { // Try to open existing file (RAII block)
        std::ifstream inputFile( "Core/ip.txt" );
        std::string ipAddress;
        if (inputFile >> ipAddress)
            return ipAddress;
    }

    // If open/read failed, create new file
    std::ofstream outputFile( "Core/ip.txt" );
    std::string localAddress = "127.0.0.1";
    outputFile << localAddress;
    return localAddress;
}

MultiplayerGameState::MultiplayerGameState( States::ID id, StateStack& stack, Context context, bool isHost = false )
    : State( id, stack, context )
    , mWorld( &context, &stack, *context.window, *context.fonts, *context.sounds, true, false )
    , mTextureManager( *context.textures )
    , mConnected( false )
    , mGameServer( nullptr )
    , mActiveState( true )
    , mHasFocus( true )
    , mHost( isHost )
    , mGameStarted( false )
    , mClientTimeout( sf::seconds( 10.f ) )
    , mTimeSinceLastPacket( sf::seconds( 0.f ) )
{
    mBroadcastText.setFont( context.fonts->get( FontMap.at( "Default" ) ) );
    mBroadcastText.setPosition( 1024.f / 2, 100.f );

    mPlayerInvitationText.setFont( context.fonts->get( 0 ) );
    mPlayerInvitationText.setCharacterSize( 15 );
    mPlayerInvitationText.setFillColor( sf::Color::White );
    mPlayerInvitationText.setString( "Press J to spawn player 2" );
    mPlayerInvitationText.setPosition( 1000 - mPlayerInvitationText.getLocalBounds( ).width, 760 - mPlayerInvitationText.getLocalBounds( ).height );

    // We reuse this text for "Attempt to connect" and "Failed to connect" messages
    mFailedConnectionText.setFont( context.fonts->get( 0 ) );
    mFailedConnectionText.setString( "Attempting to connect..." );
    mFailedConnectionText.setCharacterSize( 35 );
    mFailedConnectionText.setFillColor( sf::Color::White );
    centerOrigin( mFailedConnectionText );
    mFailedConnectionText.setPosition( mWindow->getSize( ).x / 2.f, mWindow->getSize( ).y / 2.f );

    // Render a "establishing connection" frame for user feedback
    mWindow->clear( sf::Color::Black );
    mWindow->draw( mFailedConnectionText );
    mWindow->display( );
    mFailedConnectionText.setString( "Could not connect to the remote server!" );
    centerOrigin( mFailedConnectionText );

    sf::IpAddress ip;
    if( isHost )
    {
        mGameServer.reset( new GameServer( sf::Vector2f( mWindow->getSize( ) ) ) );
        ip = "127.0.0.1";
    }
    else
    {
        mGameServer.reset( new GameServer( sf::Vector2f( mWindow->getSize( ) ) ) );
        ip = getAddressFromFile( );
    }

    if( mSocket.connect( ip, ServerPort, sf::seconds( 5.f ) ) == sf::TcpSocket::Done )
    {
        std::cout << "Attempt to connect was successfull!" << std::endl;
        mConnected = true;
    }
    else
    {
        std::cout << "Attempt to connect was unsuccessfull" << std::endl;
        mFailedConnectionClock.restart( );
    }

    mSocket.setBlocking( false );
}

MultiplayerGameState::~MultiplayerGameState( )
{
}

void MultiplayerGameState::draw()
{
    if( mConnected )
    {
        mWorld.draw( );

        // Broadcast messages in default view
        mWindow->setView( mWindow->getDefaultView( ) );

        if( !mBroadcasts.empty( ) )
            mWindow->draw( mBroadcastText );

        if( mLocalPlayerIdentifiers.size() < 2 && mPlayerInvitationTime < sf::seconds( 0.5f ) )
            mWindow->draw( mPlayerInvitationText );
    }
    else
    {
        mWindow->draw( mFailedConnectionText );
    }
}

void MultiplayerGameState::onDestroy( void )
{
    if( !mHost && mConnected )
    {
        // Inform server this client is dying
        sf::Packet packet;
        packet << static_cast<sf::Int32>( Client::Quit );
        mSocket.send( packet );
    }
}

bool MultiplayerGameState::update( sf::Time dt )
{
    if( mConnected )
    {
        mWorld.update( dt );

        /*
        if( mActiveState && mHasFocus )
        {
            CommandQueue& commands = mWorld.getCommandQueue( );
            for( auto& pair : mPlayers )
                pair.second->handleRealtimeInput( commands );
        }

        // Always handle the network input
        CommandQueue& commands = mWorld.getCommandQueue( );
        for( auto& pair : mPlayers )
            pair.second->handleRealtimeNetworkInput( commands );
        */

        // Handle messages from server that may have arrived
        sf::Packet packet;
        if( mSocket.receive( packet ) == sf::Socket::Done )
        {
            mTimeSinceLastPacket = sf::seconds( 0.f );
            sf::Int32 packetType;
            packet >> packetType;
            handlePacket( packetType, packet );
        }

        updateBroadcastMessage( dt );

        // Time counter for blinking 2nd player text
        mPlayerInvitationTime += dt;
        if( mPlayerInvitationTime > sf::seconds( 2.f ) )
            mPlayerInvitationTime = sf::Time::Zero;

        // Events occurring in the game
        GameActions::Action gameAction;
        while( mWorld.pollGameAction( gameAction ) )
        {
            if( gameAction.type == GameActions::SpawnUnit )
            {
                sf::Packet packet;
                packet << static_cast<sf::Int32>( Client::SpawnUnit );
                //Unit* unit = mWorld.mCurrentUnits.front();
                //packet << unit->mUnitType;
                packet << gameAction.position.x;
                packet << gameAction.position.y;
                mSocket.send( packet );
            }
        }
        mTimeSinceLastPacket += dt;
    }
    // Failed to connect and waited for more than 5 seconds: Back to menu
    else if( mFailedConnectionClock.getElapsedTime( ) >= sf::seconds( 2.f ) )
    {
        requestStateClear( );
        requestStackPush( States::Menu );
    }
    return true;
}

void MultiplayerGameState::disableAllRealtimeActions( void )
{
    mActiveState = false;
    mPlayer->disableAllRealtimeActions();
    //for( sf::Int32 identifier : mLocalPlayerIdentifiers )
    //    mPlayers[identifier]->disableAllRealtimeActions( );
}

bool MultiplayerGameState::handleEvent( const sf::Event& event )
{
    // Game input handling
    CommandQueue& commands = mWorld.getCommandQueue( );

    // Forward event to all players ----
    if( event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left )
    {
        /* OLd WAY
        // create a new event with delta mouse pos
        sf::Event newEvent;
        newEvent.type = sf::Event::MouseButtonReleased;
        newEvent.mouseButton.button = sf::Mouse::Left;
        newEvent.mouseButton.x = event.mouseButton.x + mWorld.mDeltaMousePosition.x;
        newEvent.mouseButton.y = event.mouseButton.y + mWorld.mDeltaMousePosition.y;

        mPlayer->handleEvent( newEvent, commands );
        */
        mPlayer->handleEvent( event, mWorld.getCommandQueue( ) );
    }
    else
        mPlayer->handleEvent( event, commands );

    mWorld.handleEvent( event );

    if( event.type == sf::Event::KeyReleased )
    {
        // Escape pressed, trigger the pause screen
        if( event.key.code == sf::Keyboard::Escape )
        {
            disableAllRealtimeActions( );
            requestStackPush( States::NetworkPause );
        }
    }
    else if( event.type == sf::Event::GainedFocus )
    {
        mHasFocus = true;
    }
    else if( event.type == sf::Event::LostFocus )
    {
        mHasFocus = false;
    }

    return true;
}

void MultiplayerGameState::onActivate( void )
{
    mActiveState = true;
}

void MultiplayerGameState::updateBroadcastMessage( sf::Time elapsedTime )
{
    if( mBroadcasts.empty( ) )
        return;

    // Update broadcast timer
    mBroadcastElapsedTime += elapsedTime;
    if( mBroadcastElapsedTime > sf::seconds( 2.5f ) )
    {
        // If message has expired, remove it
        mBroadcasts.erase( mBroadcasts.begin( ) );

        // Continue to display next broadcast message
        if( !mBroadcasts.empty( ) )
        {
            mBroadcastText.setString( mBroadcasts.front( ) );
            centerOrigin( mBroadcastText );
            mBroadcastElapsedTime = sf::Time::Zero;
        }
    }
}

void MultiplayerGameState::handlePacket( sf::Int32 packetType, sf::Packet& packet )
{
    switch( packetType )
    {
        case Server::SpawnSelf:
        {
            sf::Int32 identifier;
            sf::Vector2f pos;
            sf::Int32 teamColor;
            packet >> identifier >> pos.x >> pos.y >> teamColor;
            mPlayer.reset( new Player( &mWorld, &mSocket, identifier, getContext().keys1 ) );
            mLocalPlayerIdentifiers.push_back( identifier );
            mGameStarted = true;
        }break;
        // Send message to all clients
        case Server::BroadcastMessage:
        {
            std::string message;
            packet >> message;
            mBroadcasts.push_back( message );

            // Just added first message, display immediately
            if( mBroadcasts.size( ) == 1 )
            {
                mBroadcastText.setString( mBroadcasts.front( ) );
                centerOrigin( mBroadcastText );
                mBroadcastElapsedTime = sf::Time::Zero;
            }
        } break;
        case Server::PlayerConnect: { } break;
        case Server::PlayerDisconnect:
        {
            sf::Int32 identifier;
            packet >> identifier;

            //mWorld.removeStarShip( identifier );
            //mPlayers.erase( identifier );
        } break;
        // Player event (like missile fired) occurs
        case Server::PlayerEvent:
        {
            sf::Int32 identifier;
            sf::Int32 action;
            sf::Vector2i pos;
            packet >> identifier >> action >> pos.x >> pos.y;
            mPlayer->handleNetworkEvent( static_cast<PlayerAction::Type>( action ), mWorld.getCommandQueue( ), pos );
        } break;
        case Server::LeftClick:
        {          
            sf::Int32 identifier;
            sf::Vector2i pos;
            packet >> identifier >> pos.x >> pos.y;
            mPlayer->handleNetworkEvent( PlayerAction::LeftClick, mWorld.getCommandQueue( ), pos );
        } break;
        case Server::SpawnUnit:
        {
            unsigned int unitType;
            float x;
            float y;
            packet >> unitType;
            packet >> x;
            packet >> y;
            // mWorld.spawnUnit( unitType, sf::Vector2i( x, y ) );
        } break;
        // Mission successfully completed
        case Server::MissionSuccess:
        {
            std::cout << "MultiplayerGameState Server::MissionSuccess out of date!" << std::endl;
            // requestStackPush(States::MissionSuccess);
        } break;
    }
}


