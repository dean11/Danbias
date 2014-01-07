#ifndef NETWORK_SERVER_LISTENER_H
#define NETWORK_SERVER_LISTENER_H

/////////////////////////////////////
// Created by Pontus Fransson 2013 //
/////////////////////////////////////

#include "IListener.h"
#include "Connection.h"
#include "IPostBox.h"
#include "../../Misc/Thread/OysterThread.h"
#include "../../Misc/Thread/OysterMutex.h"
#include "../../Misc/Utilities.h"

namespace Oyster
{
	namespace Network
	{
		namespace Server
		{
			class Listener : public IListener, public ::Oyster::Thread::IThreadObject
			{
			public:
				Listener();
				Listener(Oyster::Network::IPostBox<int>* postBox);
				~Listener();

				bool Init(unsigned int port) override;
				bool Init(unsigned int port, bool start);
				bool Start();
				void Stop();
				void Shutdown();

				void SetPostBox(IPostBox<int>* postBox);

			private:
				//Thread functions
				bool DoWork();
				void ThreadEntry();
				void ThreadExit();

				//Function that runs in the thread.
				int Accept();

			private:
				::Oyster::Network::Connection* connection;

				::Oyster::Thread::OysterThread thread;
				OysterMutex mutex;
				std::mutex stdMutex;

				IPostBox<int>* postBox;

			};
		}
	}
}

#endif