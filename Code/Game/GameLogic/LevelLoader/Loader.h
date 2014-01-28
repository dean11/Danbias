//////////////////////////////////
// Created by Sam Svensson 2013 //
//////////////////////////////////

#ifndef LOADER_H
#define LOADER_H

#include "..\Misc\Resource\OysterResource.h"
#include <string>

namespace GameLogic
{
	namespace LevelFileLoader
	{
		class Loader
		{
		public:
			Loader (){};
			~Loader(){};
			unsigned char* LoadFile(std::string fileName, int &size); 

			//TODO:
			//Add functionality to load physicsObjects (hitboxes)
		};
	}
}

#endif;