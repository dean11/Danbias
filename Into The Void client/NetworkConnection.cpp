#include "NetworkConnection.h"
#include "Player.h"
#include "IntoTheVoid.h"

NetworkConnection::NetworkConnection()
	:
	SocketClient()
{
	this->intoTheVoid = NULL;
	mutex = new Mutex();
	eventMutex = new Mutex();
	effectMutex = new Mutex();
	updateMutex=new Mutex();
	connStatus=OFFLINE;
	lastServerUpdate=-1;
	curClientUpdate=0;
	
	lobbyRenderSync=0;
	renderData = 0;
	//client.setPlayerContPtr(this);
}

NetworkConnection::~NetworkConnection()
{
	delete mutex;
	delete eventMutex;
}

void NetworkConnection::setIntoTheVoid(IntoTheVoid *intoTheVoid)
{
	this->intoTheVoid = intoTheVoid;
}

void NetworkConnection::receiveConnStatus(ConnectionStatus cs)
{
	connStatus=cs;
}
bool LoadConnectionData(char* filename, char* &usr, char* &ip, int &port )
{
	port=-1;
	bool userSet=false;
	bool ipSet=false;
	std::ifstream ifs;
	char in[50];
	std::vector<std::string> cont;
	ifs.open(filename);
	if(!ifs.is_open())
	{
		return false;
	}
	while(!ifs.eof())
	{
		ifs.getline(in, 100);
		cont=splitString(in, '=');
		if (cont.size()==2)
		{
			if(!strcmp("ipaddress", cont[0].c_str()))
			{
				ipSet=true;
				strcpy(ip, cont[1].c_str());

			}
			else if(!strcmp("username", cont[0].c_str()))
			{
				userSet=true;
				strcpy(usr, cont[1].c_str());
			}
			else if(!strcmp("portrange", cont[0].c_str()))
			{
				cont=splitString(cont[1].c_str(),'-');
				if(cont.size()>1)
				{
					int min=atoi(cont[0].c_str());
					int range=atoi(cont[1].c_str())-min;
					port=(rand()%range)+min;
				}
				else
				{
					port=atoi(cont[0].c_str());
				}
			}
		}

	}
	ifs.close();
	if(ipSet && port>0 && userSet)
		return true;
	return false;
}
NetworkConnection::Result NetworkConnection::connect(const char *ip, int listenPort, char *userName)
{
	//MSG msg;
	gameOver=false;
	startRecvLoop();
	if(connectToIP(ip, listenPort, userName, (int)strlen(userName)))
		return Success;
	return Failure;
}
NetworkConnection::Result NetworkConnection::connectWithFileData(char* file)
{
	//MSG msg;
	char* userName=new char[100];
	
	char* ip=new char[100];
	int port;
	if(LoadConnectionData(file, userName, ip, port))
	{
		return connect(ip, port, userName);
	} else 
		return connect("127.0.0.1", 22222, "FILE_NOT_FOUND");
	return Failure;
}
void NetworkConnection::startQueue()
{
	sendMessage("/qst");
}
void NetworkConnection::stopQueue()
{
	sendMessage("/qed");
}
void NetworkConnection::receiveKeyframe(char* kfd, int size)
{
	//Key frams aren't used right now.
	/*KeyFrame kf = KeyFrame(kfd);
	mutex->Lock();
	if (intoTheVoid)
		intoTheVoid->RecievedKeyFrame(kf);
	mutex->Unlock();
	int test=0;*/
}
void NetworkConnection::receiveEvent(char* evd)
{
	Event::GameEvent* newEvent;
	Event::Type eTest;
	memcpy(&eTest, evd, sizeof(Event::Type));
	switch (eTest)
	{
		case Event::Type::eBulletHit:
			newEvent=new Event::BulletHit();
			break;
		case Event::Type::eBulletCreated:
			newEvent=new Event::BulletCreated();
			break;
		case Event::Type::eShipSpawned:
			newEvent=new Event::ShipSpawned();
			break;
		case Event::Type::eGameEnded:
			newEvent=new Event::GameEnded();
			gameOver=true;
			break;
		case Event::Type::eShipDestroyed:
			newEvent=new Event::ShipDestroyed();
			break;
		default://Invalid data
			return;
	}
	newEvent->LoadRawData(evd+4);

	eventMutex->Lock();
	eventList.push_back(newEvent);
	eventMutex->Unlock();
}
void NetworkConnection::receiveRenderData(char* data, int size)
{
	renderData=new Protocol::RenderData();
	renderData->readFromBuffer(data);
	//SetRenderData does not exist at the moment
	lobbyRenderSync++;
	if (lobbyRenderSync==2)
		if (connStatus!= ONLINE_INGAME) 
			connStatus=ONLINE_INLOBBY;
}
void NetworkConnection::receiveEffectData(char* data, int size)
{
	int count=size/sizeof(Network::EffectData);
	int offset=sizeof(Network::EffectData);
	Network::EffectData test;
	effectMutex->Lock();
	effectList.resize(count);
	for (int i=0; i<count; i++)
	{
		memcpy(&test, data+offset*i, sizeof(Network::EffectData));
		effectList[i]=test;
	}
	effectMutex->Unlock();

}
void NetworkConnection::receiveLobbyInitData(char* data, int size)
{
	lobbyTimer.start();
	lobbyTimer.reset();
	LobbyInitData lobby;
	memcpy(&lobby, data, size);
	lobbyTotalTime=lobby.timer;
	id=lobby.pid;

	for(unsigned int i=0; i<PLAYER_MAX_COUNT; i++)
	{
		//if (intoTheVoid)
		//	intoTheVoid->SetPlayerPosition(i, init.pos[i].position);

		//positions[i]=init.pos[i].position;
	}
	lobbyData=lobby;
	lobbyRenderSync++;
	if (lobbyRenderSync==2)
		if (connStatus!= ONLINE_INGAME) 
			connStatus=ONLINE_INLOBBY;
}
int NetworkConnection::lobbySecondsLeft()
{
	lobbyTimer.tick();
	int timeLeft=(int)(  (lobbyTotalTime-lobbyTimer.getGameTime()) );
	if(timeLeft>0)
		return timeLeft;
	else
		return 0;
}
void NetworkConnection::disconnect()
{
	sendMessage("/dc");
	closeConnection();
}
void NetworkConnection::doneLoading()
{
	sendMessage("/rdy");
}

NetworkConnection::Result NetworkConnection::startSendLoop()
{
	//startSendDataThread();
	return Failure;
}

NetworkConnection::Result NetworkConnection::startRecvLoop()
{
	startReceiveThread();
	return Success;
}


void NetworkConnection::receivePlayerUpdate(char* ps, int size)
{
	ServerToClientUpdateData recv;
	int count=size/Network::SERVER_PLAYER_DATA_SIZE;
	for (int i=0; i<count; i++)
	{
		updateMutex->Lock();
		memcpy( &recvStruct[i], ps+i*Network::SERVER_PLAYER_DATA_SIZE, SERVER_PLAYER_DATA_SIZE );
		updateMutex->Unlock();
		if ( lastServerUpdate < recv.updateCount )
		{
			setUpdate(recv,recv.pid);
			if(id==1)
			{
				int a=0;
				if(recvStruct[i].pid==1)
				{
					int b=0;
				}
			}
			//intoTheVoid->SetPlayerUpdate( recvStruct[i] );
			lastServerUpdate = recv.updateCount;
		}
	}
}
void NetworkConnection::setUpdate(ServerToClientUpdateData data, int i)
{
	updateMutex->Lock();
	recvStruct[i]=data;
	updateMutex->Unlock();
}
const ServerToClientUpdateData NetworkConnection::getUpdate(int i)
{
	updateMutex->Lock();
	ServerToClientUpdateData data=recvStruct[i];
	updateMutex->Unlock();
	return data;
}
Oyster::Math::Float4x4 NetworkConnection::getPosition(int i)
{
	return positions[i];
}

unsigned int NetworkConnection::getPlayerID()
{
	return id;
}


void NetworkConnection::fetchEvents(std::vector<Event::GameEvent*> &list)
{
	eventMutex->Lock();
	list=eventList;
	eventList.resize(0);
	eventMutex->Unlock();
}
void NetworkConnection::fetchEffects(std::vector<Network::EffectData> &list)
{
	effectMutex->Lock();
	list=effectList;
	//effectList.resize(0);
	effectMutex->Unlock();
}

void NetworkConnection::receiveGameInitData(char* data)
{
	GameInitData init;
	int c=sizeof(init);
	memcpy(&init, data, sizeof(init));
	id=init.pid;

	for(unsigned int i=0; i<PLAYER_MAX_COUNT; i++)
	{
		if (intoTheVoid)
			intoTheVoid->SetPlayerPosition(i, init.player[i].position);
		teamID[i]=init.player[i].teamid;
		positions[i]=init.player[i].position;
	}
	connStatus=ONLINE_INGAME;
}

char* NetworkConnection::getPlayerData()
{
	return "Not yet implemented";
}
void NetworkConnection::sendData(ClientToServerUpdateData &data)
{
	if(!gameOver)
	{
		data.pid=id;
		data.updateCount=curClientUpdate;
		curClientUpdate++;
		sendUserData((char*)&data, sizeof(data));
	}
}

