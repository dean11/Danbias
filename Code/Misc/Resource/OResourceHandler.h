#ifndef MISC_O_RESOURCE_HANLDER_H
#define MISC_O_RESOURCE_HANLDER_H


#include "OysterResource.h"

#include <map>

/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////




namespace Oyster
{
	namespace Resource
	{
		class OResourceHandler	:public IResourceHandler
		{
		public:
								OResourceHandler		();
			virtual~			OResourceHandler		();
			void				Release					()															override;
			void				Clean					()															override;
			OHRESOURCE			LoadResource			(const wchar_t filename[], ResourceType type, bool force)	override;
			void				ReleaseResource			(const OHRESOURCE& resource)								override;
			void				SetResourceId			(const OHRESOURCE& resource, unsigned int id)				override;
			ResourceType		GetResourceType			(const OHRESOURCE& resource)								const override;
			const wchar_t*		GetResourceFilename		(const OHRESOURCE& resource)								const override;
			unsigned int		GetResourceId			(const OHRESOURCE& resource)								const override;
		};
	}
}

#endif