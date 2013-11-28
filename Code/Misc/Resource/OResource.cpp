#include "OResource.h"

using namespace Oyster::Resource;

OResource::OResource(OHRESOURCE handle, ResourceType type, size_t resourceSize, size_t elementSize, ::std::wstring filename)
	:	resourceFilename	(filename)
	,	resourceSize		(resourceSize)
	,	resourceElementSize	(elementSize)
	,	resourceType		(type)
	,	customData			(0)
{
	resourceData = handle;
}
OResource::~OResource()
{
	delete this->customData;
	this->customData = 0;
}


OResource*	OResource::Load		(const wchar_t filename[], ResourceType type)
{
	switch (type)
	{
		case Oyster::Resource::ResourceType_Byte_Raw:
		case Oyster::Resource::ResourceType_Byte_ANSI:
		case Oyster::Resource::ResourceType_Byte_UTF8:
		case Oyster::Resource::ResourceType_Byte_UNICODE:
		case Oyster::Resource::ResourceType_Byte_UTF16LE:
			return OResource::ByteLoader(filename, type);
		break;
	}

	return 0;
}
OResource*	OResource::Load		(const wchar_t filename[], CustomLoadFunction loadFnc)
{
	return OResource::CustomLoader(filename, loadFnc);
}
OResource*	OResource::Reload	(OResource* resource)
{
	if(!resource) return 0;

	switch (resource->resourceType)
	{
		case Oyster::Resource::ResourceType_Byte_Raw:
		case Oyster::Resource::ResourceType_Byte_ANSI:
		case Oyster::Resource::ResourceType_Byte_UTF8:
		case Oyster::Resource::ResourceType_Byte_UNICODE:
		case Oyster::Resource::ResourceType_Byte_UTF16LE:
			resource->ByteReloader();
		break;

		case Oyster::Resource::ResourceType_UNKNOWN:
			resource->CustomReloader();
		break;
	}

	return resource;
}
bool		OResource::Release	(OResource* resource)
{
	if(resource->resourceRef.Decref() == 0)
	{
		switch (resource->resourceType)
		{
			case Oyster::Resource::ResourceType_Byte_Raw:
			case Oyster::Resource::ResourceType_Byte_ANSI:
			case Oyster::Resource::ResourceType_Byte_UTF8:
			case Oyster::Resource::ResourceType_Byte_UNICODE:
			case Oyster::Resource::ResourceType_Byte_UTF16LE:
				resource->ByteUnloader();
			break;

			case Oyster::Resource::ResourceType_UNKNOWN:
				resource->CustomUnloader();
			break;
		}
		return true;
	}
	return false;
}


