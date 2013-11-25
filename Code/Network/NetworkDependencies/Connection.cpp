#include "Connection.h"

#include <winsock2.h>
#include <iostream>

using namespace Oyster::Network;

Connection::~Connection()
{
	closesocket( this->socket );
}

bool Connection::Connect(unsigned short port , const char serverName[])
{
	struct hostent *hostEnt;
	if((hostEnt = gethostbyname(serverName)) == NULL)
	{
		//couldn't find host
		return false;
	}

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = *(unsigned long*) hostEnt->h_addr;

	if(connect(this->socket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		//Error connecting to server
		return false;
	}
	
	//connection succesfull!
	return true;
}

bool Connection::InitiateServer(unsigned short port)
{
	this->socket = ::socket(AF_INET, SOCK_STREAM, 0);
	if(this->socket == SOCKET_ERROR)
	{
		//Error opening socket!
		return false;
	}

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;

	if(bind(this->socket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		//Bind failed!;
		closesocket(this->socket);
		return false;
	}

	//not our Listen function! its trying to keep our socket open for connections
	if(listen(this->socket, 5) == SOCKET_ERROR)
	{
		//"Listen failed!
		closesocket(this->socket);
		return false;
	}

	//Server started!
	return true;
}

bool Connection::InitiateClient()
{
	this->socket = ::socket(AF_INET, SOCK_STREAM, 0);
	if(this->socket == SOCKET_ERROR)
	{
		 //error opening socket
		return false;
	}

	return true;
}

void Connection::Disconnect()
{
	closesocket(this->socket);
}

bool Connection::Send(const unsigned char message[])
{
	int nBytes;
	unsigned long messageSize = strlen((char*)message);

	messageSize = 255;
	nBytes = send(this->socket, (char*)message , messageSize, 0);
	if(nBytes == SOCKET_ERROR)
	{
		//Send failed!
		return false;
	}

	return true; 
}

int Connection::Recieve(unsigned char message[])
{
	int nBytes;
	nBytes = recv(this->socket, (char*)message , 255, 0);
	if(nBytes == SOCKET_ERROR)
	{
		//Recv failed
		return -1;
	}

	message[nBytes] = '\0';

	return 1;
}

int Connection::Listen()
{
	int clientSocket;
	if((clientSocket = accept(this->socket, NULL, NULL)) == INVALID_SOCKET)
	{
		//failed
		return -1;
	}

	return clientSocket;
}
