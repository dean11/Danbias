/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////

#ifndef MISC_O_RESOURCE_H
#define MISC_O_RESOURCE_H

#include "..\Utilities.h"
#include "OysterResource.h"
#include <string>

namespace Oyster
{
	namespace Resource
	{
		class OResource
		{
		public:
										OResource(OHRESOURCE handle, ResourceType type, size_t size, size_t elementSize, ::std::wstring resourceFilename);
			virtual~					OResource();
			bool						Release();

			inline ResourceType			GetResourceType()					const	
			{ return this->resourceType; }
			inline const wchar_t*		GetResourceFilename()				const	
			{ return this->resourceFilename.c_str(); }
			inline OHRESOURCE			GetResourceHandle()					const	
			{ return this->resourceData; }
			inline unsigned long long	GetResourceSize()					const	
			{ return this->resourceSize; }
			inline unsigned long long	GetResourceElementSize()			const	
			{ return this->resourceElementSize; }
			inline unsigned int			GetResourceID()						const	
			{ return this->resourceID; }
			inline void					SetResourceID(unsigned int id)				
			{ this->resourceID = id; }

		public:
			static OResource*	TextureLoader		(const wchar_t filename[], ResourceType type)	{return 0;}
			static OResource*	MeshLoader			(const wchar_t filename[], ResourceType type)	{return 0;};
			static OResource*	AudioLoader			(const wchar_t filename[], ResourceType type)	{return 0;};
			static OResource*	ShaderLoader		(const wchar_t filename[], ResourceType type)	{return 0;};
			static OResource*	ByteLoader			(const wchar_t filename[], ResourceType type);

			Utility::DynamicMemory::RefCount	resourceRef;

		private:
			static void			Remove			(ResourceType t, OHRESOURCE& r);

			OHRESOURCE							resourceData;
			ResourceType						resourceType;
			size_t								resourceSize;
			size_t								resourceElementSize;
			::std::wstring						resourceFilename;
			unsigned int						resourceID;
		};
	}
}

#endif