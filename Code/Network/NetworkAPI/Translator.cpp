#include "Translator.h"

#include <string>
#include <map>

#include "CustomNetProtocol.h"

#include "Utilities.h"
#include "../NetworkDependencies/Messages/MessageHeader.h"
#include "../NetworkDependencies/OysterByte.h"
#include <DynamicArray.h>

using namespace Oyster::Network;
using namespace ::Messages;
using namespace Utility::DynamicMemory;
using namespace std;


//TODO: Fix this uggly hack
struct MyCastingStruct
{
	Utility::DynamicMemory::DynamicArray<NetAttributeContainer> attributes;
};

// TODO: Check if the package has been packed correctly.
struct Translator::PrivateData
{
	PrivateData()
	{
		numberOfUnknownTypes = 0;
		size = 0;
	}

	//Packages a header with a size(int) and a string of characters(char)
	void PackHeader(OysterByte &bytes, CustomNetProtocol& protocol)
	{
		auto it = ((MyCastingStruct*)protocol.privateData)->attributes.begin();
		auto end = ((MyCastingStruct*)protocol.privateData)->attributes.end();

		size = 4;	//size(int)
		message.SetSize(0);
		message.PackInt(size, bytes);

		//Find all the data types
		for(; it != end; it++)
		{
			headerString.push_back(it->type);
		}

		message.PackShort((short)headerString.size(), bytes);
		size += 2;

		for(int i = 0; i < (int)headerString.size(); i++)
		{
			message.PackChar(headerString.at(i), bytes);
			size++;
		}

		message.SetSize(bytes);
	}

	void PackMessage(OysterByte &bytes, CustomNetProtocol& protocol)
	{
		auto it = ((MyCastingStruct*)protocol.privateData)->attributes.begin();
		auto end = ((MyCastingStruct*)protocol.privateData)->attributes.end();

		for(int i = 0; i < (int)headerString.size(); i++, it++)
		{
			switch((int)headerString.at(i))
			{
			case NetAttributeType_Bool:
				message.PackBool(it->value.netBool, bytes);
				break;
			case NetAttributeType_Char:
				message.PackChar(it->value.netChar, bytes);
				break;
			case NetAttributeType_UnsignedChar:
				message.PackUnsignedChar(it->value.netUChar, bytes);
				break;
			case NetAttributeType_Short:
				message.PackShort(it->value.netShort, bytes);
				break;
			case NetAttributeType_UnsignedShort:
				message.PackUnsignedShort(it->value.netUShort, bytes);
				break;
			case NetAttributeType_Int:
				message.PackInt(it->value.netInt, bytes);
				break;
			case NetAttributeType_UnsignedInt:
				message.PackUnsignedInt(it->value.netUInt, bytes);
				break;
			case NetAttributeType_Int64:
				message.PackInt64(it->value.netInt64, bytes);
				break;
			case NetAttributeType_UnsignedInt64:
				message.PackUnsignedInt64(it->value.netUInt64, bytes);
				break;
			case NetAttributeType_Float:
				message.PackFloat(it->value.netFloat, bytes);
				break;
			case NetAttributeType_Double:
				message.PackDouble(it->value.netDouble, bytes);
				break;
			case NetAttributeType_CharArray:
				message.PackStr(it->value.netCharPtr, bytes);
				break;
			default:
				numberOfUnknownTypes++;
				break;
			}
		}

		message.SetSize(bytes);
	}

	bool UnpackHeader(CustomNetProtocol& protocol, OysterByte &bytes)
	{
		message.SetSize(0);
		int packageSize = message.UnpackInt(bytes);
		if(packageSize != bytes.GetSize())
		{
			return false;
		}

		short numberOfTypes = message.UnpackShort(bytes);

		for(int i = 0; i < numberOfTypes; i++)
		{
			char temp = message.UnpackChar(bytes);
			headerString.push_back(temp);
		}

		return true;
	}

	void UnpackMessage(CustomNetProtocol& protocol, OysterByte &bytes)
	{
		for(int i = 0; i < (int)headerString.size(); i++)
		{
			protocol[i].type = (NetAttributeType)headerString.at(i);
			switch(protocol[i].type)
			{
			case NetAttributeType_Bool:
				protocol[i].value.netBool = message.UnpackBool(bytes);
				break;
			case NetAttributeType_Char:
				protocol[i].value.netChar = message.UnpackChar(bytes);
				break;
			case NetAttributeType_UnsignedChar:
				protocol[i].value.netUChar = message.UnpackUnsignedChar(bytes);
				break;
			case NetAttributeType_Short:
				protocol[i].value.netShort = message.UnpackShort(bytes);
				break;
			case NetAttributeType_UnsignedShort:
				protocol[i].value.netUShort = message.UnpackUnsignedShort(bytes);
				break;
			case NetAttributeType_Int:
				protocol[i].value.netInt = message.UnpackInt(bytes);
				break;
			case NetAttributeType_UnsignedInt:
				protocol[i].value.netUInt = message.UnpackUnsignedInt(bytes);
				break;
			case NetAttributeType_Int64:
				protocol[i].value.netInt64 = message.UnpackInt64(bytes);
				break;
			case NetAttributeType_UnsignedInt64:
				protocol[i].value.netUInt64 = message.UnpackUnsignedInt64(bytes);
				break;
			case NetAttributeType_Float:
				protocol[i].value.netFloat = message.UnpackFloat(bytes);
				break;
			case NetAttributeType_Double:
				protocol[i].value.netDouble = message.UnpackDouble(bytes);
				break;
			case NetAttributeType_CharArray:
				protocol[i].value.netCharPtr = message.UnpackCStr(bytes);
				break;
			default:
				numberOfUnknownTypes++;
				break;
			}
		}
	}
	
	MessageHeader message;
	string headerString;
	unsigned int size;

	int numberOfUnknownTypes;
};

Translator::Translator()
{
	privateData = new PrivateData();
}

Translator::~Translator()
{
	if(privateData)
	{
		delete privateData;
		privateData = NULL;
	}
}
Translator::Translator(const Translator& obj)
{
	this->privateData = new PrivateData(*obj.privateData);
}
const Translator& Translator::operator=(const Translator& obj)
{
	delete this->privateData;
	this->privateData = new PrivateData(*obj.privateData);
	return *this;
}

void Translator::Pack(OysterByte &bytes, CustomNetProtocol& protocol)
{

	privateData->headerString.clear();

	privateData->PackHeader(bytes, protocol);
	privateData->PackMessage(bytes, protocol);
}

bool Translator::Unpack(CustomNetProtocol& protocol, OysterByte &bytes)
{
	this->privateData->headerString.clear();

	if(!privateData->UnpackHeader(protocol, bytes))
	{
		return false;
	}

	privateData->UnpackMessage(protocol, bytes);

	return true;
}