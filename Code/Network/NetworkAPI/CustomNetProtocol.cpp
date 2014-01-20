/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#include "CustomNetProtocol.h"
#include <map>
#include "Translator.h"
using namespace Oyster::Network;


struct CustomNetProtocol::PrivateData
{
	std::map<int, NetAttributeContainer> attributes;

	PrivateData()
	{ }
	PrivateData( const CustomNetProtocol::PrivateData& o)
	{ 
		for (auto i = o.attributes.begin(); i != o.attributes.end(); i++)
		{
			if(i->second.type == NetAttributeType_CharArray)
			{
				size_t size = strlen(i->second.value.netCharPtr);
				if(size == 0) continue;

				attributes[i->first].value.netCharPtr = new char[size + 1];
				//strcpy_s(attributes[i->first].value.netCharPtr, size + 1, i->second.value.netCharPtr);
				memcpy(&attributes[i->first].value.netCharPtr[0], &i->second.value.netCharPtr[0], size + 1);
				attributes[i->first].type = NetAttributeType_CharArray;
			}
			else
			{
				attributes[i->first] = i->second;
			}
		}
	}
	~PrivateData()
	{
		for (auto i = attributes.begin(); i != attributes.end(); i++)
		{
			RemoveAttribute(i->first);
		}
		attributes.clear();
	}
	void RemoveAttribute(int ID)
	{
		auto i = attributes.find(ID);
		if(i == attributes.end()) return;

		switch (i->second.type)
		{
			case NetAttributeType_CharArray:
				//delete [] i->second.value.netCharPtr;
				i->second.value.netCharPtr = 0;
			break;
		}
	}

	//Do network stuff
};


CustomNetProtocol::CustomNetProtocol()
{
	this->privateData = new PrivateData();
}
CustomNetProtocol::CustomNetProtocol(const CustomNetProtocol& o)
{
	this->privateData = new PrivateData(*o.privateData);
}
const CustomNetProtocol& CustomNetProtocol::operator=(const CustomNetProtocol& o)
{
	delete this->privateData;
	this->privateData = new PrivateData(*o.privateData);
	return *this;
}
CustomNetProtocol::~CustomNetProtocol()
{
	delete this->privateData;
}
NetAttributeContainer& CustomNetProtocol::operator[](int ID)
{
	if(this->privateData->attributes.find(ID) == this->privateData->attributes.end())
	{
		this->privateData->attributes[ID];
		this->privateData->attributes[ID].type = NetAttributeType_UNKNOWN;
		memset(&this->privateData->attributes[ID].value, 0, sizeof(NetAttributeValue));
	}

	return this->privateData->attributes[ID];
}