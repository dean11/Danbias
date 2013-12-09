#ifndef OYSTER_NETWORK_SERVER_I_SERVER_H
#define OYSTER_NETWORK_SERVER_I_SERVER_H

/////////////////////////////////////
// Created by Pontus Fransson 2013 //
/////////////////////////////////////

#include "IClient.h"

namespace Oyster
{
	namespace Network
	{
		namespace Server
		{
			class IServer
			{
			public:
				struct INIT_DESC
				{
					unsigned short port;		//Port the server should be accepting clients on.
					void (*proc)(IClient*);
				};

				IServer();
				virtual ~IServer();

				virtual bool Init(INIT_DESC& initDesc);
				virtual bool Start();
				virtual bool Stop();
				virtual bool Shutdown();

				virtual bool IsStarted() const;

			private:
				struct PrivateData;
				PrivateData* privateData;

			};
		}
	}
}

#endif