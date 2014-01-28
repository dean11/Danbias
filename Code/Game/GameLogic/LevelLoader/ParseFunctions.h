//////////////////////////////////
// Created by Sam Svensson 2013 //
//////////////////////////////////

#ifndef PARSERFUNCTIONS_H
#define PARSERFUNCTIONS_H
#include "ObjectDefines.h"

namespace GameLogic
{
	namespace LevelFileLoader
	{
			void ParseObject(unsigned char* buffer, void* header, int size);
			void ParseLevelMetaData(unsigned char* buffer, struct ObjectTypeHeader &header); 
	}
}


#endif