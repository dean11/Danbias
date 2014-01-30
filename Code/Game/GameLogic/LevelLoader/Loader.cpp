//////////////////////////////////
// Created by Sam Svensson 2013 //
//////////////////////////////////

#include "Loader.h"
#include <fstream>

using namespace GameLogic::LevelFileLoader;
using namespace Oyster::Resource;
using namespace std;

char* Loader::LoadFile(std::string fileName, int &size)
{
	//convert from string to wstring
	std::wstring temp(fileName.begin(), fileName.end());

	//convert from wstring to wchar then loads the file
	char* buffer = (char*)OysterResource::LoadResource(temp.c_str(), Oyster::Resource::ResourceType::ResourceType_Byte_Raw, -1 , false);

	size = OysterResource::GetResourceSize(buffer);
	return buffer;
}