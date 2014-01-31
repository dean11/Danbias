//////////////////////////////////
// Created by Sam Svensson 2013 //
//////////////////////////////////

#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include <string>
#include <vector>
#include <Vector.h>
#include "ObjectDefines.h"
#include "LevelParser.h"

namespace GameLogic
{
		class LevelLoader
		{

		public:
			LevelLoader(){this->parser =  GameLogic::LevelFileLoader::LevelParser(); }
			~LevelLoader(){}

			/********************************************************
			 * Loads the level and objects from file.
			 * @param fileName: Path to the level-file that you want to load.
			 * @return: Returns all structs with objects and information about the level.
			 ********************************************************/
			std::vector<ObjectTypeHeader> LoadLevel(std::string fileName);

			/********************************************************
			 * Just for fast access for the meta information about the level.
			 * @param fileName: Path to the level-file that you want to load.
			 * @return: Returns the meta information about the level.
			 ********************************************************/
			LevelMetaData LoadLevelHeader(std::string fileName); //.

		private:
			GameLogic::LevelFileLoader::LevelParser parser;
		};
}

#endif