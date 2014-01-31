/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#include "CustomNetProtocol.h"
#include <map>
#include "Translator.h"
using namespace Oyster::Network;
using namespace Utility::DynamicMemory;


struct CustomNetProtocol::PrivateData
{
	std::map<int, NetAttributeContainer> attributes;		//...Im an idiot
	
	PrivateData()
	{  }

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
CustomNetProtocol::CustomNetProtocol(const CustomNetProtocol& o)
{
	this->privateData = o.privateData;
}
const CustomNetProtocol& CustomNetProtocol::operator=(const CustomNetProtocol& o)
{
	this->privateData = o.privateData;
	return *this;
}
CustomNetProtocol::~CustomNetProtocol()
{
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

void CustomNetProtocol::Set(int ID, Oyster::Network::NetAttributeValue val, Oyster::Network::NetAttributeType type)
{
	this->privateData->attributes[ID].type = type;

	switch (type)
	{
		case Oyster::Network::NetAttributeType_Bool:
		case Oyster::Network::NetAttributeType_Char:
		case Oyster::Network::NetAttributeType_UnsignedChar:
		case Oyster::Network::NetAttributeType_Short:
		case Oyster::Network::NetAttributeType_UnsignedShort:
		case Oyster::Network::NetAttributeType_Int:
		case Oyster::Network::NetAttributeType_UnsignedInt:
		case Oyster::Network::NetAttributeType_Int64:
		case Oyster::Network::NetAttributeType_UnsignedInt64:
		case Oyster::Network::NetAttributeType_Float:
		case Oyster::Network::NetAttributeType_Double:
			this->privateData->attributes[ID].value = val;
		break;
	}
}
void CustomNetProtocol::Set(int ID, std::string s)
{
	if(s.size() == 0) return;

	this->privateData->attributes[ID].type = Oyster::Network::NetAttributeType_CharArray;

	this->privateData->attributes[ID].value.netCharPtr = new char[s.size() + 1];
	memcpy(&this->privateData->attributes[ID].value.netCharPtr[0], &s[0], s.size() + 1);
}
const NetAttributeContainer& CustomNetProtocol::Get(int id)
{
	return this->privateData->attributes[id];
}