#ifndef LEVEL_PARSER_H
#define LEVEL_PARSER_H

#include <string>
#include <vector>
#include "ObjectDefines.h"

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
			std::vector<ObjectTypeHeader> Parse(std::string filename);

			//
			LevelMetaData ParseHeader(std::string filename);

		private:
			FormatVersion formatVersion;

		};
	}
}
#endif