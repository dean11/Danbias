#ifndef NETWORK_SERVER_LISTENER_H
#define NETWORK_SERVER_LISTENER_H

/////////////////////////////////////
// Created by Pontus Fransson 2013 //
/////////////////////////////////////

#include "IListener.h"
#include "../NetworkDependencies/Connection.h"
#include "../../Misc/Thread/OysterThread.h"
#include "../../Misc/Thread/OysterMutex.h"
#include "../../Misc/Utilities.h"
#include "IPostBox.h"

namespace Oyster
{
	namespace Network
	{
		namespace Server
		{
			class Listener : public ::Oyster::Thread::IThreadObject
			{
			public:
				Listener();
				~Listener();

				bool Init(unsigned int port);
				void Shutdown();

				void SetPostBox(IPostBox<Utility::DynamicMemory::SmartPointer<int>>* postBox);

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

				IPostBox<Utility::DynamicMemory::SmartPointer<int>>* postBox;

			};
		}
	}
}

#endif