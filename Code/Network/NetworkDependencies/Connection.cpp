#include "Connection.h"

Connection::Connection()
{
	mySocket = NULL;
}

Connection::Connection(int socket)
{
	mySocket = socket;
}

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


	struct hostent *hostEntry;
	if((hostEntry = gethostbyname(serverName)) == NULL)
	{
		//couldn't find host
		return false;
	}
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = *(unsigned long*) hostEntry->h_addr;

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
	int mySocket = socket(AF_INET, SOCK_STREAM, 0);
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

	//not our Listen function!
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

bool Connection::Send(int socket , const char message[])
{
	int nBytes;
	unsigned long messageSize = strlen(message);

	if((nBytes = send(socket, message , messageSize)) == SOCKET_ERROR)
	{
		//Send failed!
		return false;
	}

	return true; 
}

int Recieve(int socket, char message[])
{
	int nBytes;
	nBytes = recv(socket, message , 255, 0);
	if(nBytes == SOCKET_ERROR)
	{
		//Recv failed
		return -1;
	}

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
