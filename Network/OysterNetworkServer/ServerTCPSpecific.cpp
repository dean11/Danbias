#include "SocketServer.h"
bool SocketServer::initTCPSocket()
{
	//----------------------
	// Create a SOCKET for listening for incoming connection requests.
	TCPSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (TCPSocket == INVALID_SOCKET) {
		wprintf(L"TCP socket function failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return false;
	}

	iResult = bind(TCPSocket, (SOCKADDR *) & TCPRecvAddr, addrSize);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"TCP bind function failed with error %d\n", WSAGetLastError());
		iResult = closesocket(TCPSocket);
		if (iResult == SOCKET_ERROR)
			wprintf(L"TCP closesocket function failed with error %d\n", WSAGetLastError());
		WSACleanup();
		return false;
	}
	return true;
}
DWORD SocketServer::activateTCPConnectLoop(ThreadArguments* tra)
{
	while (true)
	{
		(tra->ptr)->receiveConnection(tra->threadID);
	}
}
void SocketServer::receiveConnection(int threadID)
{
	User tmp;
	//----------------------
	// Listen for incoming connection requests 
	// on the created socket
	if (listen(TCPSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		wprintf(L"listen function failed with error: %d\n", WSAGetLastError());
		return;
	}

	printf("Starting TCP connection loop.\n");
	int a=0;
	while(a==0)
	{
		a=1;
		tmp.connection=accept(TCPSocket, (struct sockaddr*)&TCPRecvAddr, &addrSize);
		printf("Accepted a TCP connection from IP %s.\n", inet_ntoa(TCPRecvAddr.sin_addr));
		tcpData[threadID].dataSize=recv(
			tmp.connection, 
			tcpData[threadID].buffer, 
			tcpData[threadID].bufLen, 
			0);
		connData[threadID].buffer[connData[threadID].dataSize]='\0';
		tmp.setUsername(tcpData[threadID].buffer);
		if (tcpData[threadID].dataSize == SOCKET_ERROR) 
		{
			wprintf(L"TCP recv failed with error %d\n", WSAGetLastError());
		}
		printf("TCP Thread #%d received connData from %s\n", threadID, inet_ntoa(tcpData[threadID].srcAddr.sin_addr));
		//connData[threadID].buffer[connData[threadID].dataSize]='\0';
		//AddUser(&tcpData[threadID]);
		//parseReceivedData(threadID);
	}
}