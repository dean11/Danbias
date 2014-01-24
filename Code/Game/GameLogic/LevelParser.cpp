#include "LevelParser.h"

using namespace GameLogic;
using namespace ::LevelLoader;

LevelParser::LevelParser()
{
}

LevelParser::~LevelParser()
{
}

//
std::vector<ObjectTypeHeader> LevelParser::Parse(std::string filename)
{
	//Read entire level file.

	std::vector<ObjectTypeHeader> objects;

	unsigned int counter = 0;
	unsigned int stringSize = 0;
	while(counter < stringSize)
	{
		//Get typeID
		int typeID = 0;
		

		switch(typeID)
		{
		case TypeID_LevelHeader:
			//Call function
			counter += LevelHeaderSize;
			break;

		case TypeID_Object:
			//Call function
			counter += ObjectHeaderSize;
			break;
		default:
			//Couldn't find typeID. FAIL!!!!!!
			break;
		}
	}

	return objects;
}

//
ObjectTypeHeader LevelParser::ParseHeader(std::string filename)
{
	//Read entire level file.

	//Find the header in the returned string.
	unsigned int counter = 0;
	unsigned int stringSize = 0;

	ObjectTypeHeader header;
	header.typeID = ObjectType_Level;

	while(counter < stringSize)
	{
		int typeID = 0;
		switch(typeID)
		{
		case TypeID_LevelHeader:
			//Call function
			
			break;
		case TypeID_Object:
			//Call function
			counter += ObjectHeaderSize;
			break;

		default:
			//Couldn't find typeID. FAIL!!!!!!
			break;
		}
	}

	return header;
}