/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#include "Include\IDanBiasServer.h"
#include "DBServer.h"

namespace DanBias
{

#pragma region Server Data
	static DBServer server;
#pragma endregion


	DanBiasServerReturn DanBiasServer::Initiate(DanBiasServerDesc& desc)
	{
		return server.Create(desc);
	}
	DanBiasServerReturn DanBiasServer::Run()
	{
		return server.Run();
	}
	DanBiasServerReturn DanBiasServer::Release()
	{
		return server.Release();
	}

} //End namspace DanBias