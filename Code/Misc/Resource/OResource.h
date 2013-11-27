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
			struct CustomResourceData
			{
				CustomLoadFunction loadingFunction;
				CustomUnloadFunction unloadingFunction;
			};

		public:
										OResource(OHRESOURCE handle, ResourceType type, size_t size, size_t elementSize, ::std::wstring resourceFilename);
			virtual~					OResource();

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
			static OResource*	Load				(const wchar_t filename[], ResourceType type);
			static OResource*	Load				(const wchar_t filename[], CustomLoadFunction loadFnc);
			static OResource*	Reload				(OResource* resource);
			static bool			Release				(OResource* resource);

			Utility::DynamicMemory::RefCount	resourceRef;

		private:
			static OResource*	ByteLoader			(const wchar_t filename[], ResourceType type, OResource* old = 0);
			void				ByteUnloader		();
			OResource*			ByteReloader		();

			static OResource*	CustomLoader		(const wchar_t filename[], CustomLoadFunction loadFnc);
			void				CustomUnloader		();
			OResource*			CustomReloader		();

			OHRESOURCE			resourceData;
			ResourceType		resourceType;
			size_t				resourceSize;
			size_t				resourceElementSize;
			::std::wstring		resourceFilename;
			unsigned int		resourceID;

			CustomResourceData	*customData;
		};
	}
}

#endif