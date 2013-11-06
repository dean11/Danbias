#include "SocketClient.h"
bool SocketClient::initUDPSocket(int listenPort)
{
	UDPrecvAddr.sin_family = AF_INET;
	UDPrecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	UDPrecvAddr.sin_port = htons(listenPort);
	//---------------------------------------------
	// Create a socket for sending data
	connUDP = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (connUDP == INVALID_SOCKET) 
	{
		wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return false;
	}
	iResult = bind(connUDP, (SOCKADDR *) & UDPrecvAddr, addrSize);
	if (iResult == SOCKET_ERROR) 
	{
		wprintf(L"bind function failed with error %d\n", WSAGetLastError());
		iResult = closesocket(connUDP);
		if (iResult == SOCKET_ERROR)
			wprintf(L"closesocket function failed with error %d\n", WSAGetLastError());
		WSACleanup();
		return false;
	}
	return true;
}
bool SocketClient::sendDataUDP(const char* data, int size)
{
	iResult = sendto(connUDP,
		data, size, 0, (SOCKADDR *) & UDPsendAddr, addrSize);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"sendto failed with error: %d\n", WSAGetLastError());
		//closesocket(connUDP);
		//WSACleanup();
		return false;
	}
	return true;
}