//////////////////////////////////
// Created by Sam Svensson 2013 //
//////////////////////////////////

#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include <vector>
#include <Vector.h>
#include "ObjectDefines.h"
#include "LevelParser.h"

namespace GameLogic
{
		class LevelLoader
		{

		public:
			LevelLoader(){this->parser = new Parser()};
			~LevelLoader(){};
			std::vector<ObjectTypeHeader> LoadLevel(std::string fileName); //loads the level and objects from file
			std::vector<ObjectTypeHeader> LoadLevelHeader(std::string fileName); //just for fast access for the meta information about the level.

		private:
			LevelParser parser;
		};
}

#endif