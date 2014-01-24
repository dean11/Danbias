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
			ObjectTypeHeader ParseHeader(std::string filename);

		private:
			static const int TypeHeaderSize = 4;	//Including the TypeID
			static const int ObjectHeaderSize = 10;	//Including modelID, textureID, position, rotation, scale.
			static const int LevelHeaderSize = 10;	//Including Format version, Name, Map version etc.

			static const int FormatVersionMajor = 1;
			static const int FormatVersionMinor = 0;

			/*************************************
						Question
			*************************************/
			//Could we use the IDs in "ObjectDefines.h" or do we have to use our own???
			enum TypeID
			{
				TypeID_LevelHeader,
				TypeID_Player,
				TypeID_Object,


				TypeID_NUM_OF_TYPES,

				ObjectType_Unknown = -1,
			};

		};
	}
}
#endif