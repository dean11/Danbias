#include "Connection.h"

#include <winsock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <string>
#include <fcntl.h>

using namespace Oyster::Network;

int CloseSocket(int &socket)
{
	if(socket == -1) return 0;

	if(closesocket( socket ) == SOCKET_ERROR)
	{
		return WSAGetLastError();
	}

	socket = -1;

	return 0;
}

Connection::Connection()
{
	this->socket = -1;
	this->stillSending = false;
	this->closed = true;
}

Connection::Connection(int socket)
{
	this->socket = socket;
	this->stillSending = true;
	this->closed = false;
}

Connection::~Connection()
{
	CloseSocket( this->socket );
}

int Connection::Connect(ConnectionInfo info, bool blocking)
{
	this->addr = info.addr;
	this->socket = info.socket;
	this->stillSending = true;
	this->closed = false;
	

	SetBlockingMode(blocking);
	//connection succesfull!
	return 0;
}

int Connection::Connect(unsigned short port , const char serverName[], bool blocking)
{
	if(this->socket == -1 || this->socket == 0) InitiateSocket();

	lastConnectPort = port;
	lastConnectAddr = serverName;

	struct hostent *hostEnt;
	if((hostEnt = gethostbyname(serverName)) == NULL)
	{
		return WSAGetLastError();
	}

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = *(unsigned long*) hostEnt->h_addr;

	if(connect(this->socket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		return WSAGetLastError();
	}
	
	closed = false;
	stillSending = true;

	SetBlockingMode(blocking);

	//connection succesfull!
	return 0;
}

int Connection::Reconnect()
{
	if(this->socket == -1 || this->socket == 0) InitiateSocket();

	struct hostent *hostEnt;
	if((hostEnt = gethostbyname(this->lastConnectAddr.c_str())) == NULL)
	{
		return WSAGetLastError();
	}

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(this->lastConnectPort);
	server.sin_addr.s_addr = *(unsigned long*) hostEnt->h_addr;

	SetBlockingMode(true);

	if(connect(this->socket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		return WSAGetLastError();
	}
	
	closed = false;
	stillSending = true;

	SetBlockingMode(blocking);

	//connection succesfull!
	return 0;
}

int Connection::InitiateServer(unsigned short port)
{
	int errorCode = 0;

	if((errorCode = InitiateSocket()) != 0)
	{
		return errorCode;
	}

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;

	if(bind(this->socket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		errorCode = WSAGetLastError();
		CloseSocket(this->socket);
		return errorCode;
	}

	//not our Listen function! its trying to keep our socket open for connections
	if(listen(this->socket, 5) == SOCKET_ERROR)
	{
		errorCode = WSAGetLastError();
		CloseSocket(this->socket);
		return errorCode;
	}

	closed = false;
	stillSending = true;

	//Server started!
	return 0;
}

int Connection::InitiateClient()
{
	return InitiateSocket();
}

int Connection::Disconnect()
{
	if(this->closed) return -1;
	int val = CloseSocket(this->socket);
	this->socket = -1;
	this->closed = true;
	this->stillSending = false;
	return val;
}

int Connection::Send(OysterByte &bytes)
{
	if(this->closed) return -1;

	int nBytes;

	nBytes = send(this->socket, bytes, bytes.GetSize(), 0);
	if(nBytes == SOCKET_ERROR)
	{
		return WSAGetLastError();
	}

	return 0; 
}

int Connection::Recieve(OysterByte &bytes)
{
	if(this->closed) return -1;
	int nBytes;

	bytes.Resize(MAX_NETWORK_MESSAGE_SIZE);
	nBytes = recv(this->socket, bytes, MAX_NETWORK_MESSAGE_SIZE, 0);
	if(nBytes == SOCKET_ERROR)
	{
		bytes.SetSize(0);
		return WSAGetLastError();
	}
	else
	{
		bytes.SetSize(nBytes);
	}

	return 0;
}

//Listen will only return the correct socket or -1 for failure.
ConnectionInfo Connection::Listen()
{
	ConnectionInfo val = { 0 };
	if(this->closed) return val;

	SOCKADDR_IN client_info = { 0 };
	int addrsize = sizeof(client_info);

	if((val.socket = (int)accept(this->socket, (struct sockaddr*)&client_info, &addrsize)) == INVALID_SOCKET)
	{
		val.socket = WSAGetLastError();
	}
	else
	{
		val.addr = inet_ntoa(client_info.sin_addr);
	}

	return val;
}

bool Connection::IsSending()
{
	return stillSending;
}

bool Connection::IsConnected()
{
	return !closed;
}

int Connection::SetBlockingMode(bool blocking)
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
		return WSAGetLastError();
	}

	//Success
	return 0;
}


std::string	Connection::GetIpAddress()
{
	return this->addr;
}

int Connection::GetSocket()
{
	return socket;
}

///////////////////////////////////////
//Private functions
///////////////////////////////////////
int Connection::InitiateSocket()
{
	if(this->socket != -1) return 0;

	this->socket = (int)::socket(AF_INET, SOCK_STREAM, 0);
	if(this->socket == SOCKET_ERROR)
	{
		return WSAGetLastError();
	}

	int flag = 1;
	int result = setsockopt(this->socket,		/* socket affected */
							IPPROTO_TCP,		/* set option at TCP level */
							TCP_NODELAY,		/* name of option */
							(char *) &flag,		/* the cast is historical cruft */
							sizeof(int));		/* length of option value */
	if (result < 0)
		return -1;

	return 0;
}