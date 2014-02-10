//////////////////////////////////
// Created by Sam Svensson 2013 //
//////////////////////////////////

#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include <string>
#include <vector>
#include "../Misc/Utilities.h"
#include "ObjectDefines.h"

namespace GameLogic
{
	class LevelLoader
	{

	public:
		LevelLoader();
		~LevelLoader();

		/********************************************************
			* Loads the level and objects from file.
			* @param fileName: Path to the level-file that you want to load.
			* @return: Returns all structs with objects and information about the level.
			********************************************************/
		std::vector<Utility::DynamicMemory::SmartPointer<ObjectTypeHeader>> LoadLevel(std::string fileName);

		/********************************************************
			* Just for fast access for the meta information about the level.
			* @param fileName: Path to the level-file that you want to load.
			* @return: Returns the meta information about the level.
			********************************************************/
		LevelMetaData LoadLevelHeader(std::string fileName); //.

	private:
		struct PrivData;
		Utility::DynamicMemory::SmartPointer<PrivData> pData;
	};
}

#endif