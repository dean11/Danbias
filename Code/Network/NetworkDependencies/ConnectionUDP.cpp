#include "ConnectionUDP.h"

#include <WinSock2.h>

using namespace Oyster::Network;

ConnectionUDP::ConnectionUDP()
{
	this->ipSize = 16;
	this->socket = -1;
	this->stillSending = false;
	this->closed = true;
	this->Address = NULL;
}

ConnectionUDP::ConnectionUDP( int socket )
{
	this->ipSize = 16;
	this->socket = socket;
	this->stillSending = false;
	this->closed = true;
	this->Address = NULL;
}

ConnectionUDP::~ConnectionUDP()
{
	closesocket( this->socket );
}

int ConnectionUDP::Connect(unsigned short port, const char serverName[])
{
	this->port = port;
	
	closed = false;
	stillSending = true;

	struct hostent *hostEnt;
	if((hostEnt = gethostbyname(serverName)) == NULL)
	{
		return SOCKET_ERROR;
	}

	
	this->Address = *(unsigned long*)hostEnt->h_addr;
	this->port = htons(this->port);
	return 0;
}

int ConnectionUDP::Disconnect()
{
	int result = closesocket(this->socket);

	if(result == SOCKET_ERROR)
	{
		return result;
	}

	return 0;
}

int ConnectionUDP::InitiateServer(unsigned short port)
{
	int errorCode = 0;

	if((errorCode = InitiateSocket()) != 0)
	{
		return errorCode;
	}

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);

	if(bind(this->socket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		errorCode = SOCKET_ERROR;
		closesocket(this->socket);
		return errorCode;
	}

	closed = false;
	stillSending = true;
	return 0;
}

int ConnectionUDP::InitiateClient()
{
	return InitiateSocket();
}

int ConnectionUDP::InitiateBroadcast(unsigned short port)
{
	int result = InitiateSocket();


	int flag = 1;
	result = setsockopt(this->socket,			/* socket affected */
							SOL_SOCKET,			/* set option at TCP level */
							SO_BROADCAST,		/* name of option */
							(char *) &flag,		/* the cast is historical cruft */
							sizeof(flag));		/* length of option value */
	if (result < 0)
		return -1;
}

int ConnectionUDP::Send(OysterByte &bytes)
{
	int nBytes;
	struct sockaddr_in reciever;
	reciever.sin_family = AF_INET;
	reciever.sin_addr.s_addr = this->Address;
	reciever.sin_port = port;

	//printf("Send: %d\n", bytes.GetSize());
	nBytes = sendto(this->socket,
					bytes,
					bytes.GetSize(),
					0,
					(sockaddr*)&reciever, 
					sizeof(sockaddr_in)
					); 

	if ( nBytes != bytes.GetSize() )
    {
		return nBytes;
    }

	return 0;
}

int ConnectionUDP::Recieve(OysterByte &bytes)
{
	int nBytes;
	sockaddr_in from;
    int fromLength = sizeof( from );

	bytes.Resize(1000);
	nBytes = recvfrom(this->socket,
					  bytes,
					  1000,
					  0,
					  (sockaddr*)&from,
					  &fromLength 
					  );


	if(nBytes <= 0)
	{
		bytes.SetSize(0);
		return WSAGetLastError();
	}
	else
	{
		bytes.SetSize(nBytes);
	}


	//address and port of the client who sent the message
	unsigned int from_address = ntohl( from.sin_addr.s_addr );
    unsigned int from_port = ntohs( from.sin_port );


	return 0;
}

bool ConnectionUDP::IsSending()
{
	return stillSending;
}

bool ConnectionUDP::IsConnected()
{
	if(closed)
	{
		return false;
	}

	return true;
}

int ConnectionUDP::SetBlockingMode( bool blocking )
{
	DWORD nonBlocking;

	if(blocking)
	{
		nonBlocking = 0;
	}
	else
	{
		nonBlocking = 1;
	}

	int result = ioctlsocket(this->socket, FIONBIO, &nonBlocking);
	if(result != 0)
	{
		return result;
	}

	//Success
	return 0;
}
//////////////////////////////////////
// Private Methods
//////////////////////////////////////

int ConnectionUDP::InitiateSocket()
{
	this->socket = (int)::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if(this->socket == SOCKET_ERROR)
	{
		return socket;
	}

	return 0;
}