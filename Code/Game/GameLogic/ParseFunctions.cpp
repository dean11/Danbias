//////////////////////////////////
// Created by Sam Svensson 2013 //
//////////////////////////////////

#include "ParseFunctions.h"
#include "../../Misc/Packing/Packing.h"
#include <string>

using namespace Oyster::Packing;
using namespace GameLogic::LevelFileLoader;
using namespace GameLogic;
using namespace std;

namespace GameLogic
{
	namespace LevelFileLoader
	{
		ObjectTypeHeader ParseObjectTypeHeader(unsigned char* buffer)
		{		
			struct ObjectTypeHeader header;
			memcpy(&header, buffer, sizeof(ObjectTypeHeader));
			return header;
		}

		ObjectHeader     ParseObjectHeader    (unsigned char* buffer)
		{
			struct ObjectHeader header;
			memcpy(&header, buffer, sizeof(ObjectHeader));
		
			return header;
		}
	}
}