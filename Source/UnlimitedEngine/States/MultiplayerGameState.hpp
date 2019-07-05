#ifndef MULTIPLAYERGAMESTATE_HPP
#define MULTIPLAYERGAMESTATE_HPP

#include "Core/State.hpp"
#include "Core/World.hpp"
#include "Core/Player.hpp"
#include "Core/GameServer.hpp"
#include "Core/Globals.hpp"

#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/Packet.hpp>

class MultiplayerGameState : public State
{
public:
    MultiplayerGameState(States::ID id, StateStack& stack, Context context , bool isHost );
    ~MultiplayerGameState( void );

    virtual void draw( void );
    void onDestroy( void );
    virtual bool update( sf::Time dt );
    void disableAllRealtimeActions( void );

    virtual bool handleEvent( const sf::Event& event );
    virtual void				onActivate( void );

private:
    void						updateBroadcastMessage(sf::Time elapsedTime);
    void						handlePacket(sf::Int32 packetType, sf::Packet& packet);

private:
    typedef std::unique_ptr<Player> PlayerPtr;

protected:
    World                       mWorld;
    sf::RenderWindow&			mWindow;
    std::map<int, PlayerPtr>	mPlayers;

    TextureManager&             mTextureManager;
    std::vector<sf::Int32>		mLocalPlayerIdentifiers;
    sf::TcpSocket				mSocket;
    bool						mConnected;
    std::unique_ptr<GameServer> mGameServer;
    sf::Clock					mTickClock;

    std::vector<std::string>	mBroadcasts;
    sf::Text					mBroadcastText;
    sf::Time					mBroadcastElapsedTime;

    sf::Text					mPlayerInvitationText;
    sf::Time					mPlayerInvitationTime;

    sf::Text					mFailedConnectionText;
    sf::Clock					mFailedConnectionClock;

    bool						mActiveState;
    bool						mHasFocus;
    bool						mHost;
    bool						mGameStarted;
    sf::Time					mClientTimeout;
    sf::Time					mTimeSinceLastPacket;
};

#endif // MULTIPLAYERGAMESTATE_HPP
