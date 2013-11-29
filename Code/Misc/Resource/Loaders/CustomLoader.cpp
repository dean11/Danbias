/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////

#include "..\OResource.h"
#include "..\..\Utilities.h"

#include <fstream>

using namespace Oyster::Resource;


OResource*	OResource::CustomLoader(const wchar_t filename[], CustomLoadFunction fnc)
{
	CustomData data;
	memset(&data, 0, sizeof(CustomData));

	fnc(filename, data);
	OHRESOURCE n = (OHRESOURCE)data.loadedData;
	if(!data.loadedData)			
	{
		return 0;
	}
	if(!data.resourceUnloadFnc)	
	{
		return 0;
	}
	/** For some wierd reason that i don't understand when trying to send data.loadedData directly as a 
	*	parameter to OResource constructor, the value is changed when it arrives in the constructor. 
	*	Doing it like this, storing in a temporary variable, the value stays correct. (What the fuck! I must be overloking something...)*/
	//OHRESOURCE temp = data.loadedData;
	OResource *resource = new OResource(data.loadedData, ResourceType_UNKNOWN, 0, 0, filename);

	resource->customData = new CustomResourceData();
	resource->customData->unloadingFunction = data.resourceUnloadFnc;
	resource->customData->loadingFunction = fnc;

	return resource;
}
void OResource::CustomUnloader()
{
	this->customData->unloadingFunction(this->resourceData);
}
OResource* OResource::CustomReloader()
{
	CustomUnloader();

	CustomData data;
	memset(&data, 0, sizeof(CustomData));

	this->customData->loadingFunction(this->resourceFilename.c_str(), data);
	this->resourceData = (OHRESOURCE)data.loadedData;

	if(data.resourceUnloadFnc)
	{
		this->customData->unloadingFunction = data.resourceUnloadFnc;
	}
	return this;
}

