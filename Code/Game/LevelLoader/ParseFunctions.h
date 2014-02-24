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
		/*
			These functions will copy data from where the buffer pointer points.
			header is the destination where the data will be copied.
			size is either the size of the data to be copied (if it is NOT sent by reference). 
			Or the current index that is being used to parse the entire file (if it is sent by reference) this means you have to increase size with the appropiate size after you have copied.

		*/
		void ParseObject(char* buffer, void *header, int size);
		void ParseLight(char* buffer, BasicLight& header, int& size);
		void ParseObject(char* buffer, ObjectHeader& header, int& size , bool loadCgf);
		void ParseLevelMetaData(char* buffer, LevelMetaData &header, int &size);
		void ParseBoundingVolume(char* buffer, LevelLoaderInternal::BoundingVolume& volume, int &size);
	}
}


#endif