#ifndef GAMESERVER_HPP
#define GAMESERVER_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>

#include <vector>
#include <memory>
#include <map>

#include "Core/Category.hpp"

class GameServer
{
	public:
        explicit GameServer( sf::Vector2f battlefieldSize );
        ~GameServer( void );
        void notifyPlayerSpawn( void );
        void notifyPlayerRealtimeChange(sf::Int32 playerIdentifier, sf::Int32 action, bool shipEnabled );
        void notifyPlayerEvent(sf::Int32 identifier, sf::Int32 action , sf::Vector2i pos);
	private:
		// A GameServerRemotePeer refers to one instance of the game, may it be local or from another computer
		struct RemotePeer
		{
            RemotePeer( void );
			sf::TcpSocket			socket;
			sf::Time				lastPacketTime;
			bool					ready;
			bool					timedOut;
            Category::Type          teamColor;
		};
		// Unique pointer to remote peers
		typedef std::unique_ptr<RemotePeer> PeerPtr;
	private:
        void setListening( bool enable );
        void executionThread( void );
        void tick( void );
        sf::Time now( void ) const;

        void handleIncomingPackets( );
        void handleIncomingPacket(sf::Packet& packet, RemotePeer& receivingPeer, bool& detectedTimeout);
        void handleIncomingConnections( );
        void handleDisconnections( );
        void informWorldState( sf::TcpSocket& socket );
        void broadcastMessage(const std::string& message);
        void sendToAll( sf::Packet& packet );
        void updateClientState( );
	private:
		sf::Thread							mThread;
		sf::Clock							mClock;
		sf::TcpListener						mListenerSocket;
		bool								mListeningState;
		sf::Time							mClientTimeoutTime;

		std::size_t							mMaxConnectedPlayers;
		std::size_t							mConnectedPlayers;
        unsigned int                        mPlayerIDCounter;
		std::vector<PeerPtr>				mPeers;
		bool								mWaitingThreadEnd;		
};

#endif // GAMESERVER_HPP
