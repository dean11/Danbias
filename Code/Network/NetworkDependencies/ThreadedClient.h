#ifndef NETWORK_DEPENDENCIES_THREADED_CLIENT_H
#define NETWORK_DEPENDENCIES_THREADED_CLIENT_H

//////////////////////////////////
// Created by Sam Svensson 2013 //
//////////////////////////////////

#include "../../Misc/Thread/IThreadObject.h"
#include "PostBox.h"
#include "Connection.h"
#include "../../Misc/Thread/OysterThread.h"
#include "../../Misc/Thread/OysterMutex.h"
#include "../../Misc/Utilities.h"

#include <mutex>

namespace Oyster
{
	namespace Network
	{
		class OysterByte;
		class ThreadedClient : public Thread::IThreadObject
		{
		public:
			ThreadedClient();
			ThreadedClient(unsigned int socket);
			ThreadedClient(IPostBox<Utility::DynamicMemory::SmartPointer<OysterByte>> *postBox, unsigned int socket);
			virtual ~ThreadedClient();

			void Send(Utility::DynamicMemory::SmartPointer<OysterByte>& byte);

			int Connect(unsigned short port, const char serverName[]);

			void setRecvPostBox(IPostBox<Utility::DynamicMemory::SmartPointer<OysterByte>> *postBox);

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
			std::mutex stdMutex;

		};
	}
}

#endif