#ifndef NETWORK_DEPENDENCIES_WINSOCK_FUNCTIONS_H
#define NETWORK_DEPENDENCIES_WINSOCK_FUNCTIONS_H

/////////////////////////////////////
// Created by Pontus Fransson 2013 //
/////////////////////////////////////

#include <string>

void ShutdownWinSock();
bool InitWinSock();

std::wstring GetErrorMessage(int errorCode);

#endif