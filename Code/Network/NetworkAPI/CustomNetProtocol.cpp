/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#include "CustomNetProtocol.h"
#include <map>
#include "Translator.h"
#include <DynamicArray.h>
using namespace Oyster::Network;
using namespace Utility::DynamicMemory;


struct CustomNetProtocol::PrivateData
{
	//std::map<int, NetAttributeContainer> attributes;		//...Im an idiot
	DynamicArray<NetAttributeContainer> attributes;		//...Im an idiot
	
	PrivateData()
	{  }

	~PrivateData()
	{
		attributes.Clear();
	}
	void RemoveAttribute(NetAttributeContainer* i)
	{
		if(!i) return;

		switch (i->type)
		{
			case NetAttributeType_CharArray:
				delete [] i->value.netCharPtr;
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
	this->privateData = new PrivateData();
	this->privateData->attributes = o.privateData->attributes;
}

CustomNetProtocol& CustomNetProtocol::operator=(const CustomNetProtocol& o)
{
	if(this->privateData) 
	{
		delete this->privateData;
		this->privateData = 0;
	}
	this->privateData = new PrivateData();
	this->privateData->attributes = o.privateData->attributes;
	return *this;
}

CustomNetProtocol::~CustomNetProtocol()
{
	delete this->privateData;
	this->privateData = 0;
}

const NetAttributeContainer& CustomNetProtocol::operator[](int ID) const
{
	//if(!this->privateData) this->privateData = new PrivateData();
	if((unsigned int)ID >= this->privateData->attributes.Size())
	{
		NetAttributeContainer temp;
		
		temp.type = NetAttributeType_UNKNOWN;
		memset(&temp.value, 0, sizeof(NetAttributeValue));

		this->privateData->attributes.Push(ID, temp);
	}

	return this->privateData->attributes[ID];
}

NetAttributeContainer& CustomNetProtocol::operator[](int ID)
{
	//if(!this->privateData) this->privateData = new PrivateData();
	if((unsigned int)ID >= this->privateData->attributes.Size())
	{
		NetAttributeContainer temp;
		
		temp.type = NetAttributeType_UNKNOWN;
		memset(&temp.value, 0, sizeof(NetAttributeValue));

		this->privateData->attributes.Push(ID, temp);
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





///////////////////////////////////////////////////////////////////////
//// Created by [Dennis Andersen] [2013]
///////////////////////////////////////////////////////////////////////
//#include "CustomNetProtocol.h"
//#include <map>
//#include "Translator.h"
//#include <DynamicArray.h>
//using namespace Oyster::Network;
//using namespace Utility::DynamicMemory;
//
//
//
//struct CustomNetProtocol::PrivateData
//{
//	Utility::DynamicMemory::DynamicArray<NetAttributeContainer> attributes;		//...Im an idiot
//	
//	PrivateData()
//	{  }
//
//	~PrivateData()
//	{
//		for (unsigned int i = 0; i < attributes.Size(); i++)
//		{
//			RemoveAttribute(i);
//		}
//
//		attributes.Clear();
//	}
//	void RemoveAttribute(int i)
//	{
//		switch (attributes[i].type)
//		{
//			case NetAttributeType_CharArray:
//				delete [] attributes[i].value.netCharPtr;
//			break;
//		}
//	}
//
//	//Do network stuff
//};
//
//
//CustomNetProtocol::CustomNetProtocol()
//{
//	this->privateData = new PrivateData();
//}
//CustomNetProtocol::CustomNetProtocol(const CustomNetProtocol& o)
//{
//	this->privateData = o.privateData;
//}
//const CustomNetProtocol& CustomNetProtocol::operator=(const CustomNetProtocol& o)
//{
//	this->privateData = o.privateData;
//	return *this;
//}
//CustomNetProtocol::~CustomNetProtocol()
//{
//}
//NetAttributeContainer& CustomNetProtocol::operator[](int ID)
//{
//	if(ID >= this->privateData->attributes.Size())
//		this->privateData->attributes.Resize(
//	if(this->privateData->attributes.find(ID) == this->privateData->attributes.end())
//	{
//		this->privateData->attributes[ID];
//		this->privateData->attributes[ID].type = NetAttributeType_UNKNOWN;
//		memset(&this->privateData->attributes[ID].value, 0, sizeof(NetAttributeValue));
//	}
//
//	return this->privateData->attributes[ID];
//}
//
//void CustomNetProtocol::Set(int ID, Oyster::Network::NetAttributeValue val, Oyster::Network::NetAttributeType type)
//{
//	this->privateData->attributes[ID].type = type;
//
//	switch (type)
//	{
//		case Oyster::Network::NetAttributeType_Bool:
//		case Oyster::Network::NetAttributeType_Char:
//		case Oyster::Network::NetAttributeType_UnsignedChar:
//		case Oyster::Network::NetAttributeType_Short:
//		case Oyster::Network::NetAttributeType_UnsignedShort:
//		case Oyster::Network::NetAttributeType_Int:
//		case Oyster::Network::NetAttributeType_UnsignedInt:
//		case Oyster::Network::NetAttributeType_Int64:
//		case Oyster::Network::NetAttributeType_UnsignedInt64:
//		case Oyster::Network::NetAttributeType_Float:
//		case Oyster::Network::NetAttributeType_Double:
//			this->privateData->attributes[ID].value = val;
//		break;
//	}
//}
//void CustomNetProtocol::Set(int ID, std::string s)
//{
//	if(s.size() == 0) return;
//
//	this->privateData->attributes[ID].type = Oyster::Network::NetAttributeType_CharArray;
//
//	this->privateData->attributes[ID].value.netCharPtr = new char[s.size() + 1];
//	memcpy(&this->privateData->attributes[ID].value.netCharPtr[0], &s[0], s.size() + 1);
//}
//const NetAttributeContainer& CustomNetProtocol::Get(int id)
//{
//	return this->privateData->attributes[id];
//}