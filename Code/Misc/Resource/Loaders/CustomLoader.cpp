/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////

#include "..\OResource.h"
#include "..\..\Utilities.h"

#include <fstream>

using namespace Oyster::Resource;


OResource*	OResource::CustomLoader(const wchar_t filename[], CustomLoadFunction fnc)
{
	CustomData &data = fnc(filename);

	if(!data.loadedData)		return 0;
	if(!data.resourceUnloadFnc)	return 0;
	OHRESOURCE n = (OHRESOURCE)data.loadedData;
	OResource *resource = new OResource(n, ResourceType_UNKNOWN, 0, 0, filename);
	
	resource->customData = new CustomResourceData();
	resource->customData->unloadingFunction = data.resourceUnloadFnc;
	//resource->resourceData = (OHRESOURCE)data.loadedData;
	resource->customData->loadingFunction = fnc;

	return resource;
}
void OResource::CustomUnloader()
{
	this->customData->unloadingFunction((void*)this->resourceData);
}
OResource* OResource::CustomReloader()
{
	CustomUnloader();

	const CustomData &data = this->customData->loadingFunction(this->resourceFilename.c_str());
	this->resourceData = (OHRESOURCE)data.loadedData;

	if(data.resourceUnloadFnc)
		this->customData->unloadingFunction = data.resourceUnloadFnc;

	return this;
}

