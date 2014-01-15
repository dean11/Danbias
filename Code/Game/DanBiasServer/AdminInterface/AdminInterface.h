/////////////////////////////////////////
// Created by [Dennis Andersen] [2013] //
/////////////////////////////////////////
#ifndef DANBIASSERVER_ADMIN_INTERFACE_H
#define DANBIASSERVER_ADMIN_INTERFACE_H

#include <NetworkServer.h>

namespace DanBias
{
	class NetworkSession;
	//Global admin
	class AdminInterface
	{
	public:
		static void Toggle(bool toggle, NetworkSession* parent);

	};
}
#endif // !DANBIASSERVER_ADMIN_INTERFACE_H
