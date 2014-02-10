//////////////////////////////////////////////////////////
// Created 2013											//
// Dennis Andersen, Linda Andersson						//
//////////////////////////////////////////////////////////

#ifndef GAMELOGIC_LOBBY_PROTOCOLS_H
#define GAMELOGIC_LOBBY_PROTOCOLS_H

#include <CustomNetProtocol.h>
#include "ProtocolIdentificationID.h"

#include <DynamicArray.h>


/**	OBS!
**	It seems like if a string is set in the middle of a data set, 
**	the reciever will crach when trying to use the protocol. 
**	Only tested on Protocol_LobbyStartGame.
**/


namespace GameLogic
{
	struct Protocol_LobbyCreateGame :public Oyster::Network::CustomProtocolObject
	{
		short clientID; // The unuiqe id reprsenting a specific client
		std::string modelName;
		float worldMatrix[16];

		Protocol_LobbyCreateGame()
		{
			int c = 0;
			this->protocol[c].value = protocol_Lobby_Create;
			this->protocol[c++].type = Oyster::Network::NetAttributeType_Short;

			this->protocol[c++].type = Oyster::Network::NetAttributeType_Short;
			for (int i = 0; i <= 16; i++)
			{
				this->protocol[c++].type = Oyster::Network::NetAttributeType_Float;
			}
			this->protocol[c++].type = Oyster::Network::NetAttributeType_CharArray;
		}
		Protocol_LobbyCreateGame(short _clientID, std::string name, float world[16])
		{
			int c = 0;
			this->protocol[c].value = protocol_Lobby_Create;
			this->protocol[c++].type = Oyster::Network::NetAttributeType_Short;

			this->protocol[c++].type = Oyster::Network::NetAttributeType_Short;
			for (int i = 0; i <= 16; i++)
			{
				this->protocol[c++].type = Oyster::Network::NetAttributeType_Float;
			}

			this->protocol[c++].type = Oyster::Network::NetAttributeType_CharArray;

			clientID = _clientID;
			modelName = name;
			memcpy(&worldMatrix[0], &world[0], sizeof(float) * 16);
		}
		Protocol_LobbyCreateGame(Oyster::Network::CustomNetProtocol o)
		{
			int c = 1;
			clientID = o[c++].value.netInt;
			for (int i = 0; i <= 16; i++)
			{
				this->worldMatrix[i] = o[c++].value.netFloat;
			}
			modelName.assign(o[c++].value.netCharPtr);
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			int c = 1;
			protocol[c++].value = clientID;
			
			for (int i = 0; i <= 16; i++)
			{
				this->protocol[c++].value = this->worldMatrix[i];
			}
			protocol.Set(c++, this->modelName);
			return protocol;
		}
		
		private:
			Oyster::Network::CustomNetProtocol protocol;

	};

	struct Protocol_LobbyStartGame :public Oyster::Network::CustomProtocolObject
	{
		float seconds;

		Protocol_LobbyStartGame()
		{
			this->protocol[0].value = protocol_Lobby_Start;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[1].type = Oyster::Network::NetAttributeType_Float;
			seconds = 0;
		}
		Protocol_LobbyStartGame(float _seconds)
		{
			this->protocol[0].value = protocol_Lobby_Start;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
			this->protocol[1].type = Oyster::Network::NetAttributeType_Float;
			seconds = _seconds;
		}
		Protocol_LobbyStartGame(Oyster::Network::CustomNetProtocol& o)
		{
			seconds = o[1].value.netFloat;
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol[1].value = seconds;
			return protocol;
		}
		
		private:
			Oyster::Network::CustomNetProtocol protocol;

	};

	struct Protocol_LobbyLogin :public Oyster::Network::CustomProtocolObject
	{
		// Login stuff
		Protocol_LobbyLogin()
		{
			this->protocol[0].value = protocol_Lobby_Join;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;

			this->protocol[1].type = Oyster::Network::NetAttributeType_Short;
		}
		Protocol_LobbyLogin(Oyster::Network::CustomNetProtocol& p)
		{

		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			return protocol;
		}
		
		private:
			Oyster::Network::CustomNetProtocol protocol;
	};

	//struct Protocol_LobbyJoin :public Oyster::Network::CustomProtocolObject
	//{
	//	short value;
	//
	//	Protocol_LobbyJoin()
	//	{
	//		this->protocol[0].value = protocol_Lobby_Join;
	//		this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
	//		this->protocol[1].type = Oyster::Network::NetAttributeType_Short;
	//	}
	//	Protocol_LobbyJoin(Oyster::Network::CustomNetProtocol& p)
	//	{
	//		this->protocol[0].value = protocol_Lobby_Join;
	//		this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
	//		this->protocol[1].type = Oyster::Network::NetAttributeType_Short;
	//		value = p[1].value.netShort;
	//	}
	//	Oyster::Network::CustomNetProtocol GetProtocol() override
	//	{
	//		protocol[1].value = value;
	//		return &protocol;
	//	}
	//	
	//	private:
	//		Oyster::Network::CustomNetProtocol protocol;
	//};

	struct Protocol_LobbyRefresh :public Oyster::Network::CustomProtocolObject
	{
		Protocol_LobbyRefresh()
		{
			this->protocol[0].value = protocol_Lobby_Login;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
		}
		Protocol_LobbyRefresh(Oyster::Network::CustomNetProtocol& o)
		{
			
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{ return protocol; }
		
		private:
			Oyster::Network::CustomNetProtocol protocol;
	};

	/**
	*	A protocol that contains all data to send to client when update game lobby
	*/

	struct Protocol_LobbyClientData :public Oyster::Network::CustomProtocolObject
	{
		// Player list
		struct PlayerData
		{
			std::string name;
			std::string ip;
			int id;
			int team;
		};
		Utility::DynamicMemory::DynamicArray<PlayerData> list;
		
		Protocol_LobbyClientData()
		{
			this->protocol[0].value = protocol_Lobby_ClientData;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;

			list.Reserve(10);
		}
		Protocol_LobbyClientData(Oyster::Network::CustomNetProtocol& p)
		{
			unsigned int size = this->protocol[1].value.netUInt;
			list.Reserve(size);
			int a = 2;
			for (unsigned int i = 0; i < list.Size(); i++)
			{
				PlayerData d;
				d.id = this->protocol[a++].value.netInt;
				d.team = this->protocol[a++].value.netInt;
				d.name = this->protocol.Get(a++).value.netCharPtr;
				d.ip = this->protocol.Get(a++).value.netCharPtr;
				list.Push(d);
			}
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol[1].value = list.Size();

			int a = 2;
			for (unsigned int i = 0; i < list.Size(); i++)
			{
				this->protocol[a].type = Oyster::Network::NetAttributeType_Int;			// client-id
					this->protocol[a++].value = list[i].id;

				this->protocol[a].type = Oyster::Network::NetAttributeType_Int;			// team-id
					this->protocol[a++].value = list[i].team;

				this->protocol[a].type = Oyster::Network::NetAttributeType_CharArray;	// clientName
					this->protocol.Set(a++, list[i].name);

				this->protocol[a].type = Oyster::Network::NetAttributeType_CharArray;	// clientIP
					this->protocol.Set(a++, list[i].ip);
			}
			
			return protocol;
		}
		
		private:
			Oyster::Network::CustomNetProtocol protocol;
	};

	struct Protocol_LobbyGameData :public Oyster::Network::CustomProtocolObject
	{
		std::string mapName;
		int majorVersion;
		int minorVersion;
	
		Protocol_LobbyGameData()
		{
			this->protocol[0].value = protocol_Lobby_GameData;
			this->protocol[0].type = Oyster::Network::NetAttributeType_Short;

			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;
			this->protocol[2].type = Oyster::Network::NetAttributeType_Int;
			this->protocol[3].type = Oyster::Network::NetAttributeType_CharArray;
		}
		Protocol_LobbyGameData(Oyster::Network::CustomNetProtocol& p)
		{
			majorVersion = (int)p.Get(1).value.netInt;
			minorVersion = (int)p.Get(2).value.netInt;
			mapName = p.Get(3).value.netCharPtr;
		}
		Oyster::Network::CustomNetProtocol GetProtocol() override
		{
			this->protocol[1].value = majorVersion;
			this->protocol[2].value = minorVersion;
			this->protocol.Set(3, mapName.c_str());

			return protocol;
		}
		
		private:
			Oyster::Network::CustomNetProtocol protocol;
	};

	/**
	*	A protocol that contains all data to send to client when update main lobby
	*/
	//struct Protocol_LobbyMainData :public Oyster::Network::CustomProtocolObject
	//{
	//	// Game instance list 
	//
	//	Protocol_LobbyMainData()
	//	{
	//		this->protocol[0].value = protocol_Lobby_MainData;
	//		this->protocol[0].type = Oyster::Network::NetAttributeType_Short;
	//
	//		this->protocol[1].type = Oyster::Network::NetAttributeType_Short;
	//	}
	//	Protocol_LobbyMainData(Oyster::Network::CustomNetProtocol& p)
	//	{
	//
	//	}
	//	Oyster::Network::CustomNetProtocol GetProtocol() override
	//	{
	//		return &protocol;
	//	}
	//	
	//	private:
	//		Oyster::Network::CustomNetProtocol protocol;
	//};
}

#endif // !GAMELOGIC_PLAYER_PROTOCOLS_H
