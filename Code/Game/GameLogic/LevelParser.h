#ifndef LEVEL_PARSER_H
#define LEVEL_PARSER_H

#include <vector>
#include "ObjectDefines.h"

namespace GameLogic
{
	namespace LevelLoader
	{
		class LevelParser
		{
		public:
			LevelParser();
			~LevelParser();

			//
			std::vector<ObjectTypeHeader> Parse();

			//
			ObjectTypeHeader ParseHeader();

		private:

		};
	}
}
#endif