#ifndef NET_INCL_H
#define NET_INCL_H
#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <string>
#include <ctime>
#include <iostream>

#include "OysterMath.h"
using namespace Oyster::Math;

//ws2_32.lib is a lib file the linker requires for winsock compilation
#pragma comment(lib, "Ws2_32.lib")
#endif