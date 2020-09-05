#include "GameServer.hpp"
#include "NetworkProtocol.hpp"
#include "Utility.hpp"
#include "Globals.hpp"

#include <iostream> // Debugging only
#include <SFML/Network/Packet.hpp>

GameServer::RemotePeer::RemotePeer( void )
: ready( false )
, timedOut( false )
{
    socket.setBlocking( false );
}

GameServer::GameServer( sf::Vector2f )
: mThread( &GameServer::executionThread, this )
, mListeningState( false )
, mClientTimeoutTime( sf::seconds( 10.f ) )
, mMaxConnectedPlayers( 5 )
, mConnectedPlayers( 0 )
, mPeers( 1 )
, mWaitingThreadEnd( false )
{
    mListenerSocket.setBlocking( false );
    mPeers[0].reset( new RemotePeer( ) );
    mThread.launch( );
}

GameServer::~GameServer( void )
{
    mWaitingThreadEnd = true;
    mThread.wait( );
}

void GameServer::notifyPlayerRealtimeChange( sf::Int32 playerIdentifier, sf::Int32 action, bool actionEnabled )
{
    for( std::size_t i = 0; i < mConnectedPlayers; ++i )
	{
        if( mPeers[i]->ready )
		{
			sf::Packet packet;
            packet << static_cast<sf::Int32>( Server::PlayerRealtimeChange );
            packet << playerIdentifier;
			packet << action;
			packet << actionEnabled;

            mPeers[i]->socket.send( packet );
		}
	}
}

void GameServer::notifyPlayerEvent( sf::Int32 identifier, sf::Int32 action, sf::Vector2i pos )
{
    for( std::size_t i = 0; i < mConnectedPlayers; ++i )
	{
        if( mPeers[i]->ready )
		{
            sf::Packet packet;
            packet << action;
            packet << identifier;
            packet << pos.x;
            packet << pos.y;
            mPeers[i]->socket.send( packet );
		}
	}  
}

void GameServer::notifyPlayerSpawn( void )
{
    for( std::size_t i = 0; i < mConnectedPlayers; ++i )
    {
        if( mPeers[i]->ready )
		{
            broadcastMessage( "New Player!" );
			sf::Packet packet;
            packet << static_cast<sf::Int32>( Server::PlayerConnect );

            // load gold and devotion points??
            packet << static_cast<unsigned int>( mConnectedPlayers );
            mPeers[i]->socket.send( packet );
		}
	}
}

void GameServer::setListening( bool enable )
{
	// Check if it isn't already listening
    if( enable )
	{	
        if( !mListeningState )
            mListeningState = ( mListenerSocket.listen( ServerPort ) == sf::TcpListener::Done );
	}
	else
	{
        mListenerSocket.close( );
		mListeningState = false;
	}
}

void GameServer::executionThread( void )
{
    setListening( true );

    sf::Time stepInterval = sf::seconds( 1.f / 60.f );
	sf::Time stepTime = sf::Time::Zero;
    sf::Time tickInterval = sf::seconds( 1.f / 20.f );
	sf::Time tickTime = sf::Time::Zero;
	sf::Clock stepClock, tickClock;

    while( !mWaitingThreadEnd )
	{	
        handleIncomingPackets( );
        handleIncomingConnections( );

        stepTime += stepClock.getElapsedTime( );
        stepClock.restart( );

        tickTime += tickClock.getElapsedTime( );
        tickClock.restart( );

        // Fixed update step
        while (stepTime >= stepInterval)
        {
            //mBattleFieldRect.top += mBattleFieldScrollSpeed * stepInterval.asSeconds();
            stepTime -= stepInterval;
        }

		// Fixed tick step
        while( tickTime >= tickInterval )
		{
            tick( );
			tickTime -= tickInterval;
		}

		// Sleep to prevent server from consuming 100% CPU
        sf::sleep( sf::milliseconds( 100 ) );
	}	
}

void GameServer::tick( void )
{
    updateClientState(  );

    // check for victory conditions

    // Run Game logic here
}

sf::Time GameServer::now( void ) const
{
    return mClock.getElapsedTime( );
}

void GameServer::handleIncomingPackets( void )
{
	bool detectedTimeout = false;
	
    for( PeerPtr& peer : mPeers )
	{
        if( peer->ready )
		{
			sf::Packet packet;
            while( peer->socket.receive( packet ) == sf::Socket::Done )
			{
				// Interpret packet and react to it
                handleIncomingPacket( packet, *peer, detectedTimeout );

				// Packet was indeed received, update the ping timer
                peer->lastPacketTime = now( );
                packet.clear( );
			}

            //if( now( ) >= peer->lastPacketTime + mClientTimeoutTime )
            //{
            //	peer->timedOut = true;
            //	detectedTimeout = true;
            //}
		}
	}

    //if( detectedTimeout )
    //    handleDisconnections( );
}

void GameServer::handleIncomingPacket( sf::Packet& packet, RemotePeer& receivingPeer, bool& detectedTimeout )
{
	sf::Int32 packetType;
	packet >> packetType;

    switch( packetType )
	{
		case Client::Quit:
		{
			receivingPeer.timedOut = true;
			detectedTimeout = true;
		} break;

		case Client::PlayerEvent:
		{
            sf::Int32 identifier;
			sf::Int32 action;
            sf::Vector2i pos;
            packet >> identifier >> action >> pos.x >> pos.y;

            notifyPlayerEvent( identifier, action, pos );
		} break;
    case Client::GameEvent:
		{
			// Enemy explodes: With certain probability, drop pickup
			// To avoid multiple messages spawning multiple pickups, only listen to first peer (host)
            //if( action == GameActions::EnemyExplode && randomInt( 3 ) == 0 && &receivingPeer == mPeers[0].get( ) )
			{
                /* BOOK EXAMPLE
				sf::Packet packet;
                packet << static_cast<sf::Int32>( Server::SpawnPickup );
                packet << static_cast<sf::Int32>( randomInt(Pickup::TypeCount ) );
				packet << x;
				packet << y;

				sendToAll(packet);
                */
			}
        }break;
    case Client::LeftClick:
    {
        float x;
        float y;
        sf::Int32 identifier;
        packet >> identifier;
        packet >> x;
        packet >> y;
        notifyPlayerEvent( identifier, Server::LeftClick, sf::Vector2i( x, y ) );
    }break;
    case Client::RightClick:
    {

    }break;
    case Client::SpawnUnit:
    {
        sf::Int32 unitType;
        float x;
        float y;
        packet >> unitType;
        packet >> x;
        packet >> y;
        sf::Packet unitPacket;
        unitPacket << static_cast<sf::Int32>( Server::SpawnUnit );
        unitPacket << unitType;
        unitPacket << x;
        unitPacket << y;
        sendToAll( unitPacket );
    }break;
	}
}

void GameServer::updateClientState( void )
{
    // update the position and stats of all buildings and units to each client here
    //sf::Packet updateClientStatePacket;
    //updateClientStatePacket << static_cast<sf::Int32>( Server::UpdateClientState );

    // update unit positions and stats
    //for( auto unit : this->mUnits )
        //updateClientStatePacket << unit.first << unit.second.position.x << unit.second.position.y;

    // update building stats

    //sendToAll( updateClientStatePacket );
}

void GameServer::handleIncomingConnections( void )
{
    if( mListeningState && mListenerSocket.accept( mPeers[mConnectedPlayers]->socket ) == sf::TcpListener::Done )
    {
        sf::Packet packet;
        packet << static_cast<sf::Int32>( Server::SpawnSelf );
        packet << static_cast<sf::Int32>( mConnectedPlayers );
        packet << 0;
        packet << 0;

        packet << static_cast<sf::Int32>( mPeers[mConnectedPlayers]->teamColor );

        mPeers[mConnectedPlayers]->socket.send(packet);
		mPeers[mConnectedPlayers]->ready = true;
		mPeers[mConnectedPlayers]->lastPacketTime = now(); // prevent initial timeouts

        mConnectedPlayers++;
        notifyPlayerSpawn( );

        if( mConnectedPlayers >= mMaxConnectedPlayers )
            setListening( false );
		else // Add a new waiting peer
            mPeers.push_back( PeerPtr( new RemotePeer( ) ) );
	}
}

void GameServer::handleDisconnections( )
{
    /*
    for( auto itr = mPeers.begin( ); itr != mPeers.end(); )
	{
        if( (*itr)->timedOut )
		{
			mConnectedPlayers--;

            itr = mPeers.erase( itr );

			// Go back to a listening state if needed
            if( mConnectedPlayers < mMaxConnectedPlayers )
			{
                mPeers.push_back( PeerPtr( new RemotePeer( ) ) );
                setListening( true );
			}
				
            broadcastMessage( "An ally has disconnected." );
		}
		else
		{
			++itr;
		}
	}
    */
}

// Tell the newly connected peer about how the world is currently
void GameServer::informWorldState( sf::TcpSocket& )
{
    /*
     *  This packet is not handled client side.
     *
	sf::Packet packet;
    packet << static_cast<sf::Int32>( Server::InitialState );
    //packet << mWorldHeight << mBattleFieldRect.top + mBattleFieldRect.height;
    packet << static_cast<sf::Int32>( mConnectedPlayers );

    socket.send( packet );
    */
}

void GameServer::broadcastMessage( const std::string& message )
{
    for( std::size_t i = 0; i < mConnectedPlayers; ++i )
	{
        if( mPeers[i]->ready )
		{
			sf::Packet packet;
            packet << static_cast<sf::Int32>( Server::BroadcastMessage );
			packet << message;

            mPeers[i]->socket.send( packet );
		}	
	}
}

void GameServer::sendToAll( sf::Packet& packet )
{
    for( PeerPtr& peer : mPeers )
	{
        if( peer->ready )
            peer->socket.send( packet );
	}
}
