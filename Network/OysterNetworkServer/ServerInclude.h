#include "Network.h"
#pragma once
#ifdef _DEBUG
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#else
#define DEBUG_NEW new
#endif

#include <vector>
#include <time.h>
#include <string>
#include <sstream>
#include "OysterMath.h"
//#include "Session.h"
#include "ServerTimer.h"
using namespace Network;

const float GAME_UPDATEDELAY=1.0f/120.0f;
