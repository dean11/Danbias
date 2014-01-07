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
			this->protocol[protocol_INDEX_ID].value = protocol_Lobby_CreateGame;
			this->protocol[protocol_INDEX_ID].type = Oyster::Network::NetAttributeType_Short;

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
			this->protocol[protocol_INDEX_ID].value = protocol_Lobby_JoinGame;
			this->protocol[protocol_INDEX_ID].type = Oyster::Network::NetAttributeType_Short;

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
			this->protocol[protocol_INDEX_ID].value = protocol_Lobby_StartGame;
			this->protocol[protocol_INDEX_ID].type = Oyster::Network::NetAttributeType_Short;

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
		int LobbyID;
		Protocol_LobbyJoinLobby(int id = -1)
		{
			this->protocol[protocol_INDEX_ID].value = protocol_Lobby_JoinLobby;
			this->protocol[protocol_INDEX_ID].type = Oyster::Network::NetAttributeType_Short;

			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;
			LobbyID = id;
		}
		Protocol_LobbyJoinLobby(Oyster::Network::CustomNetProtocol& o)
		{
			LobbyID = o[1].value.netInt;
		}
		Oyster::Network::CustomNetProtocol* GetProtocol() override
		{
			this->protocol[1].value = LobbyID;

			return &protocol;
		}
		
		private:
			Oyster::Network::CustomNetProtocol protocol;
	};

	struct Protocol_LobbyLeaveLobby :public Oyster::Network::CustomProtocolObject
	{
		
		Protocol_LobbyLeaveLobby()
		{
			this->protocol[protocol_INDEX_ID].value = protocol_Lobby_LeaveLobby;
			this->protocol[protocol_INDEX_ID].type = Oyster::Network::NetAttributeType_Short;
		}
		Oyster::Network::CustomNetProtocol* GetProtocol() override
		{
			return &protocol;
		}
		
		private:
			Oyster::Network::CustomNetProtocol protocol;
	};

	struct Protocol_LobbyUpdate :public Oyster::Network::CustomProtocolObject
	{
		struct LobbyUpdateData
		{
			std::string mapName;
			int LobbyId;
		};
		int count;
		LobbyUpdateData* data;
		Protocol_LobbyUpdate()
		{
			this->protocol[protocol_INDEX_ID].value = protocol_Lobby_LeaveLobby;
			this->protocol[protocol_INDEX_ID].type = Oyster::Network::NetAttributeType_Short;

			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;
		}
		Protocol_LobbyUpdate( Oyster::Network::CustomNetProtocol* p )
		{
			count = (*p)[1].value.netInt;
			data = new LobbyUpdateData[count];
			for (int i = 0; i < count; i++)
			{
				//data[i].mapName = (*p)[i].value.
			}
		}
		~Protocol_LobbyUpdate()
		{
			delete [] data;
			data = 0;
		}
		Oyster::Network::CustomNetProtocol* GetProtocol() override
		{
			this->protocol[1].value.netInt = count;
			for (int i = 2; i < count; i++)
			{
				protocol[i].type = Oyster::Network::NetAttributeType_CharArray;
				protocol[i+1].type = Oyster::Network::NetAttributeType_Int;

				protocol[i].value.netCharPtr = const_cast<char*>(data[i-2].mapName.c_str());
				protocol[i+1].value.netInt = data[i-1].LobbyId;
			}
			return &protocol;
		}
		
		private:
			Oyster::Network::CustomNetProtocol protocol;
	};
}

#endif // !GAMELOGIC_PLAYER_PROTOCOLS_H
