/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#include "DanBiasServerAPI.h"
#include "..\GameServer.h"

namespace DanBias
{

#pragma region Server Data
	static GameServer server;
#pragma endregion


	DanBiasServerReturn DanBiasServerAPI::Initiate()
	{
		return server.Create();
	}
	DanBiasServerReturn DanBiasServerAPI::Run()
	{
		return server.Run();
	}
	DanBiasServerReturn DanBiasServerAPI::Release()
	{
		return server.Release();
	}

} //End namspace DanBias