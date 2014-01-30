#ifndef DANBIAS_CLIENTRECIEVEROBJECT_H
#define DANBIAS_CLIENTRECIEVEROBJECT_H

//WTF!? No headers included???

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

				Client::GameClientState::ObjPos* protocolData = new Client::GameClientState::ObjPos;
				protocolData->object_ID = p[1].value.netInt;
				for(int i = 0; i< 16; i++)
				{
					protocolData->worldPos[i] = p[i+2].value.netFloat;
				}

				if(dynamic_cast<Client::GameState*>(gameClientState))
					((Client::GameState*)gameClientState)->Protocol(protocolData);

				delete protocolData;
				protocolData = NULL;
			}
			break;

		default:
			break;
		}	

	}
};
} 
#endif