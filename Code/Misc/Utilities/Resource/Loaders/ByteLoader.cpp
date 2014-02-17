
#include "..\OResource.h"
#include "..\..\Utilities.h"

#include <fstream>

using namespace Oyster::Resource;
bool readANSI = false;



bool ReadFromFile(const wchar_t fileName[], const wchar_t openFlag[], std::wstring& outData, size_t elemSize)
{
	size_t bytesTotal = 0;
	size_t bytesRead = 0;
	FILE *stream;

	if( _wfopen_s( &stream, fileName, openFlag ) == 0 )
	{
		//Get size of the file
		fseek(stream, 0L, SEEK_END);
		bytesTotal = ftell(stream);
		fseek(stream, 0L, SEEK_SET);

		//Sanity check
		if(bytesTotal == 0) return false;

		//Create the new byte buffer
		wchar_t *buff = new wchar_t[bytesTotal + 1];

		//Read the bytes to the end
		bytesRead = fread_s( buff, bytesTotal, elemSize, bytesTotal ,stream );
		fclose( stream );

		//Did we read enough bytes
		if(!readANSI && bytesRead != bytesTotal)	return false;

		//Add delimiter
		buff[bytesRead] = L'\0';

		outData.resize(bytesTotal);
		outData = buff;

		delete [] buff;
	}
	else
	{
		std::wstring msg = L"Failed to open file: \n";
		msg.append(fileName);

		return false;
	}

	return true;
}
bool ReadFromFile(const wchar_t fileName[], const char openFlag[], std::string& outData, size_t elemSize)
{
	std::string sFilename;
	std::wstring wsFile = fileName;
	::Utility::String::WStringToString(wsFile, sFilename);
	size_t bytesTotal = 0;
	size_t bytesRead = 0;
	FILE *stream;

	if( fopen_s( &stream, sFilename.c_str(), openFlag ) == 0 )
	{
		//Get size of the file
		fseek(stream, 0L, SEEK_END);
		bytesTotal = ftell(stream);
		fseek(stream, 0L, SEEK_SET);
		fflush(stream);

		//Sanity check
		if(bytesTotal == 0) return false;

		//Create the new byte buffer
		char *buff = new char[bytesTotal + 1];

		//Read the bytes to the end
		bytesRead = fread_s( buff, bytesTotal, elemSize, bytesTotal ,stream );
		fclose( stream );

		//Did we read enough bytes (Get the bytes if we read with ANSI since the hidden characters is ignored)
		if(!readANSI && bytesRead != bytesTotal)	return false;

		buff[bytesRead + 1];

		outData.clear();
		outData.resize(bytesRead);
		memcpy(&outData[0], &buff[0], bytesRead);
	
		delete [] buff;
	}
	else
	{
		std::string msg = "Failed to open file: \n";
		msg.append(sFilename.c_str());

		return false;
	}

	return true;
}


OResource* OResource::ByteLoader(const wchar_t filename[], ResourceType type, OResource* old)
{
	OResource *resource = old;
	std::wstring wOut;
	std::string sOut;
	bool success = false;

	switch (type)
	{
		case Oyster::Resource::ResourceType_Byte_Raw:
			success = ReadFromFile(filename, "rb", sOut, sizeof(char));
		break;
			
		case Oyster::Resource::ResourceType_Byte_ANSI:
			readANSI = true;
			success = ReadFromFile(filename, "r", sOut, sizeof(char));
			readANSI = false;
		break;

		case Oyster::Resource::ResourceType_Byte_UTF8:
			success = ReadFromFile(filename, "r, ccs=UTF-8", sOut, sizeof(char));
		break;

		case Oyster::Resource::ResourceType_Byte_UNICODE:
			success = ReadFromFile(filename, "r, ccs=UNICODE", sOut, sizeof(char));
		break;

		case Oyster::Resource::ResourceType_Byte_UTF16LE:
			success = ReadFromFile(filename, "r, ccs=UTF-16LE", sOut, sizeof(char));
		break;
	}

	if(!success)	return 0;
	if(wOut.size())
	{
		//const wchar_t *data = new wchar_t[wOut.size()];
		//resource = new OResource((void*)data, type, (sizeof(wchar_t) * wOut.size()), sizeof(wchar_t), filename);
	}
	else if(sOut.size())
	{
		char *data = new char[sOut.size()+1];
		data[sOut.size()] = '\0';
		memcpy(&data[0], &sOut[0], sOut.size());

		if(!old)
		{
			resource = new OResource((OHRESOURCE&)data, type, (sizeof(char) * sOut.size()), sizeof(char), filename);
		}
		else
		{
			old->resourceData = (OHRESOURCE)data;
		}
	}
	return resource;
}
			
void OResource::ByteUnloader()
{
	delete [] ((char*)this->resourceData);
	this->resourceData = 0;
}

OResource* OResource::ByteReloader()
{
	ByteUnloader();
	return ByteLoader(this->resourceFilename.c_str(), this->resourceType, this); 
}

