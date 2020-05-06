#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Command.hpp"
#include "KeyBinding.hpp"
#include <SFML/Window/Event.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <map>

class Player
{
public:
    typedef PlayerAction::Type Action;

    Player( sf::TcpSocket* socket, sf::Int32 identifier, const KeyBinding* binding );

    void handleEvent( const sf::Event& event, CommandQueue& commands );
    void handleRealtimeInput( CommandQueue& commands );
    void handleRealtimeNetworkInput( CommandQueue& commands );
    // React to events or realtime state changes received over the network
    void handleNetworkEvent(PlayerAction::Type action, CommandQueue& commands , sf::Vector2i pos );
    void handleNetworkRealtimeChange( PlayerAction::Type action, bool actionEnabled );
    void disableAllRealtimeActions( );
    bool isLocal( ) const;
private:
    void initializeActions( );

    const KeyBinding*                       mKeyBinding;
    std::map<PlayerAction::Type, Command>	mActionBinding;
    std::map<PlayerAction::Type, bool>		mActionProxies;
    sf::TcpSocket*                          mSocket;
public:
    int                                     mIdentifier;
};
#endif // PLAYER_HPP

