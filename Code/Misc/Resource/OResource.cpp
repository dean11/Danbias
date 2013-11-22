#include "OResource.h"

using namespace Oyster::Resource;

OResource::OResource(OHRESOURCE handle, ResourceType type, size_t resourceSize, size_t elementSize, ::std::wstring filename)
	:	resourceData		(handle)
	,	resourceFilename	(filename)
	,	resourceSize		(resourceSize)
	,	resourceElementSize	(elementSize)
	,	resourceType		(type)
{
	
}
OResource::~OResource()
{}

bool OResource::Release()
{
	if(this->resourceRef.Decref() == 0)
	{
		Remove(this->resourceType, this->resourceData);
		return true;
	}
	return false;
}

void OResource::Remove(ResourceType t, OHRESOURCE& r)
{
	switch (t)
	{
		case Oyster::Resource::ResourceType_Texture_PNG:
		case Oyster::Resource::ResourceType_Texture_DDS:
		case Oyster::Resource::ResourceType_Texture_JPG:

		break;

		case Oyster::Resource::ResourceType_Mesh_VertexData:
		case Oyster::Resource::ResourceType_Mesh_AnimationData:

		break;

		case Oyster::Resource::ResourceType_Audio_mp3:

		break;

		case Oyster::Resource::ResourceType_Shader_Vertex:
		case Oyster::Resource::ResourceType_Shader_Hull:
		case Oyster::Resource::ResourceType_Shader_Domain:
		case Oyster::Resource::ResourceType_Shader_Geometry:
		case Oyster::Resource::ResourceType_Shader_Pixel:
		case Oyster::Resource::ResourceType_Shader_Compute:

		break;

		case Oyster::Resource::ResourceType_UNKNOWN:
		case Oyster::Resource::ResourceType_Byte_Raw:
		case Oyster::Resource::ResourceType_Byte_ANSI:
		case Oyster::Resource::ResourceType_Byte_UTF8:
		case Oyster::Resource::ResourceType_Byte_UNICODE:
		case Oyster::Resource::ResourceType_Byte_UTF16LE:
			delete ((char*)r);
		break;
	}
}