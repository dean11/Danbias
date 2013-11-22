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


void IResourceHandler::Release()
{
	IResourceHandler::Clean();
}
void IResourceHandler::Clean()
{
	auto i = resourcePrivate.resources.begin();
	auto last = resourcePrivate.resources.end();

	for (i; i != last; i++)
	{
		if(i->second->Release())
		{
			const wchar_t* temp = i->second->GetResourceFilename();
			delete resourcePrivate.resources[temp];
			resourcePrivate.resources.erase(temp);
		}
	}
}
OHRESOURCE IResourceHandler::LoadResource(const wchar_t* filename, ResourceType type, bool force)
{
	OResource *resourceData = resourcePrivate.FindResource(filename);
	if(resourceData)	
	{
		resourcePrivate.SaveResource(resourceData, false);
		return resourceData->GetResourceHandle();
	}

	switch (type)
	{
		case Oyster::Resource::ResourceType_Texture_PNG:
		case Oyster::Resource::ResourceType_Texture_DDS:
		case Oyster::Resource::ResourceType_Texture_JPG:
			resourceData = OResource::TextureLoader(filename, type);
		break;

		case Oyster::Resource::ResourceType_Mesh_VertexData:
		case Oyster::Resource::ResourceType_Mesh_AnimationData:
			resourceData = OResource::MeshLoader(filename, type);
		break;

		case Oyster::Resource::ResourceType_Audio_mp3:
			resourceData = OResource::AudioLoader(filename, type);
		break;

		case Oyster::Resource::ResourceType_Shader_Vertex:
		case Oyster::Resource::ResourceType_Shader_Hull:
		case Oyster::Resource::ResourceType_Shader_Domain:
		case Oyster::Resource::ResourceType_Shader_Geometry:
		case Oyster::Resource::ResourceType_Shader_Pixel:
		case Oyster::Resource::ResourceType_Shader_Compute:
			resourceData = OResource::ShaderLoader(filename, type);
		break;

		case Oyster::Resource::ResourceType_UNKNOWN:
		case Oyster::Resource::ResourceType_Byte_Raw:
		case Oyster::Resource::ResourceType_Byte_ANSI:
		case Oyster::Resource::ResourceType_Byte_UTF8:
		case Oyster::Resource::ResourceType_Byte_UNICODE:
		case Oyster::Resource::ResourceType_Byte_UTF16LE:
			resourceData = OResource::ByteLoader(filename, type);
		break;

		default:
			//The format is not supported.
			printf("(%i) is not a resourceData type or it is currently not supported!", type);
		break;
	}

	//Resource exists! It's a miracle =D.
	resourcePrivate.SaveResource(resourceData);

	return resourceData->GetResourceHandle();
}
void IResourceHandler::ReleaseResource(const OHRESOURCE& resourceData)
{
	OResource* t = resourcePrivate.FindResource(resourceData);
	if(t)
	{
		if(t->Release())
		{
			const wchar_t* temp = t->GetResourceFilename();
			delete resourcePrivate.resources[temp];
			resourcePrivate.resources.erase(temp);
		}
	}
}
void IResourceHandler::SetResourceId (const OHRESOURCE& resourceData, unsigned int id)
{
	OResource* t = resourcePrivate.FindResource(resourceData);

	if(t)	t->SetResourceID(id);
}
ResourceType IResourceHandler::GetResourceType (const OHRESOURCE& resourceData)
{
	OResource* t = resourcePrivate.FindResource(resourceData);

	if(t)	return t->GetResourceType();

	return ResourceType_UNKNOWN;
}
const wchar_t* IResourceHandler::GetResourceFilename (const OHRESOURCE& resourceData)
{
	OResource* t = resourcePrivate.FindResource(resourceData);

	if(t)	return t->GetResourceFilename();

	return 0;
}
unsigned int IResourceHandler::GetResourceId (const OHRESOURCE& resourceData)	
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

	if(!addNew)
	{
		r->resourceRef.Incref();
	}
	else
	{
		this->resources[r->GetResourceFilename()] = r;
		r->resourceRef.Incref();
	}
}




