//////////////////////////////////////////////////////////
// Created 2013											//
// Dennis Andersen, Linda Andersson						//
//////////////////////////////////////////////////////////

#ifndef GAMELOGIC_LOBBY_PROTOCOLS_H
#define GAMELOGIC_LOBBY_PROTOCOLS_H

#include <CustomNetProtocol.h>
#include "ProtocolIdentificationID.h"

#include <DynamicArray.h>


namespace GameLogic
{
	/*
	struct Protocol_LobbyCreateGame :public Oyster::Network::CustomProtocolObject
	{
		char* mapName;
		char gameId;

		Protocol_LobbyCreateGame()
		{
			this->protocol[protocol_INDEX_ID].value = protocol_Lobby_Create;
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
	*/
	struct Protocol_LobbyStartGame :public Oyster::Network::CustomProtocolObject
	{
		short gameId;

		Protocol_LobbyStartGame()
		{
			this->protocol[protocol_INDEX_ID].value = protocol_Lobby_Start;
			this->protocol[protocol_INDEX_ID].type = Oyster::Network::NetAttributeType_Short;

			this->protocol[1].type = Oyster::Network::NetAttributeType_Short;
		}
		Protocol_LobbyStartGame(Oyster::Network::CustomNetProtocol& o)
		{
			gameId = o[1].value.netInt;
		}
		Oyster::Network::CustomNetProtocol* GetProtocol() override
		{
			protocol[1].value = gameId;
			return &protocol;
		}
		
		private:
			Oyster::Network::CustomNetProtocol protocol;
	};

	struct Protocol_LobbyLogin :public Oyster::Network::CustomProtocolObject
	{
		// Login stuff
		Protocol_LobbyLogin()
		{
			this->protocol[protocol_INDEX_ID].value = protocol_Lobby_Join;
			this->protocol[protocol_INDEX_ID].type = Oyster::Network::NetAttributeType_Short;

			this->protocol[1].type = Oyster::Network::NetAttributeType_Short;
		}
		Protocol_LobbyLogin(Oyster::Network::CustomNetProtocol& p)
		{

		}
		Oyster::Network::CustomNetProtocol* GetProtocol() override
		{
			return &protocol;
		}
		
		private:
			Oyster::Network::CustomNetProtocol protocol;
	};

	struct Protocol_LobbyJoin :public Oyster::Network::CustomProtocolObject
	{
		short value;

		Protocol_LobbyJoin()
		{
			this->protocol[protocol_INDEX_ID].value = protocol_Lobby_Join;
			this->protocol[protocol_INDEX_ID].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[1].type = Oyster::Network::NetAttributeType_Short;
		}
		Protocol_LobbyJoin(Oyster::Network::CustomNetProtocol& p)
		{
			this->protocol[protocol_INDEX_ID].value = protocol_Lobby_Join;
			this->protocol[protocol_INDEX_ID].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[1].type = Oyster::Network::NetAttributeType_Short;
			value = p[1].value.netShort;
		}
		Oyster::Network::CustomNetProtocol* GetProtocol() override
		{
			protocol[1].value = value;
			return &protocol;
		}
		
		private:
			Oyster::Network::CustomNetProtocol protocol;
	};

	struct Protocol_LobbyRefresh :public Oyster::Network::CustomProtocolObject
	{
		Protocol_LobbyRefresh()
		{
			this->protocol[protocol_INDEX_ID].value = protocol_Lobby_Login;
			this->protocol[protocol_INDEX_ID].type = Oyster::Network::NetAttributeType_Short;
		}
		Protocol_LobbyRefresh(Oyster::Network::CustomNetProtocol& o)
		{
			
		}
		Oyster::Network::CustomNetProtocol* GetProtocol() override
		{ return &protocol; }
		
		private:
			Oyster::Network::CustomNetProtocol protocol;
	};

	/**
	*	A protocol that contains all data to send to client when update game lobby
	*/
	struct Protocol_LobbyGameData :public Oyster::Network::CustomProtocolObject
	{
		// Player list
		struct PlayerData
		{
			std::string name;
			int id;
		};
		Utility::DynamicMemory::DynamicArray<PlayerData> list;

		Protocol_LobbyGameData()
		{
			this->protocol[protocol_INDEX_ID].value = protocol_Lobby_GameData;
			this->protocol[protocol_INDEX_ID].type = Oyster::Network::NetAttributeType_Short;

			list.Reserve(10);
		}
		Protocol_LobbyGameData(Oyster::Network::CustomNetProtocol& p)
		{
			this->protocol[protocol_INDEX_ID].value = protocol_Lobby_GameData;
			this->protocol[protocol_INDEX_ID].type = Oyster::Network::NetAttributeType_Short;

			list.Reserve(10);
		}
		Oyster::Network::CustomNetProtocol* GetProtocol() override
		{
			int a = 1;
			for (unsigned int i = 0; i < list.Size(); i++)
			{
				this->protocol[a].type = Oyster::Network::NetAttributeType_Int;
				this->protocol[a].type = Oyster::Network::NetAttributeType_CharArray;

				this->protocol[a].value = list[i].id;
				this->protocol.Set(a, list[i].name);
			}
			return &protocol;
		}
		
		private:
			Oyster::Network::CustomNetProtocol protocol;
	};
	/**
	*	A protocol that contains all data to send to client when update main lobby
	*/
	struct Protocol_LobbyMainData :public Oyster::Network::CustomProtocolObject
	{
		// Game instance list 

		Protocol_LobbyMainData()
		{
			this->protocol[protocol_INDEX_ID].value = protocol_Lobby_MainData;
			this->protocol[protocol_INDEX_ID].type = Oyster::Network::NetAttributeType_Short;

			this->protocol[1].type = Oyster::Network::NetAttributeType_Short;
		}
		Protocol_LobbyMainData(Oyster::Network::CustomNetProtocol& p)
		{

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
