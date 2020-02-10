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
        std::ifstream inputFile("Core/ip.txt");
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
    , mWorld( &context, &stack, *context.window, *context.fonts, *context.sounds, LevelMap.at( "DemoMap" ), true, true )
    , mTextureManager( *context.textures )
    , mConnected( false )
    , mGameServer( nullptr )
    , mActiveState( true )
    , mHasFocus( true )
    , mHost( isHost )
    , mGameStarted( false )
    , mClientTimeout( sf::seconds( 2.f ) )
    , mTimeSinceLastPacket( sf::seconds( 0.f ) )
{
    mBroadcastText.setFont( context.fonts->get( 0 ) );
    mBroadcastText.setPosition( 1024.f / 2, 100.f );

    mPlayerInvitationText.setFont( context.fonts->get( 0 ) );
    mPlayerInvitationText.setCharacterSize( 15 );
    mPlayerInvitationText.setFillColor( sf::Color::White );
    mPlayerInvitationText.setString( "Press Enter to spawn player 2" );
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

bool MultiplayerGameState::update( sf::Time )
{
    /*
    // Connected to server: Handle all the network logic
    if( mConnected )
    {
        mWorld.update( dt );
        // Remove players whose aircrafts were destroyed
        bool foundLocalPlane = false;
        for( auto itr = mPlayers.begin( ); itr != mPlayers.end( ); )
        {
            // Check if there are no more local planes for remote clients
            if( std::find( mLocalPlayerIdentifiers.begin( ), mLocalPlayerIdentifiers.end( ), itr->first ) != mLocalPlayerIdentifiers.end( ) )
            {
                foundLocalPlane = true;
            }

            if ( !mWorld.getStarShip( itr->first ) )
            {
                itr = mPlayers.erase( itr );

                // No more players left: Mission failed
                if( mPlayers.empty( ) )
                    requestStackPush(States::GameOver);
            }
            else
            {
                ++itr;
            }
        }

        if( !foundLocalPlane && mGameStarted )
        {
            requestStackPush( States::GameOver );
        }
        // Only handle the realtime input if the window has focus and the game is unpaused
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

        // Handle messages from server that may have arrived
        sf::Packet packet;
        if( mSocket.receive( packet ) == sf::Socket::Done )
        {
            mTimeSinceLastPacket = sf::seconds( 0.f );
            sf::Int32 packetType;
            packet >> packetType;
            handlePacket( packetType, packet );
        }
        else
        {
            // Check for timeout with the server
            if( mTimeSinceLastPacket > mClientTimeout )
            {
                mConnected = false;

                mFailedConnectionText.setString( "Lost connection to server" );
                centerOrigin( mFailedConnectionText );

                mFailedConnectionClock.restart( );
            }
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
            sf::Packet packet;
            packet << static_cast<sf::Int32>( Client::GameEvent );
            packet << static_cast<sf::Int32>( gameAction.type );
            packet << gameAction.position.x;
            packet << gameAction.position.y;

            mSocket.send( packet );
        }

        // Regular position updates
        if( mTickClock.getElapsedTime( ) > sf::seconds( 1.f / 20.f ) )
        {
            sf::Packet positionUpdatePacket;
            positionUpdatePacket << static_cast<sf::Int32>( Client::PositionUpdate );
            positionUpdatePacket << static_cast<sf::Int32>( mLocalPlayerIdentifiers.size( ) );

            for( sf::Int32 identifier : mLocalPlayerIdentifiers )
            {
                if( StarShip* a = mWorld.getStarShip( identifier ) )
                    positionUpdatePacket << identifier << a->getPosition().x << a->getPosition().y << static_cast<sf::Int32>( a->getHitpoints( ) );
            }

            mSocket.send( positionUpdatePacket );
            mTickClock.restart( );
        }

        mTimeSinceLastPacket += dt;
    }
    // Failed to connect and waited for more than 5 seconds: Back to menu
    else if( mFailedConnectionClock.getElapsedTime( ) >= sf::seconds( 2.f ) )
    {
        requestStateClear( );
        requestStackPush( States::Menu );
    }
*/
    return true;
}

void MultiplayerGameState::disableAllRealtimeActions( void )
{
    mActiveState = false;

    for( sf::Int32 identifier : mLocalPlayerIdentifiers )
        mPlayers[identifier]->disableAllRealtimeActions( );
}

bool MultiplayerGameState::handleEvent( const sf::Event& event )
{
    // Game input handling
    CommandQueue& commands = mWorld.getCommandQueue( );

    // Forward event to all players
    for( auto& pair : mPlayers )
        pair.second->handleEvent( event, commands );

    if( event.type == sf::Event::KeyPressed )
    {
        // Enter pressed, add second player co-op (only if we are one player)
        if( event.key.code == sf::Keyboard::Return && mLocalPlayerIdentifiers.size( ) == 1 )
        {
            sf::Packet packet;
            packet << static_cast<sf::Int32>( Client::RequestCoopPartner );
            mSocket.send( packet );
        }

        // Escape pressed, trigger the pause screen
        else if( event.key.code == sf::Keyboard::Escape )
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

        // Sent by the server to order to spawn player 1 airplane on connect
        case Server::SpawnSelf:
        {
            sf::Int32 identifier;
            sf::Vector2f position;
            packet >> identifier >> position.x >> position.y;

            mPlayers[identifier].reset( new Player( &mSocket, identifier, getContext( ).keys1 ) );
            mLocalPlayerIdentifiers.push_back( identifier );

            mGameStarted = true;
        } break;

        //
        case Server::PlayerConnect:
        {
        /*
            sf::Int32 identifier;
            sf::Vector2f position;
            packet >> identifier >> position.x >> position.y;
            StarShip* starShip = ARENA->REDTEAM->starShips[0];
            starShip->setPosition( position );
            mPlayers[identifier].reset( new Player( &mSocket, identifier, nullptr ) );
            */
        } break;
        //
        case Server::PlayerDisconnect:
        {
            sf::Int32 identifier;
            packet >> identifier;

            //mWorld.removeStarShip( identifier );
            mPlayers.erase( identifier );
        } break;
        case Server::AcceptCoopPartner:
        {
            sf::Int32 identifier;
            packet >> identifier;

            mPlayers[identifier].reset( new Player( &mSocket, identifier, getContext( ).keys2 ) );

            mLocalPlayerIdentifiers.push_back( identifier );
        } break;
        case Server::InitialState:
        {

        } break;


        // Player event (like missile fired) occurs
        case Server::PlayerEvent:
        {
            sf::Int32 identifier;
            sf::Int32 action;
            packet >> identifier >> action;

            auto itr = mPlayers.find( identifier );
            if( itr != mPlayers.end( ) )
                itr->second->handleNetworkEvent( static_cast<Player::Action>( action ), mWorld.getCommandQueue( ) );
        } break;

        // Player's movement or fire keyboard state changes
        case Server::PlayerRealtimeChange:
        {
            sf::Int32 identifier;
            sf::Int32 action;
            bool actionEnabled;
            packet >> identifier >> action >> actionEnabled;

            auto itr = mPlayers.find( identifier );
            if( itr != mPlayers.end( ) )
                itr->second->handleNetworkRealtimeChange( static_cast<Player::Action>( action ), actionEnabled );
        } break;

        // Mission successfully completed
        case Server::MissionSuccess:
        {
            std::cout << "MultiplayerGameState Server::MissionSuccess out of date!" << std::endl;
            // requestStackPush(States::MissionSuccess);
        } break;

        // Pickup created
        case Server::SpawnPickup:
        {
        /*
            sf::Int32 type;
            sf::Vector2f position;
            packet >> type >> position.x >> position.y;

            // mWorld.createPickup(position, static_cast<Pickup::Type>(type));
            */
        } break;

        //
        case Server::UpdateClientState:
        {
            float currentWorldPosition;
            sf::Int32 starShipCount;
            packet >> currentWorldPosition >> starShipCount;

            for (sf::Int32 i = 0; i < starShipCount; ++i)
            {
                sf::Vector2f position;
                sf::Int32 identifier;
                packet >> identifier >> position.x >> position.y;

                //StarShip* starShip = mWorld.getStarShip( identifier );
               // bool isLocal = std::find( mLocalPlayerIdentifiers.begin( ), mLocalPlayerIdentifiers.end( ), identifier ) != mLocalPlayerIdentifiers.end( );
               // if( starShip && !isLocal )
                {
                 //   sf::Vector2f interpolatedPosition =  starShip->getPosition( ) + ( position - starShip->getPosition( ) ) * 0.1f;
                 //   starShip->setPosition( interpolatedPosition );
                }
            }
        } break;
    }
}


