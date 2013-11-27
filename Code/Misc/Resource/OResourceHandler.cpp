/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////


#include "OysterResource.h"
#include "OResource.h"
#include <string>
#include <map>

using namespace Oyster::Resource;

class ResourcePrivate
{
public:

	std::map<std::wstring, OResource*> resources;

	OResource* FindResource(const OHRESOURCE& h) const;
	OResource* FindResource(const wchar_t c[]) const;
	void SaveResource(OResource* r, bool addNew = true);

} resourcePrivate;


OHRESOURCE OysterResource::LoadResource(const wchar_t* filename, ResourceType type)
{
	if(!filename) return 0;

	OResource *resourceData = resourcePrivate.FindResource(filename);
	if(resourceData)	
	{
		//Add new reference
		resourcePrivate.SaveResource(resourceData, false);
		return resourceData->GetResourceHandle();
	}
	
	resourceData = OResource::Load(filename, type);

	if(!resourceData) return 0;

	resourcePrivate.SaveResource(resourceData);

	return resourceData->GetResourceHandle();
}
OHRESOURCE OysterResource::LoadResource(const wchar_t filename[], CustomLoadFunction loadFnc, unsigned int CustomId)
{
	if(!filename)	return 0;
	if(!loadFnc)	return 0;

	OResource *resourceData = resourcePrivate.FindResource(filename);
	if(resourceData)	
	{
		//Add new reference
		resourcePrivate.SaveResource(resourceData, false);
		return resourceData->GetResourceHandle();
	}

	resourceData = OResource::Load(filename, loadFnc);

	if(!resourceData) return 0;

	resourceData->SetResourceID(CustomId);

	resourcePrivate.SaveResource(resourceData);

	return (OHRESOURCE)resourceData->GetResourceHandle();
}

OHRESOURCE ReloadResource(const wchar_t filename[])
{
	OResource *resourceData = resourcePrivate.FindResource(filename);
	if(!resourceData) return 0;		//The resource has not been loaded

	return OResource::Reload(resourceData)->GetResourceHandle();
}
OHRESOURCE ReloadResource(OHRESOURCE resource)
{
	OResource *resourceData = resourcePrivate.FindResource(resource);
	if(!resourceData) return 0;		//The resource has not been loaded

	return OResource::Reload(resourceData)->GetResourceHandle();
}

void OysterResource::Clean()
{
	auto i = resourcePrivate.resources.begin();
	auto last = resourcePrivate.resources.end();

	for (i; i != last; i++)
	{
		if(OResource::Release(i->second))
		{
			const wchar_t* temp = i->second->GetResourceFilename();
			delete resourcePrivate.resources[temp];
			resourcePrivate.resources.erase(temp);
		}
	}
}
void OysterResource::ReleaseResource(const OHRESOURCE& resourceData)
{
	OResource* t = resourcePrivate.FindResource(resourceData);
	if(t)
	{
		if(OResource::Release(t))
		{
			const wchar_t* temp = t->GetResourceFilename();
			delete resourcePrivate.resources[temp];
			resourcePrivate.resources.erase(temp);
		}
	}
}

void OysterResource::SetResourceId (const OHRESOURCE& resourceData, unsigned int id)
{
	OResource* t = resourcePrivate.FindResource(resourceData);

	if(t)	t->SetResourceID(id);
}
ResourceType OysterResource::GetResourceType (const OHRESOURCE& resourceData)
{
	OResource* t = resourcePrivate.FindResource(resourceData);

	if(t)	return t->GetResourceType();

	return ResourceType_UNKNOWN;
}
const wchar_t* OysterResource::GetResourceFilename (const OHRESOURCE& resourceData)
{
	OResource* t = resourcePrivate.FindResource(resourceData);

	if(t)	return t->GetResourceFilename();

	return 0;
}
unsigned int OysterResource::GetResourceId (const OHRESOURCE& resourceData)	
{
	OResource* t = resourcePrivate.FindResource(resourceData);

	if(t)	return t->GetResourceID();

	return -1;
}



OResource* ResourcePrivate::FindResource(const OHRESOURCE& h) const
{
	for (auto i = this->resources.begin(); i != this->resources.end() ; i++)
	{
		if(i->second->GetResourceHandle() == h)
		{
			return i->second;
		}
	}
	return 0;
}
OResource* ResourcePrivate::FindResource(const wchar_t c[]) const
{
	std::wstring temp = c;
	auto t = this->resources.find(c);
	if(t == this->resources.end()) return 0;

	return t->second;
}
void ResourcePrivate::SaveResource( OResource* r, bool addNew )
{
	if(!r)	return;

	if(addNew)
	{
		this->resources[r->GetResourceFilename()] = r;
	}
	
	r->resourceRef.Incref();
}




