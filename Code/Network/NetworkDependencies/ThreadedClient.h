#ifndef NETWORK_DEPENDENCIES_THREADED_CLIENT_H
#define NETWORK_DEPENDENCIES_THREADED_CLIENT_H

//////////////////////////////////
// Created by Sam Svensson 2013 //
//////////////////////////////////

#include "Thread/IThreadObject.h"
#include "PostBox.h"
#include "Connection.h"
#include "Thread/OysterThread.h"
#include "Utilities.h"

#include <mutex>

namespace Oyster
{
	namespace Network
	{
		class CustomNetProtocol;
		class OysterByte;
		class ThreadedClient : public Thread::IThreadObject
		{
		public:
			ThreadedClient();
			ThreadedClient(unsigned int socket);
			ThreadedClient(IPostBox<CustomNetProtocol*> *postBox, unsigned int socket);
			virtual ~ThreadedClient();

			void Send(CustomNetProtocol* protocol);

			bool IsConnected();

			int Connect(unsigned short port, const char serverName[]);

			void Disconnect();

			void setRecvPostBox(IPostBox<CustomNetProtocol*> *postBox);

		protected:
			virtual int Send();
			virtual int Recv();

			//These functions should not be called by any other than the thread.
			virtual void ThreadEntry();
			virtual Thread::IThreadObject::ThreadCode ThreadExit();
			virtual bool DoWork();

		protected:
			Connection* connection;
			IPostBox<CustomNetProtocol*> *sendPostBox;
			IPostBox<CustomNetProtocol*> *recvPostBox;
			Oyster::Thread::OysterThread thread;
			std::mutex stdMutex;

		};
	}
}

#endif