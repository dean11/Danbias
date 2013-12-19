//////////////////////////////////////////////////////////
// Created 2013											//
// Dennis Andersen, Linda Andersson						//
//////////////////////////////////////////////////////////

#ifndef GAMELOGIC_LOBBY_PROTOCOLS_H
#define GAMELOGIC_LOBBY_PROTOCOLS_H

#include <CustomNetProtocol.h>
#include "ProtocolIdentificationID.h"

namespace GameLogic
{
	struct Protocol_LobbyCreateGame :public Oyster::Network::CustomProtocolObject
	{
		char* mapName;
		char gameId;

		Protocol_LobbyCreateGame()
		{
			this->protocol[0].value = protocol_Lobby_CreateGame;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;

			this->protocol[1].type = Oyster::Network::NetAttributeType_CharArray;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Char;
		}
		Protocol_LobbyCreateGame(Oyster::Network::CustomNetProtocol& o)
		{
			mapName = o[1].value.netCharPtr;
			gameId = o[2].value.netChar;
		}
		Oyster::Network::CustomNetProtocol* GetProtocol() override
		{
			protocol[1].value = mapName;
			protocol[2].value = gameId;
			return &protocol;
		}
		
		private:
			Oyster::Network::CustomNetProtocol protocol;
	};

	struct Protocol_LobbyJoinGame :public Oyster::Network::CustomProtocolObject
	{
		char gameId;

		Protocol_LobbyJoinGame()
		{
			this->protocol[0].value = protocol_Lobby_JoinGame;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;

			this->protocol[1].type = Oyster::Network::NetAttributeType_Char;
		}
		Oyster::Network::CustomNetProtocol* GetProtocol() override
		{
			protocol[1].value = gameId;
			return &protocol;
		}
		
		private:
			Oyster::Network::CustomNetProtocol protocol;
	};

	struct Protocol_LobbyStartGame :public Oyster::Network::CustomProtocolObject
	{
		char gameId;

		Protocol_LobbyStartGame()
		{
			this->protocol[0].value = protocol_Lobby_StartGame;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;

			this->protocol[1].type = Oyster::Network::NetAttributeType_Char;
		}
		Oyster::Network::CustomNetProtocol* GetProtocol() override
		{
			protocol[1].value = gameId;
			return &protocol;
		}
		
		private:
			Oyster::Network::CustomNetProtocol protocol;
	};

	struct Protocol_LobbyJoinLobby :public Oyster::Network::CustomProtocolObject
	{
		Protocol_LobbyJoinLobby()
		{
			this->protocol[0].value = protocol_Lobby_JoinLobby;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
		}
		Oyster::Network::CustomNetProtocol* GetProtocol() override
		{
			return &protocol;
		}
		
		private:
			Oyster::Network::CustomNetProtocol protocol;
	};

	struct Protocol_LobbyLeaveLobby :public Oyster::Network::CustomProtocolObject
	{
		
		Protocol_LobbyLeaveLobby()
		{
			this->protocol[0].value = protocol_Lobby_LeaveLobby;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
		}
		Oyster::Network::CustomNetProtocol* GetProtocol() override
		{
			return &protocol;
		}
		
		private:
			Oyster::Network::CustomNetProtocol protocol;
	};

}

#endif // !GAMELOGIC_PLAYER_PROTOCOLS_H
