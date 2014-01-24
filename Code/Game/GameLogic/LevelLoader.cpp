//////////////////////////////////
// Created by Sam Svensson 2013 //
//////////////////////////////////

#include "LevelLoader.h"
using namespace GameLogic;
using namespace GameLogic::LevelLoader;

std::vector<ObjectTypeHeader> LevelLoader::LoadLevel(std::string fileName)
{
	Parser->parse(fileName);
}

std::vector<ObjectTypeHeader> LevelLoader::LoadLevelHeader(std::string fileName)
{
	parser->parseHeader(fileName);
}