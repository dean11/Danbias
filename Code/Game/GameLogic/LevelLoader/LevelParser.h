#ifndef LEVEL_PARSER_H
#define LEVEL_PARSER_H

#include <string>
#include <vector>
#include "ObjectDefines.h"
#include "Utilities.h"

namespace GameLogic
{
	namespace LevelFileLoader
	{
		class LevelParser
		{
		public:
			LevelParser();
			~LevelParser();

			//
			std::vector<Utility::DynamicMemory::SmartPointer<ObjectTypeHeader>> Parse(std::wstring filename);

			//
			LevelMetaData ParseHeader(std::wstring filename);

		private:
			LevelLoaderInternal::FormatVersion formatVersion;

		};
	}
}
#endif