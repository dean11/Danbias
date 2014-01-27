//////////////////////////////////
// Created by Sam Svensson 2013 //
//////////////////////////////////

#include "Loader.h"

using namespace GameLogic::LevelFileLoader;
using namespace Oyster::Resource;
using namespace std;

unsigned char* Loader::LoadFile(std::string fileName, int &size)
{
	//convert from string to wstring
	std::wstring temp(fileName.begin(), fileName.end());

	size = temp.size();
	//convert from wstring to wchar then loads the file
	return (unsigned char*)OysterResource::LoadResource(temp.c_str(), Oyster::Resource::ResourceType::ResourceType_Byte_Raw, -1 , false); 
}