/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////


#include "ResourceManager.h"
#include "..\Utilities.h"

using namespace Oyster::Resource;

struct ::ResourceData
{
	LoadFunction loadFnc;
	UnloadFunction unloadFnc;
	ResourceType resourcetype;
	HRESOURCE resource;
	unsigned int resourceSize;
	int resourceID;
	Utility::DynamicMemory::ReferenceCount referenceCount;
};

bool ReadFromFile(const wchar_t fileName[], const char openFlag[], std::string& outData, size_t elemSize, bool ANSI = false)
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
		if(!ANSI && bytesRead != bytesTotal)	return false;

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
const wchar_t* FindResourceKey(std::map<std::wstring, ResourceData*>& resources, const HRESOURCE h)
{
	for (auto i = resources.begin(); i != resources.end() ; i++)
	{
		if(i->second && i->second->resource == h)
		{
			return i->first.c_str();
		}
	}
	return 0;
}
ResourceData* FindResource(std::map<std::wstring, ResourceData*>& resources, const HRESOURCE h)
{
	for (auto i = resources.begin(); i != resources.end() ; i++)
	{
		if(i->second->resource == h)
		{
			return i->second;
		}
	}
	return 0;
}
ResourceData* FindResource(std::map<std::wstring, ResourceData*>& resources, const wchar_t c[])
{
	std::wstring temp = c;
	auto t = resources.find(c);
	if(t == resources.end()) return 0;

	return t->second;
}
void SaveResource( std::map<std::wstring, ResourceData*>& resources, ResourceData* r, const std::wstring& key, bool addNew )
{
	if(addNew)
	{
		resources[key] = r;
	}
	
	r->referenceCount.Incref();
}
bool Release(std::map<std::wstring, ResourceData*>& resources, ResourceData* resource)
{
	if(resource->referenceCount.Decref() < 1)
	{
		const wchar_t* temp = FindResourceKey(resources, resource->resource);

		switch (resource->resourcetype)
		{
			case Oyster::Resource::ResourceType_Byte_Raw:
			case Oyster::Resource::ResourceType_Byte_ANSI:
			case Oyster::Resource::ResourceType_Byte_UTF8:
			case Oyster::Resource::ResourceType_Byte_UNICODE:
			case Oyster::Resource::ResourceType_Byte_UTF16LE:
				delete [] ((char*)resource->resource);
				resource->resource = 0;
			break;

			case Oyster::Resource::ResourceType_CUSTOM:
				resource->unloadFnc(resource->resource);
				resource->resource = 0;
			break;
		}
		
		if(temp)	delete resources[temp];

		return true;
	}
	return false;
}
ResourceData* Load(/*Out*/ResourceData* targetMem, /*in*/const wchar_t source[], /*in*/ResourceType type)
{
	targetMem->resource = 0;
	targetMem->loadFnc = 0;
	targetMem->unloadFnc = 0;
	targetMem->resourceID = 0;
	targetMem->resourcetype = type;
	targetMem->resourceSize = 0;

	std::string sOut;
	bool success = false;

	switch (type)
	{
		case Oyster::Resource::ResourceType_Byte_Raw:
			success = ReadFromFile(source, "rb", sOut, sizeof(char));
		break;
			
		case Oyster::Resource::ResourceType_Byte_ANSI:
			success = ReadFromFile(source, "r", sOut, sizeof(char), true);
		break;

		case Oyster::Resource::ResourceType_Byte_UTF8:
			success = ReadFromFile(source, "r, ccs=UTF-8", sOut, sizeof(char));
		break;

		case Oyster::Resource::ResourceType_Byte_UNICODE:
			success = ReadFromFile(source, "r, ccs=UNICODE", sOut, sizeof(char));
		break;

		case Oyster::Resource::ResourceType_Byte_UTF16LE:
			success = ReadFromFile(source, "r, ccs=UTF-16LE", sOut, sizeof(char));
		break;
	}

	if(!success)	return 0;
	
	if(sOut.size())
	{
		char *data = new char[sOut.size()+1];
		data[sOut.size()] = '\0';
		memcpy(&data[0], &sOut[0], sOut.size());

		targetMem->resource = (HRESOURCE&)data;
		targetMem->loadFnc = 0;
		targetMem->unloadFnc = 0;
		targetMem->resourceID = 0;
		targetMem->resourcetype = type;
	}

	return targetMem;
}
ResourceData* Load(/*Out*/ResourceData* targetMem, /*in*/const wchar_t source[], LoadFunction loadFnc, UnloadFunction unloadFnc)
{
	targetMem->resource = 0;
	targetMem->loadFnc = 0;
	targetMem->unloadFnc = 0;
	targetMem->resourceID = 0;
	targetMem->resourcetype = ResourceType_CUSTOM;
	targetMem->resourceSize = 0;

	if(loadFnc)	
	{
		targetMem->resource = loadFnc(source);
		if(targetMem->resource)
		{
			targetMem->resourceSize = 0;
			targetMem->resourcetype = ResourceType_CUSTOM;
			targetMem->loadFnc = loadFnc;
			targetMem->unloadFnc = unloadFnc;
		}
	}
	return targetMem;
}
ResourceData* Reload(std::map<std::wstring, ResourceData*> resources, ResourceData* resource, const wchar_t* filename)
{
	switch (resource->resourcetype)
	{
		case Oyster::Resource::ResourceType_Byte_Raw:
		case Oyster::Resource::ResourceType_Byte_ANSI:
		case Oyster::Resource::ResourceType_Byte_UTF8:
		case Oyster::Resource::ResourceType_Byte_UNICODE:
		case Oyster::Resource::ResourceType_Byte_UTF16LE:
			if(Release(resources, resource))
				return Load(resource, filename, resource->loadFnc, resource->unloadFnc); 
		break;

		case Oyster::Resource::ResourceType_CUSTOM:
		{
			resource->unloadFnc(resource->resource);

			HRESOURCE r = resource->loadFnc(filename);
			if(!r) return 0;
			resource->resource = r;
		}
		break;
	}

	return resource;
}



ResourceManager::ResourceManager()
{  }
ResourceManager::~ResourceManager()
{ Clean(); }

HBYTEARRAY ResourceManager::LoadBytes(const wchar_t filename[], ResourceType type, int customID, bool force)
{
	if(!filename) return 0;

	ResourceData *t = FindResource(this->resources, filename);

	if(t)	
	{
		if(force)
		{
			return (HBYTEARRAY)Reload(resources, t, filename )->resource;
		}
		else
		{
			//Add new reference
			SaveResource(this->resources, t, filename, false);
			return (HBYTEARRAY)t->resource;
		}
	}
	else
	{
		t = Load(new ResourceData(), filename, type);
		if(t) 
		{
			t->resourceID = (customID);
			SaveResource(this->resources, t, filename, true);
		}
		else
		{
			return 0;
		}
	}

	return (HBYTE*)t->resource;
}
HRESOURCE ResourceManager::LoadResource(const wchar_t filename[], LoadFunction loadFnc, UnloadFunction unloadFnc, int customId, bool force)
{
	if(!filename)	
	{
		return 0;
	}
	if(!loadFnc || !unloadFnc)	
	{
		return 0;
	}

	ResourceData *t = FindResource(this->resources, filename);
	if(t)	
	{
		t->loadFnc = loadFnc;
		t->unloadFnc = unloadFnc;
		if(force)
		{
			return ResourceManager::ReloadResource(filename);
		}
		else
		{
			//Add new reference
			SaveResource(this->resources, t, filename, false);
			return t->resource;
		}
	}
	else
	{
		t = Load(new ResourceData(), filename, loadFnc, unloadFnc );
		if(t && t->resource)
		{
			t->resourceID = (customId);
			SaveResource(this->resources, t, filename, true);
		}
		else
		{
			delete t;
			t = 0;
		}
	}
	if(!t)
	{
		return 0;
	}
	return (HRESOURCE)t->resource;
}

HRESOURCE ResourceManager::ReloadResource(const wchar_t filename[])
{
	ResourceData *t = FindResource(this->resources, filename);
	if(!t) return 0;		//The resource has not been loaded

	return Reload(this->resources, t, filename)->resource;
}
HRESOURCE ResourceManager::ReloadResource(HRESOURCE& resource)
{
	ResourceData *t = FindResource(this->resources, resource);
	if(!t) return 0;
	return Reload(this->resources, t, FindResourceKey(this->resources, resource))->resource;
}

void ResourceManager::Clean()
{
	if(this->resources.empty()) return;

	auto i = this->resources.begin();
	auto last = resources.end();

	for (i; i != last; i++)
	{
		//Remove all the references
		while (!Release(resources, i->second));
	}
	resources.clear();
}
void ResourceManager::ReleaseResource(const HRESOURCE& resourceData)
{
	const wchar_t* temp = FindResourceKey(resources, resourceData);
	
	if(temp)
	{
		ResourceData *t = FindResource(this->resources, resourceData);
		if(Release(resources, t))
		{
			resources.erase(temp);
		}
	}
}
void ResourceManager::ReleaseResource(const wchar_t filename[])
{
	ResourceData *t = FindResource(this->resources, filename);
	if(t)
	{
		if(Release(resources, t))
		{
			resources.erase(filename);
		}
	}
}



void ResourceManager::SetResourceId (const HRESOURCE& resourceData, unsigned int id)
{
	ResourceData *t = FindResource(this->resources, resourceData);

	if(t)	t->resourceID = (id);
}
void ResourceManager::SetResourceId(const wchar_t c[], unsigned int id)
{
	ResourceData *t = FindResource(this->resources, c);

	if(t)	t->resourceID = (id);
}
ResourceType ResourceManager::GetResourceType (const HRESOURCE& resourceData)
{
	ResourceData *t = FindResource(this->resources, resourceData);

	if(t)	return t->resourcetype;

	return ResourceType_INVALID;
}
ResourceType ResourceManager::GetResourceType (const wchar_t c[])
{
	ResourceData *t = FindResource(this->resources, c);

	if(t)	return t->resourcetype;

	return ResourceType_INVALID;
}
const wchar_t* ResourceManager::GetResourceFilename (const HRESOURCE& resourceData)
{
	return FindResourceKey(this->resources, resourceData);
}
HRESOURCE ResourceManager::GetResourceHandle(const wchar_t filename[])
{
	ResourceData *t = FindResource(this->resources, filename);

	if(t)	return t->resource;

	return 0;
}
int ResourceManager::GetResourceId (const HRESOURCE& resourceData)	
{
	ResourceData *t = FindResource(this->resources, resourceData);

	if(t)	return t->resourceID;

	return -1;
}
int ResourceManager::GetResourceId(const wchar_t c[])
{
	ResourceData *t = FindResource(this->resources, c);

	if(t)	return t->resourceID;

	return -1;
}
int	ResourceManager::GetResourceCount(const wchar_t filename[])
{
	ResourceData *t = FindResource(this->resources, filename);

	if(t) return t->referenceCount;

	return 0;
}
int	ResourceManager::GetResourceCount(const HRESOURCE& resource)
{
	ResourceData *t = FindResource(this->resources, resource);

	if(t) return t->referenceCount;

	return 0;
}
int	ResourceManager::GetResourceSize(const wchar_t filename[])
{
	ResourceData *t = FindResource(this->resources, filename);

	if(t) return t->resourceSize;

	return 0;
}
int	ResourceManager::GetResourceSize(const HRESOURCE& resource)
{
	ResourceData *t = FindResource(this->resources, resource);

	if(t) return t->resourceSize;

	return 0;
}





