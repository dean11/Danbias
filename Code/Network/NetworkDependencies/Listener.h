#ifndef NETWORK_SERVER_LISTENER_H
#define NETWORK_SERVER_LISTENER_H

/////////////////////////////////////
// Created by Pontus Fransson 2013 //
/////////////////////////////////////

#include "IListener.h"
#include "../NetworkDependencies/Connection.h"

namespace Oyster
{
	namespace Network
	{
		namespace Server
		{
			class Listener
			{
			public:
				Listener();
				~Listener();

				bool Init(unsigned int port);
				int Accept();

			private:
				::Oyster::Network::Connection* connection;

			};
		}
	}
}

#endif