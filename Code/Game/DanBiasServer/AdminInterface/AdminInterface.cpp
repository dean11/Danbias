#define NOMINMAX
#include "AdminInterface.h"
#include <Thread\OysterThread.h>
#include <WindowShell.h>
#include "..\GameServer.h"
#include "..\GameSession\GameSessionManager.h"
#include "..\GameSession\GameSession.h"
#include "..\Helpers\ServerDataReader.h"
#include "..\LobbySessions\LobbyClient.h"
#include "..\LobbySessions\NetworkSession.h"
#include "..\LobbySessions\GameLobby.h"
#include "..\LobbySessions\MainLobby.h"

using namespace Oyster::Network;
using namespace Oyster::Thread;
using namespace DanBias;

struct AdminInstanceData	:public IThreadObject
{
	bool isCreated;
	Oyster::Network::NetworkServer adminServer;
	NetworkClient* admin;
	NetworkSession* parentInstance;
	OysterThread worker;

	bool DoWork() override
	{
		//((GameServer*)parentInstance)->serve
		return true;
	}

}adminInstanceData;

void AdminArrived(NetworkClient* adm)
{
	if(adminInstanceData.admin)
	{
		delete adm;
		return;
	}

	adminInstanceData.admin = adm;
	adminInstanceData.worker.Create(&adminInstanceData, true, true);
}

void AdminInterface::Toggle(bool toggle, NetworkSession* parent)
{
	if(toggle)
	{
		if(!parent)	return;

		if(adminInstanceData.isCreated) return;

		NetworkServer::INIT_DESC desc;
		desc.port = 15152;
		desc.callbackType = NetworkClientCallbackType_Function;
		desc.recvObj.clientConnectFnc = AdminArrived;
	
		if(!adminInstanceData.adminServer.Init(desc)) return;
		adminInstanceData.parentInstance = parent;
	
		adminInstanceData.adminServer.Start();
	}
	else
	{
		adminInstanceData.adminServer.Shutdown();
	}
}