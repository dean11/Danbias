#include "SocketClient.h"

bool SocketClient::initTCPSocket(int listenPort)
{
	TCPrecvAddr.sin_family = AF_INET;
	TCPrecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	TCPrecvAddr.sin_port = htons(/*TCPRecvPort*/listenPort);

	connTCP = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connTCP == INVALID_SOCKET) 
	{
		wprintf(L"socket function failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return false;
	}

	iResult = bind(connTCP, (SOCKADDR *) & TCPrecvAddr, addrSize);
	if (iResult == SOCKET_ERROR) 
	{
		int tst=WSAGetLastError();
		wprintf(L"bind function failed with error %d\n", WSAGetLastError());
		iResult = closesocket(connTCP);
		if (iResult == SOCKET_ERROR)
			wprintf(L"closesocket function failed with error %d\n", WSAGetLastError());
		//WSACleanup();
		return false;
	}
	return true;
}
bool SocketClient::sendDataTCP(const char* data, int size)
{
	iResult = sendto(connTCP,
		data, size, 0, (SOCKADDR *) & TCPsendAddr, addrSize);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"TCP sendto failed with error: %d\n", WSAGetLastError());
		return false;
	}
	return true;
}