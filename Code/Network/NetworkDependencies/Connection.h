//////////////////////////////////
// Created by Sam Svensson 2013 //
//////////////////////////////////

#ifndef NETWORK_DEPENDENCIES_CONNECTION_H
#define NETWORK_DEPENDENCIES_CONNECTION_H

#include "IConnection.h"

namespace Oyster
{
	namespace Network
	{
		class Connection : public IConnection
		{

		public:
			Connection()           { mySocket = 0; };
			Connection(int socket) { mySocket = socket; };
			~Connection();

			virtual bool Connect( unsigned short port , const char serverName[] );
			virtual bool InitiateServer( unsigned short port );

			virtual void Disconnect();
<<<<<<< HEAD
			virtual bool Send(const unsigned char message[]);
			virtual int  Recieve(char unsigned message[]);
=======

			virtual bool Send(const unsigned char message[]);
			virtual int  Recieve(unsigned char message[]);

>>>>>>> 4142688f6c4a63aa97341205588ad6cace0f43af
			virtual int  Listen();

		private:
			int mySocket;

		};
	}
}

#endif