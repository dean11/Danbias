//////////////////////////////////
// Created by Sam Svensson 2013 //
//////////////////////////////////

#include "LevelLoader.h"
#include "LevelParser.h"
#include <Resource\OysterResource.h>

using namespace GameLogic;
using namespace GameLogic::LevelFileLoader;

struct LevelLoader::PrivData
{
	LevelParser parser;
	std::string folderPath;
};

LevelLoader::LevelLoader()
	: pData(new PrivData)
{
	//standard path
	pData->folderPath = "";
}

LevelLoader::LevelLoader(std::string folderPath)
	: pData(new PrivData)
{
	pData->folderPath = folderPath;
}

LevelLoader::~LevelLoader()
{
	Oyster::Resource::OysterResource::Clean();
}

std::vector<Utility::DynamicMemory::SmartPointer<ObjectTypeHeader>> LevelLoader::LoadLevel(std::string fileName)
{
	return pData->parser.Parse(pData->folderPath + fileName);
}

LevelMetaData LevelLoader::LoadLevelHeader(std::string fileName)
{
	return pData->parser.ParseHeader(pData->folderPath + fileName);
}

std::string LevelLoader::GetFolderPath()
{
	return this->pData->folderPath;
}

void LevelLoader::SetFolderPath(std::string folderPath)
{
	this->pData->folderPath = folderPath;
}