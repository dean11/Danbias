#ifndef NETWORK_SERVER_LISTENER_H
#define NETWORK_SERVER_LISTENER_H

/////////////////////////////////////
// Created by Pontus Fransson 2013 //
/////////////////////////////////////

#include "Connection.h"
#include "IPostBox.h"
#include "Thread/OysterThread.h"
#include "Thread/OysterMutex.h"
#include "Utilities.h"
#include <atomic>

namespace Oyster
{
	namespace Network
	{
		class Listener : public ::Oyster::Thread::IThreadObject
		{
		public:
			Listener();
			Listener(Oyster::Network::IPostBox<ConnectionInfo>* postBox);
			~Listener();

			bool Init(unsigned int port);
			bool Init(unsigned int port, bool start);
			bool Start();
			void Stop();
			void Shutdown();

			void SetPostBox(IPostBox<ConnectionInfo>* postBox);

		private:
			//Thread functions
			bool DoWork();
			void ThreadEntry();
			Thread::IThreadObject::ThreadCode ThreadExit();

			//Function that runs in the thread.
			int Accept();
			void StopListen();

		private:
			::Oyster::Network::Connection* connection;

			::Oyster::Thread::OysterThread thread;
			OysterMutex mutex;
			std::mutex stdMutex;

			IPostBox<ConnectionInfo>* postBox;
			std::atomic<bool> isListening;
			int port;
		};

	}
}

#endif