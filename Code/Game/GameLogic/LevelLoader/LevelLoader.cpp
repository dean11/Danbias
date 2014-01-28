//////////////////////////////////
// Created by Sam Svensson 2013 //
//////////////////////////////////

#include "LevelLoader.h"
using namespace GameLogic;
using namespace GameLogic::LevelFileLoader;


std::vector<ObjectTypeHeader> LevelLoader::LoadLevel(std::string fileName)
{
	return parser->Parse(fileName);
}

ObjectTypeHeader LevelLoader::LoadLevelHeader(std::string fileName)
{
	return parser->ParseHeader(fileName);
}