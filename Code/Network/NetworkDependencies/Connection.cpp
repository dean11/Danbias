#include "Connection.h"

#include <winsock2.h>
#include <iostream>

using namespace Oyster::Network; 

Connection::~Connection()
{
	mySocket = NULL;

	if(socket != NULL)
	{
		closesocket( mySocket );
		mySocket = NULL;
	}
}

bool Connection::Connect(unsigned short port , const char serverName[])
{
	mySocket = socket(AF_INET, SOCK_STREAM, 0);
	if(mySocket == SOCKET_ERROR)
	{
		 //error opening socket
		return false;
	}


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

	while(1)
	{
		if(connect(mySocket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
		{
			//Error connecting to server
			return false;
		}

		else
		{
			break;
		}
		Sleep(10);
	}
	
	//connection succesfull!
	return true;
}

bool Connection::InitiateServer(unsigned short port)
{
	mySocket = socket(AF_INET, SOCK_STREAM, 0);
	if(mySocket == SOCKET_ERROR)
	{
		//Error opening socket!
		return false;
	}

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;

	if(bind(mySocket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		//Bind failed!;
		closesocket(mySocket);
		return false;
	}

	//not our Listen function! its trying to keep our socket open for connections
	if(listen(mySocket, 5) == SOCKET_ERROR)
	{
		//"Listen failed!
		closesocket(mySocket);
		return -1;
	}

	//Server started!
	return mySocket;
}

void Connection::Disconnect()
{
	closesocket(mySocket);
}

bool Connection::Send(const unsigned char message[])
{
	int nBytes;
	unsigned long messageSize = strlen((char*)message);
<<<<<<< HEAD
	if((nBytes = send(mySocket, (char*)message , messageSize, 0)) == SOCKET_ERROR)
=======
	messageSize = 18;
	nBytes = send(mySocket, (char*)message , messageSize, 0);
	if(nBytes == SOCKET_ERROR)
>>>>>>> 4142688f6c4a63aa97341205588ad6cace0f43af
	{
		//Send failed!
		return false;
	}

	return true; 
}

int Connection::Recieve(unsigned char message[])
{
	int nBytes;
	nBytes = recv(mySocket, (char*)message , 255, 0);
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
	if((clientSocket = accept(mySocket, NULL, NULL)) == INVALID_SOCKET)
	{
		//failed
		return -1;
	}

	return clientSocket;
}
