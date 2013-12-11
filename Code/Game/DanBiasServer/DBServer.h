/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef DANBIASSERVER_DBSERVER_H
#define DANBIASSERVER_DBSERVER_H

#include "Include\IDanBiasServer.h"

namespace DanBias
{
	class DBServer
	{
	public:
		DBServer();
		~DBServer();

		DanBiasServerReturn Create(const DanBias::DanBiasServerDesc& desc);
		DanBiasServerReturn Run();
		DanBiasServerReturn Release();

	private:
		bool initiated;
		bool running;
		bool released;
	};
}// End namspace DanBias
#endif // !DANBIASSERVER_DBSERVER_H
