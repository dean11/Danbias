#include "SocketServer.h"
bool SocketServer::initUDPSocket()
{
	//---------------------------------------------
	// Create a socket for sending data
	UDPSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (UDPSocket == INVALID_SOCKET) {
		wprintf(L"UDP socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return false;
	}
	//---------------------------------------------
	// Bind socket to IP
	iResult = bind(UDPSocket, (SOCKADDR *) & UDPRecvAddr, addrSize);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"UDP bind failed with error: %d\n", WSAGetLastError());
		closesocket(UDPSocket);
		WSACleanup();
		return false;
	}
	return true;
}
DWORD SocketServer::activateUDPReceiveLoop(ThreadArguments* tra)
{
	(tra->ptr)->serverUDPReceiveLoopActive=true;//weird crash //PAR
	(tra->ptr)->receiveDataUDP(tra->threadID);
	return 0;
}
void SocketServer::stopUDPReceiveLoops()
{
	serverUDPReceiveLoopActive=false;
	WaitForMultipleObjects(NR_CONNECTTHREADS, udpDataHandle, true, INFINITE);
	printf("All UDP data recv threads stopped.\n");
}
void SocketServer::receiveDataUDP(int threadID)
{
	while(serverUDPReceiveLoopActive)
	{
	connData[threadID].dataSize=recvfrom(
		UDPSocket, 
		connData[threadID].buffer, 
		connData[threadID].bufLen, 
		0, 
		(SOCKADDR *)&connData[threadID].srcAddr, 
		&addrSize);
	if (connData[threadID].dataSize == SOCKET_ERROR) 
	{
		wprintf(L"recvfrom failed with error %d\n", WSAGetLastError());
	}
	//printf("Thread #%d received data from %s\n", threadID, inet_ntoa(connData[threadID].srcAddr.sin_addr));
	//connData[threadID].buffer[connData[threadID].dataSize]='\0';
	else
		parseReceivedData(threadID);
	}
}