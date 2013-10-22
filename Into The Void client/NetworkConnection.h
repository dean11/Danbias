#pragma once
#ifndef NETWORKCONNECTION_H
#define NETWORKCONNECTION_H

#include "SocketClient.h"
#include "Protocoll.h"
#include "ThreadPool.h"
#include "Network.h"
#include "NetworkInitStructs.h"
using namespace Network;

class IntoTheVoid;



class NetworkConnection : private SocketClient
{
private:
	ConnectionStatus connStatus;
	Oyster::Math::Float4x4 positions[PLAYER_MAX_COUNT];
	Mutex *mutex;
	Mutex *eventMutex;
	Mutex *effectMutex;
	Mutex *updateMutex;
	ServerToClientUpdateData recvStruct[PLAYER_MAX_COUNT];
	ClientToServerUpdateData sendStruct;
	unsigned int id;
	bool gameOver;
	IntoTheVoid *intoTheVoid;

	Oyster::Math::Float4x4 testVar;
	void setPosition(Oyster::Math::Float4x4 pos, int i);
	LobbyInitData lobbyData;
	std::vector<Event::GameEvent*> eventList;
	std::vector<Network::EffectData> effectList;
	long lastServerUpdate;
	long curClientUpdate;
	__int8 lobbyRenderSync;
	Protocol::RenderData* renderData;
	void setUpdate(ServerToClientUpdateData data, int i);

	NetworkTimer lobbyTimer;
	int lobbyTotalTime;
public:
	int lobbySecondsLeft();

	const ServerToClientUpdateData getUpdate(int i);
	Protocol::RenderData* GetRenderData() const
	{
		while(!renderData)
			Sleep(100);
		
		return renderData;
	}
	int teamID[PLAYER_MAX_COUNT];
	LobbyInitData getLobbyData() {return lobbyData;}
	ConnectionStatus getConnectionStatus() const {return connStatus;}
	void receiveConnStatus(ConnectionStatus cs);
	Oyster::Math::Float4x4 getPosition(int i);

	NetworkConnection();
	virtual ~NetworkConnection();

	enum Result { Success, Failure };
	//Starts the receive thread.
	Result startRecvLoop();

	void setIntoTheVoid(IntoTheVoid *intoTheVoid);

	//Step 1: connect to server IP
	Result connectWithFileData(char* file);
	Result connect( const char *ip, int listenPort, char *userName = "anonymous" );
	//You should now be in a server lobby.
	//Step 2: Press queue button. Will tell the server to set user state to queueing/stop queueing
	void startQueue();
	void stopQueue();
	//Step 3:Server will tell the client that he/she has connected to a game
	//This data will contain things such as other players names, shipIDs and so on
	void receiveLobbyInitData(char* data, int size);
	//Step 4: Select ship.
	//During this time you'll be able to receive other players ship IDswhen they change
	void selectShip(int sid);
	void receiveShipID(int pid, int sid);
	//Step 5: Game start, receive init data
	//Init data will come with final selection data together with positions of players.
	void receiveGameInitData(char* data);
	void receiveRenderData(char* data, int size);
	void doneLoading();
	//Step 6: In this step, you will send event data as well as receive it from other players.
	//void sendData(Oyster::Math::Float4x4 pos);
	void sendData(ClientToServerUpdateData &data);
	void receivePlayerUpdate(char* upd, int size);
	void receiveEvent(char* evd);
	void receiveEffectData(char* data, int size);
	void receiveKeyframe(char* kfd, int size);
	//Step 7: Game finished. Receive data from server that we may want. TBD.
	void receiveGameFinishedData(char* data);

	//Other functions without listed use
	void disconnect();
	Result startSendLoop();
	//void sendMessage(char* msg);
	//void prepareSend( const Float4x4 &orientation );

	unsigned int getPlayerID();

	void fetchEvents(std::vector<Event::GameEvent*> &list);
	void fetchEffects(std::vector<Network::EffectData> &list);

	//Client calls on these functions
	char* getPlayerData();
};

#endif