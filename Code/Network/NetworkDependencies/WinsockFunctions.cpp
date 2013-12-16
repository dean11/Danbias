#ifndef INCLUDE_WINSOCK_LIB
#define INCLUDE_WINSOCK_LIB
	#pragma comment(lib, "ws2_32.lib")
#endif

#include "WinsockFunctions.h"
#include <WinSock2.h>

bool InitWinSock()
{
	WSADATA wsaData;
	return WSAStartup(MAKEWORD(2, 2), &wsaData) == NO_ERROR;
}

void ShutdownWinSock()
{
	WSACleanup();
}

std::wstring GetErrorMessage(int errorCode)
{
	LPWSTR lpMessage;
	std::wstring retVal(L"Succesful");

	DWORD bufLen = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS ,
		NULL,
		errorCode ,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT) ,
		(LPWSTR)&lpMessage,
		0 ,
		NULL );

	if(bufLen)
	{
		retVal = lpMessage;
		
		LocalFree(lpMessage);

		return retVal;
	}

	//Added this if bufLen is 0
	return retVal;
}