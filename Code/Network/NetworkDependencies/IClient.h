#ifndef NETWORK_DEPENDENCIES_I_CLIENT_H
#define NETWORK_DEPENDENCIES_I_CLIENT_H

//////////////////////////////////
// Created by Sam Svensson 2013 //
//////////////////////////////////

#include "../NetworkDependencies/Connection.h"
#include "../NetworkDependencies/OysterByte.h" 

namespace Oyster
{
	namespace Network
	{
		class IClient
		{

		public:
			virtual ~IClient() {};
			virtual int Send() = 0;
			virtual int Recv() = 0; 

		private:

		};
	}
}
#endif