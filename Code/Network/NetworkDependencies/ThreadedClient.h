#ifndef NETWORK_DEPENDENCIES_THREADED_CLIENT_H
#define NETWORK_DEPENDENCIES_THREADED_CLIENT_H

//////////////////////////////////
// Created by Sam Svensson 2013 //
//////////////////////////////////

#include "../NetworkDependencies/IClient.h"
#include "../../Misc/Thread/IThreadObject.h"
#include "../NetworkDependencies/PostBox.h"
#include "../../Misc/Thread/OysterThread.h"
#include "../../Misc/Thread/OysterMutex.h"
#include "../../Misc/Utilities.h"

namespace Oyster
{
	namespace Network
	{
		class ThreadedClient : public IClient, public Thread::IThreadObject
		{
		public:
			ThreadedClient();
			ThreadedClient(unsigned int socket);
			ThreadedClient(IPostBox<Utility::DynamicMemory::SmartPointer< OysterByte >> *postBox, unsigned int socket);
			virtual ~ThreadedClient();

			int Send(Utility::DynamicMemory::SmartPointer< OysterByte >& byte);

			int Connect(unsigned short port, const char serverName[]);

			void setRecvPostBox(IPostBox< Utility::DynamicMemory::SmartPointer< OysterByte >> *postBox);

		private:
			virtual int Send();
			virtual int Recv();

			virtual void ThreadEntry();
			virtual void ThreadExit();
			virtual bool DoWork();

		private:
			Connection* connection;
			IPostBox<Utility::DynamicMemory::SmartPointer<OysterByte>> *sendPostBox;
			IPostBox<Utility::DynamicMemory::SmartPointer<OysterByte>> *recvPostBox;
			Oyster::Thread::OysterThread thread;
			OysterMutex mutex;

		};
	}
}

#endif