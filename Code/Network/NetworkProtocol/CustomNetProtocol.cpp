/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#include "CustomNetProtocol.h"
#include <map>

using namespace Network;

enum NetContainerType
{
	NetContainer_Array,
	NetContainer_Single,
	NetContainer_UNKNOWN,
};
struct NetAttributeArray
{
	NetAttributeContainer *value;
	int count;
	NetAttributeArray()
		:value(0)
		, count(0)
	{}
};
union NetAttributeData
{
	struct { NetAttributeContainer singleVal; };
	struct { NetAttributeArray arrayVal; };
};
struct NetDataValue
{
	NetContainerType containerType;
	NetAttributeData attributeData;
	NetDataValue()
	{
		//memset(&attributeData, 0, sizeof(NetAttributeData));
		containerType = NetContainer_UNKNOWN;
		attributeData.singleVal.type = NetAttributeType_UNKNOWN;
	}
};



struct CustomNetProtocol::PrivateData
{
	std::map<int, NetDataValue> attributes;
	//DataValue *val;
	//unsigned int size;

	PrivateData()
	{
		//val = new DataValue[_size];
		//size = _size;
	}
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

		if(i->second.containerType == NetContainer_Single)
		{
			switch (i->second.attributeData.singleVal.type)
			{
				case NetAttributeType_CharArray:
					delete [] i->second.attributeData.singleVal.value.netCharPtr;
				break;
			}
		}
		else if(i->second.containerType == NetContainer_Array)
		{
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
NetAttributeContainer* CustomNetProtocol::operator[](int ID)
{
	NetAttributeContainer *retVal = 0;

	if(this->privateData->attributes.find(ID) == this->privateData->attributes.end())
		return retVal;

	if(this->privateData->attributes[ID].containerType == NetContainer_Single)
		retVal = &this->privateData->attributes[ID].attributeData.singleVal;

	if(this->privateData->attributes[ID].containerType == NetContainer_Array)
		retVal = this->privateData->attributes[ID].attributeData.arrayVal.value;

	return retVal;
}


NetAttributeContainer* CustomNetProtocol::BuildValue(NetAttributeContainer val, short ID)
{
	if(val.type == NetAttributeType_UNKNOWN)	return 0;
	
	this->privateData->RemoveAttribute(ID);
	
	this->privateData->attributes[ID].attributeData.singleVal = val;

	return &this->privateData->attributes[ID].attributeData.singleVal;
}
NetAttributeContainer* CustomNetProtocol::BuildValue(NetAttributeValue value, NetAttributeType type, short ID)
{
	if(type == NetAttributeType_UNKNOWN)	return 0;
	
	this->privateData->RemoveAttribute(ID);
	
	this->privateData->attributes[ID].attributeData.singleVal.value = value;
	this->privateData->attributes[ID].attributeData.singleVal.type = type;

	return &this->privateData->attributes[ID].attributeData.singleVal;
}
NetAttributeContainer* CustomNetProtocol::BuildValue(const bool				& attribute, short attributeId)
{
	this->privateData->RemoveAttribute(attributeId);

	this->privateData->attributes[attributeId].attributeData.singleVal.value.netBool = attribute;
	this->privateData->attributes[attributeId].attributeData.singleVal.type = NetAttributeType_Bool;
	this->privateData->attributes[attributeId].containerType = NetContainer_Single;

	return &this->privateData->attributes[attributeId].attributeData.singleVal;
}
NetAttributeContainer* CustomNetProtocol::BuildValue(const char				& attribute, short attributeId)
{
	this->privateData->RemoveAttribute(attributeId);
	
	this->privateData->attributes[attributeId].attributeData.singleVal.value.netChar = attribute;
	this->privateData->attributes[attributeId].attributeData.singleVal.type = NetAttributeType_Char;
	this->privateData->attributes[attributeId].containerType = NetContainer_Single;

	return &this->privateData->attributes[attributeId].attributeData.singleVal;
}
NetAttributeContainer* CustomNetProtocol::BuildValue(const unsigned char		& attribute, short attributeId)
{
	this->privateData->RemoveAttribute(attributeId);
	
	this->privateData->attributes[attributeId].attributeData.singleVal.value.netUChar = attribute;
	this->privateData->attributes[attributeId].attributeData.singleVal.type = NetAttributeType_UnsignedChar;
	this->privateData->attributes[attributeId].containerType = NetContainer_Single;

	return &this->privateData->attributes[attributeId].attributeData.singleVal;
}
NetAttributeContainer* CustomNetProtocol::BuildValue(const short				& attribute, short attributeId)
{
	this->privateData->RemoveAttribute(attributeId);
	
	this->privateData->attributes[attributeId].attributeData.singleVal.value.netShort = attribute;
	this->privateData->attributes[attributeId].attributeData.singleVal.type = NetAttributeType_Short;
	this->privateData->attributes[attributeId].containerType = NetContainer_Single;

	return &this->privateData->attributes[attributeId].attributeData.singleVal;
}
NetAttributeContainer* CustomNetProtocol::BuildValue(const unsigned short		& attribute, short attributeId)
{
	this->privateData->RemoveAttribute(attributeId);
	
	this->privateData->attributes[attributeId].attributeData.singleVal.value.netUShort = attribute;
	this->privateData->attributes[attributeId].attributeData.singleVal.type = NetAttributeType_UnsignedShort;
	this->privateData->attributes[attributeId].containerType = NetContainer_Single;

	return &this->privateData->attributes[attributeId].attributeData.singleVal;
}
NetAttributeContainer* CustomNetProtocol::BuildValue(const int				& attribute, short attributeId)
{
	this->privateData->RemoveAttribute(attributeId);
	
	this->privateData->attributes[attributeId].attributeData.singleVal.value.netInt = attribute;
	this->privateData->attributes[attributeId].attributeData.singleVal.type = NetAttributeType_Int;
	this->privateData->attributes[attributeId].containerType = NetContainer_Single;

	return &this->privateData->attributes[attributeId].attributeData.singleVal;
}
NetAttributeContainer* CustomNetProtocol::BuildValue(const unsigned int		& attribute, short attributeId)
{
	this->privateData->RemoveAttribute(attributeId);
	
	this->privateData->attributes[attributeId].attributeData.singleVal.value.netUInt = attribute;
	this->privateData->attributes[attributeId].attributeData.singleVal.type = NetAttributeType_UnsignedInt;
	this->privateData->attributes[attributeId].containerType = NetContainer_Single;

	return &this->privateData->attributes[attributeId].attributeData.singleVal;
}
NetAttributeContainer* CustomNetProtocol::BuildValue(const __int64			& attribute, short attributeId)
{
	this->privateData->RemoveAttribute(attributeId);
	
	this->privateData->attributes[attributeId].attributeData.singleVal.value.netInt64 = attribute;
	this->privateData->attributes[attributeId].attributeData.singleVal.type = NetAttributeType_Int64;
	this->privateData->attributes[attributeId].containerType = NetContainer_Single;

	return &this->privateData->attributes[attributeId].attributeData.singleVal;
}
NetAttributeContainer* CustomNetProtocol::BuildValue(const unsigned __int64	& attribute, short attributeId)
{
	this->privateData->RemoveAttribute(attributeId);
	
	this->privateData->attributes[attributeId].attributeData.singleVal.value.netUInt64 = attribute;
	this->privateData->attributes[attributeId].attributeData.singleVal.type = NetAttributeType_UnsignedInt64;
	this->privateData->attributes[attributeId].containerType = NetContainer_Single;

	return &this->privateData->attributes[attributeId].attributeData.singleVal;
}
NetAttributeContainer* CustomNetProtocol::BuildValue(const float				& attribute, short attributeId)
{
	this->privateData->RemoveAttribute(attributeId);
	
	this->privateData->attributes[attributeId].attributeData.singleVal.value.netFloat = attribute;
	this->privateData->attributes[attributeId].attributeData.singleVal.type = NetAttributeType_Float;
	this->privateData->attributes[attributeId].containerType = NetContainer_Single;

	return &this->privateData->attributes[attributeId].attributeData.singleVal;
}
NetAttributeContainer* CustomNetProtocol::BuildValue(const double				& attribute, short attributeId)
{
	this->privateData->RemoveAttribute(attributeId);
	
	this->privateData->attributes[attributeId].attributeData.singleVal.value.netDouble = attribute;
	this->privateData->attributes[attributeId].attributeData.singleVal.type = NetAttributeType_Double;
	this->privateData->attributes[attributeId].containerType = NetContainer_Single;

	return &this->privateData->attributes[attributeId].attributeData.singleVal;
}
NetAttributeContainer* CustomNetProtocol::BuildValue(const std::string		& attribute, short attributeId)
{
	this->privateData->RemoveAttribute(attributeId);

	this->privateData->attributes[attributeId].attributeData.singleVal.value.netCharPtr = _strdup(attribute.c_str());
	this->privateData->attributes[attributeId].attributeData.singleVal.type = NetAttributeType_CharArray;
	this->privateData->attributes[attributeId].containerType = NetContainer_Single;

	return &this->privateData->attributes[attributeId].attributeData.singleVal;
}
NetAttributeContainer* CustomNetProtocol::BuildValue(const char* attribute, short attributeId)
{
	this->privateData->RemoveAttribute(attributeId);

	this->privateData->attributes[attributeId].attributeData.singleVal.value.netCharPtr = _strdup(attribute);
	this->privateData->attributes[attributeId].attributeData.singleVal.type = NetAttributeType_CharArray;
	this->privateData->attributes[attributeId].containerType = NetContainer_Single;

	return &this->privateData->attributes[attributeId].attributeData.singleVal;
}


//void CustomNetProtocol::SetArrayValue(NetAttributeContainer attribute[], unsigned int attributeCount, int ID)
//{
//	if(this->privateData->attributes.find(ID) == this->privateData->attributes.end())
//	{
//		this->privateData->attributes[ID] = NetDataValue(); 
//	}
//
//	if(this->privateData->attributes[ID].containerType == NetContainer_UNKNOWN)
//		this->privateData->attributes[ID].containerType = NetContainer_Array;
//
//	this->privateData->attributes[ID].attributeData.arrayVal.value = attribute;
//}

NetAttributeContainer& CustomNetProtocol::GetSingleValue(int attributeID)
{
	return this->privateData->attributes[attributeID].attributeData.singleVal;
}
//NetAttributeContainer* CustomNetProtocol::GetArrayValue(int attributeID)
//{
//	return this->privateData->attributes[attributeID].attributeData.arrayVal.value;
//}













