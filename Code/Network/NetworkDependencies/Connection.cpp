#include "Connection.h"

#include <winsock2.h>
#include <iostream>
#include <string>
#include <fcntl.h>

using namespace Oyster::Network;

Connection::Connection()
{
	this->socket = -1;
	bool stillSending = false;
	bool closed = true;
}

Connection::Connection(int socket)
{
	this->socket = socket;
	bool stillSending = false;
	bool closed = true;
}

Connection::~Connection()
{
	closesocket( this->socket );
}

int Connection::Connect(unsigned short port , const char serverName[])
{
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
		closesocket(this->socket);
		return errorCode;
	}

	//not our Listen function! its trying to keep our socket open for connections
	if(listen(this->socket, 5) == SOCKET_ERROR)
	{
		errorCode = WSAGetLastError();
		closesocket(this->socket);
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
	int result = closesocket(this->socket);

	if(result == SOCKET_ERROR)
	{
		return WSAGetLastError();
	}

	return 0;
}

int Connection::Send(OysterByte &bytes)
{
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
	int nBytes;

	bytes.Resize(1000);
	nBytes = recv(this->socket, bytes, 1000, 0);
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
int Connection::Listen()
{
	int clientSocket;
	if((clientSocket = (int)accept(this->socket, NULL, NULL)) == INVALID_SOCKET)
	{
		return (int)INVALID_SOCKET;//WSAGetLastError();
	}

	return clientSocket;
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

///////////////////////////////////////
//Private functions
///////////////////////////////////////
int Connection::InitiateSocket()
{
	this->socket = (int)::socket(AF_INET, SOCK_STREAM, 0);
	if(this->socket == SOCKET_ERROR)
	{
		return WSAGetLastError();
	}

	return 0;
}