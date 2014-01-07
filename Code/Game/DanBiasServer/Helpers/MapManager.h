#ifndef DANBIASSERVER_LEVELMANAGER_H
#define DANBIASSERVER_LEVELMANAGER_H

#include "ServerDataReader.h"
#include "..\LobbySessions\LobbyClient.h"
#include "..\LobbySessions\NetworkSession.h"
#include "..\LobbySessions\GameLobby.h"
#include <Utilities.h>

namespace DanBias
{
	class MapManager
	{
	public:
		struct MapInitDesc
		{
			const wchar_t* map;
			Utility::DynamicMemory::DynamicArray<Utility::DynamicMemory::SmartPointer<LobbyClient>> clients;
		};

	public:
		static bool InitiateMapPack(const MapInitDesc& desc);

	};
}

#endif // !DANBIASSERVER_LEVELMANAGER_H
