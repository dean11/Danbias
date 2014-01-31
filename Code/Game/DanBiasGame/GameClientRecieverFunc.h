#ifndef DANBIAS_CLIENTRECIEVEROBJECT_H
#define DANBIAS_CLIENTRECIEVEROBJECT_H

//WTF!? No headers included???
#include "../DanBiasGame/Include/DanBiasGame.h"
#include "../GameProtocols/GeneralProtocols.h"

namespace DanBias
{
	
	struct GameRecieverObject	:public Oyster::Network::NetworkClient
	{
		Client::GameClientState* gameClientState;

		// receiver function for server messages 
		// parsing protocols and sending it to the gameState
		void NetworkCallback(Oyster::Network::CustomNetProtocol& p) override
		{
			int pType = p[0].value.netInt;
			switch (pType)
			{
			case protocol_General_Status:
				{
					GameLogic::Protocol_General_Status::States state;
					state =  (GameLogic::Protocol_General_Status::States)p[1].value.netShort;
					if( state == GameLogic::Protocol_General_Status::States_disconected)
					{
						// server disconnected 
						DanBiasGame::Release();
					}
				}
				break;
			case protocol_Gameplay_PlayerMovement:
				{
					Client::GameClientState::KeyInput* protocolData = new Client::GameClientState::KeyInput;
					for(int i = 0; i< 6; i++)
					{
						protocolData->key[i] = p[i+1].value.netBool;
					}

					if(dynamic_cast<Client::GameState*>(gameClientState))
						((Client::GameState*)gameClientState)->Protocol(protocolData);
					delete protocolData;
					protocolData = NULL;
				}
				break;
			//case protocol_Gameplay_PlayerPosition:
			//	{
			//		Client::GameClientState::PlayerPos* protocolData = new Client::GameClientState::PlayerPos;
			//		for(int i = 0; i< 3; i++)
			//		{
			//			protocolData->playerPos[i] = p[i].value.netFloat;
			//		}
			//		if(dynamic_cast<Client::GameState*>(gameClientState))
			//			((Client::GameState*)gameClientState)->Protocol(protocolData);
			//		delete protocolData;
			//		protocolData = NULL;
			//	}
			//	break;

			case protocol_Gameplay_ObjectCreate:
				{
					Client::GameClientState::NewObj* protocolData = new Client::GameClientState::NewObj;
					protocolData->object_ID = p[1].value.netInt;
					protocolData->path = p[2].value.netCharPtr;
					for(int i = 0; i< 16; i++)
					{
						protocolData->worldPos[i] = p[i+3].value.netFloat;
					}

					if(dynamic_cast<Client::GameState*>(gameClientState))
						((Client::GameState*)gameClientState)->Protocol(protocolData);

					delete p[2].value.netCharPtr; //delete char array
					delete protocolData;
					protocolData = NULL;
				}
				break;
			case protocol_Gameplay_ObjectDisabled:
				{
					Client::GameClientState::RemoveObj* protocolData = new Client::GameClientState::RemoveObj;
					protocolData->object_ID = p[1].value.netInt;

					if(dynamic_cast<Client::GameState*>(gameClientState))
						((Client::GameState*)gameClientState)->Protocol(protocolData);

					delete protocolData;
					protocolData = NULL;
				}
				break;
			case protocol_Gameplay_ObjectPosition:
				{

					Client::GameClientState::ObjPos protocolData;
					protocolData.object_ID = p[1].value.netInt;
					for(int i = 0; i< 16; i++)
					{
						protocolData.worldPos[i] = p[i+2].value.netFloat;
					}

					if(dynamic_cast<Client::GameState*>(gameClientState))
						((Client::GameState*)gameClientState)->Protocol(&protocolData);
				}
				break;

			default:
				break;
			}	

			if(ProtocolIsLobby(p[0].value.netInt))	ParseLobbyProtocol(p);
		}

		void ParseLobbyProtocol(Oyster::Network::CustomNetProtocol& p)
		{
			switch (p[0].value.netShort)
			{
				case protocol_General_Status:	//this->GeneralStatus		(Protocol_General_Status	(p), c);
				break;
				case protocol_General_Text:		//this->GeneralText		(Protocol_General_Text		(p), c);
				break;
				//case protocol_Lobby_Create:		this->LobbyCreateGame	(Protocol_LobbyCreateGame	(p), c);
				//break;
				case protocol_Lobby_Start:		//this->LobbyStartGame	(Protocol_LobbyStartGame	(p), c);
				break;
				//case protocol_Lobby_Join:		this->LobbyJoin			(Protocol_LobbyJoin			(p), c);
				//break;
				case protocol_Lobby_Login:		//this->LobbyLogin		(Protocol_LobbyLogin		(p), c);
				break;
				case protocol_Lobby_Refresh:	//this->LobbyRefresh		(Protocol_LobbyRefresh		(p), c);
				break;
				case protocol_Lobby_GameData:	//this->LobbyGameData		(Protocol_LobbyGameData		(p), c);
				{
					GameLogic::Protocol_LobbyGameData temp(p);
					printf("%s, %i.%i\n", temp.mapName.c_str(), temp.majorVersion, temp.minorVersion);
				}
				break;
				case protocol_Lobby_ClientData:	//this->LobbyMainData		(Protocol_LobbyClientData	(p), c);
				break;
				//case protocol_Lobby_GameData:	this->LobbyGameData		(Protocol_LobbyGameData		(p), c);
				//break;
			}
		}
	};
} 
#endif