#ifndef NETWORK_SERVER_ILISTENER_H
#define NETWORK_SERVER_ILISTENER_H

/////////////////////////////////////
// Created by Pontus Fransson 2013 //
/////////////////////////////////////

namespace Oyster
{
	namespace Network
	{
		namespace Server
		{
			class IListener
			{
			public:
				virtual bool Init(unsigned int port) = 0;
				virtual int Accept() = 0;

			};
		}
	}
}

#endif