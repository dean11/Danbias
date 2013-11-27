#include "Connection.h"

#include <winsock2.h>
#include <iostream>
#include <string>

using namespace Oyster::Network;

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

	//Server started!
	return 0;
}

int Connection::InitiateClient()
{
	return InitiateSocket();
}

int Connection::Disconnect()
{
	closesocket(this->socket);

	return WSAGetLastError();
}

int Connection::Send(OysterByte& bytes)
{
	int nBytes;

	nBytes = send(this->socket, bytes, bytes.GetSize(), 0);
	if(nBytes == SOCKET_ERROR)
	{
		return WSAGetLastError();
	}

	return 0; 
}

int Connection::Recieve(OysterByte& bytes)
{
	int nBytes;

	bytes.Clear(1000);
	nBytes = recv(this->socket, bytes, 500, 0);
	if(nBytes == SOCKET_ERROR)
	{
		return WSAGetLastError();
	}
	else
	{
		bytes.SetSize(nBytes);
	}

	std::cout << "Size of the recieved data: " << nBytes << " bytes" << std::endl;

	//bytes.byteArray[nBytes] = '\0';

	return 0;
}

int Connection::Listen()
{
	int clientSocket;
	if((clientSocket = accept(this->socket, NULL, NULL)) == INVALID_SOCKET)
	{
		return WSAGetLastError();
	}

	return clientSocket;
}

///////////////////////////////////////
//Private functions
///////////////////////////////////////
int Connection::InitiateSocket()
{
	this->socket = ::socket(AF_INET, SOCK_STREAM, 0);
	if(this->socket == SOCKET_ERROR)
	{
		return WSAGetLastError();
	}

	return 0;
}
