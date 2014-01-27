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
		ObjectTypeHeader ParseObjectTypeHeader(unsigned char* buffer); //send in a char buffer, this function will seperate it and then return the right struct with the values.
		ObjectHeader     ParseObjectHeader    (unsigned char* buffer); //send in a char buffer, this function will seperate it and then return the right struct with the values.
	}
}


#endif