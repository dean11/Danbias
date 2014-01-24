//////////////////////////////////
// Created by Sam Svensson 2013 //
//////////////////////////////////

#ifndef LOADER_H
#define LOADER_H

#include "Resource\OysterResource.h"
#include <string>

namespace GameLogic
{
	namespace LevelLoader
	{
		class Loader
		{
		public:
			Loader(){};
			~Loader(){};
			char* LoadFile(std::string fileName); 

			//TODO:
			//Add functionality to load physicsObjects (hitboxes)
		};
	}
}

#endif;