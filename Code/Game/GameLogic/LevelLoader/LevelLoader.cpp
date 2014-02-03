//////////////////////////////////
// Created by Sam Svensson 2013 //
//////////////////////////////////

#include "LevelLoader.h"
#include "LevelParser.h"
using namespace GameLogic;
using namespace GameLogic::LevelFileLoader;

struct LevelLoader::PrivData
{
	LevelParser parser;
};

LevelLoader::LevelLoader()
	: pData(new PrivData)
{
}

LevelLoader::~LevelLoader()
{
}

std::vector<Utility::DynamicMemory::SmartPointer<ObjectTypeHeader>> LevelLoader::LoadLevel(std::string fileName)
{
	return pData->parser.Parse(fileName);
}

LevelMetaData LevelLoader::LoadLevelHeader(std::string fileName)
{
	return pData->parser.ParseHeader(fileName);
}