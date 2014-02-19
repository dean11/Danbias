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
	std::wstring folderPath;
};

LevelLoader::LevelLoader()
	: pData(new PrivData)
{
	//standard path
	pData->folderPath = L"";
}

LevelLoader::LevelLoader(std::wstring folderPath)
	: pData(new PrivData)
{
	pData->folderPath = folderPath;
}

LevelLoader::~LevelLoader()
{
}

std::vector<Utility::DynamicMemory::SmartPointer<ObjectTypeHeader>> LevelLoader::LoadLevel(std::wstring fileName)
{
	return pData->parser.Parse(pData->folderPath + fileName);
}

LevelMetaData LevelLoader::LoadLevelHeader(std::wstring fileName)
{
	return pData->parser.ParseHeader(pData->folderPath + fileName);
}

std::wstring LevelLoader::GetFolderPath()
{
	return this->pData->folderPath;
}

void LevelLoader::SetFolderPath(std::wstring folderPath)
{
	this->pData->folderPath = folderPath;
}