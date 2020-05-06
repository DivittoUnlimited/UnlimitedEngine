#ifndef NETWORKPROTOCOL_HPP
#define NETWORKPROTOCOL_HPP

#include <SFML/Config.hpp>
#include <SFML/System/Vector2.hpp>


const unsigned short ServerPort = 4000;

namespace Server
{
	// Packets originated in the server
	enum PacketType
	{
		BroadcastMessage,	// format: [Int32:packetType] [string:message]
		PlayerEvent,
		PlayerRealtimeChange,
		PlayerConnect,
		PlayerDisconnect,
        MissionSuccess,
        UpdateClientState,
        LeftClick,
        RightClick,
        ChangeTurn,
        SpawnSelf,
        SpawnUnit
	};
}

namespace Client
{
	// Packets originated in the client
	enum PacketType
	{
		PlayerEvent,
		PlayerRealtimeChange,
		GameEvent,
        Quit,
        LeftClick,
        RightClick,
        ChangeTurn,
        SpawnUnit
	};
}

namespace PlayerActions
{
	enum Action
	{
        LeftClick,
        RightClick,
		ActionCount
	};
}

namespace GameActions
{
	enum Type
	{
        LeftClick,
        RightClick,
        ChangeTurn,
        SpawnUnit,
        ActionCount
	};

	struct Action
	{
		Action()
		{ // leave uninitialized
		}

		Action(Type type, sf::Vector2f position)
		: type(type)
		, position(position)
		{
		}

		Type			type;
		sf::Vector2f	position;
	};
}

#endif // NETWORKPROTOCOL_HPP
