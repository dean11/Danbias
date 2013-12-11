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
	~PrivateData()
	{
		for (auto i = attributes.begin(); i != attributes.end(); i++)
		{
			RemoveAttribute(i->first);
		}
	}
	void RemoveAttribute(int ID)
	{
		auto i = attributes.find(ID);
		if(i == attributes.end()) return;

		switch (i->second.type)
		{
			case NetAttributeType_CharArray:
				delete [] i->second.value.netCharPtr;
			break;
		}
	}

	//Do network stuff
};


CustomNetProtocol::CustomNetProtocol()
{
	this->privateData = new PrivateData();
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