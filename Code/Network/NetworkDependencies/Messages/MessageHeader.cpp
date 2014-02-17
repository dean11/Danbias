#include "MessageHeader.h"
#include "Packing/Packing.h"
#include <iostream>
using namespace std;

using namespace Oyster::Packing;
using namespace Oyster::Network;
using namespace Oyster::Network::Messages;
using namespace Oyster::Network::Protocols;

MessageHeader::MessageHeader()
{
	size = 0;
}

MessageHeader::~MessageHeader()
{
}

void MessageHeader::Pack(ProtocolHeader& header, OysterByte& bytes)
{
	size = 0;

	PackInt(header.size, bytes);
	PackInt(header.packageType, bytes);
	PackInt(header.clientID, bytes);
	SetSize(bytes);
}

void MessageHeader::Unpack(OysterByte& bytes, ProtocolHeader& header)
{
	size = 0;
	
	header.size = UnpackInt(bytes);
	header.packageType = UnpackInt(bytes);
	header.clientID = UnpackInt(bytes);
}

/**************************
		Pack
**************************/

void MessageHeader::PackBool(bool i, OysterByte& bytes)
{
	bytes.AddSize(sizeof(i));
	Packing::Pack(&bytes.GetByteArray()[size], i);
	size += sizeof(i);
}

void MessageHeader::PackChar(char i, OysterByte& bytes)
{
	bytes.AddSize(sizeof(i));
	Packing::Pack(&bytes.GetByteArray()[size], i);
	size += sizeof(i);
}

void MessageHeader::PackUnsignedChar(unsigned char i, OysterByte& bytes)
{
	bytes.AddSize(sizeof(i));
	Packing::Pack(&bytes.GetByteArray()[size], i);
	size += sizeof(i);
}

void MessageHeader::PackShort(short i, OysterByte& bytes)
{
	bytes.AddSize(sizeof(i));
	Packing::Pack(&bytes.GetByteArray()[size], i);
	size += sizeof(i);
}

void MessageHeader::PackUnsignedShort(unsigned short i, OysterByte& bytes)
{
	bytes.AddSize(sizeof(i));
	Packing::Pack(&bytes.GetByteArray()[size], i);
	size += sizeof(i);
}

void MessageHeader::PackInt(int i, OysterByte& bytes)
{
	bytes.AddSize(sizeof(i));
	Packing::Pack(&bytes.GetByteArray()[size], i);
	size += sizeof(i);
}

void MessageHeader::PackUnsignedInt(unsigned int i, OysterByte& bytes)
{
	bytes.AddSize(sizeof(i));
	Packing::Pack(&bytes.GetByteArray()[size], i);
	size += sizeof(i);
}

void MessageHeader::PackInt64(__int64 i, OysterByte& bytes)
{
	bytes.AddSize(sizeof(i));
	Packing::Pack(&bytes.GetByteArray()[size], i);
	size += sizeof(i);
}

void MessageHeader::PackUnsignedInt64(unsigned __int64 i, OysterByte& bytes)
{
	bytes.AddSize(sizeof(i));
	Packing::Pack(&bytes.GetByteArray()[size], i);
	size += sizeof(i);
}

void MessageHeader::PackFloat(float i, OysterByte& bytes)
{
	bytes.AddSize(sizeof(i));
	Packing::Pack(&bytes.GetByteArray()[size], i);
	size += sizeof(i);
}

void MessageHeader::PackFloat(float i[], unsigned int elementCount, OysterByte& bytes)
{
	//Pack number of elements
	PackUnsignedInt(elementCount, bytes);

	//Pack all elements
	for(int j = 0; j < (int)elementCount; j++)
	{
		PackFloat(i[j], bytes);
	}
}

void MessageHeader::PackDouble(double i, OysterByte& bytes)
{               
	bytes.AddSize(sizeof(i));
	Packing::Pack(&bytes.GetByteArray()[size], i);
	size += sizeof(i);
}

void MessageHeader::PackStr(char str[], OysterByte& bytes)
{
	int totalSize = sizeof(short) + (int)strlen(str);
	bytes.AddSize(totalSize);
	Packing::Pack(&bytes.GetByteArray()[size], str);
	size += totalSize;
}

void MessageHeader::PackStr(std::string str, OysterByte& bytes)
{
	int totalSize = sizeof(short) + (int)str.length();
	bytes.AddSize(totalSize);
	Packing::Pack(&bytes.GetByteArray()[size], str);
	size += totalSize;
}

/**************************
		Unpack
**************************/

bool MessageHeader::UnpackBool(OysterByte& bytes)
{
	bool i = Packing::Unpackb(&bytes.GetByteArray()[size]);
	size += sizeof(i);
	return i;
}

char MessageHeader::UnpackChar(OysterByte& bytes)
{
	char i = Packing::Unpackc(&bytes.GetByteArray()[size]);
	size += sizeof(i);
	return i;
}

unsigned char MessageHeader::UnpackUnsignedChar(OysterByte& bytes)
{
	unsigned char i = Packing::UnpackC(&bytes.GetByteArray()[size]);
	size += sizeof(i);
	return i;
}

short MessageHeader::UnpackShort(OysterByte& bytes)
{
	short i = Packing::Unpacks(&bytes.GetByteArray()[size]);
	size += sizeof(i);
	return i;
}

unsigned short MessageHeader::UnpackUnsignedShort(OysterByte& bytes)
{
	unsigned short i = Packing::UnpackS(&bytes.GetByteArray()[size]);
	size += sizeof(i);
	return i;
}

int MessageHeader::UnpackInt(OysterByte& bytes)
{
	int i = Packing::Unpacki(&bytes.GetByteArray()[size]);
	size += sizeof(i);
	return i;
}

unsigned int MessageHeader::UnpackUnsignedInt(OysterByte& bytes)
{
	unsigned int i = Packing::UnpackI(&bytes.GetByteArray()[size]);
	size += sizeof(i);
	return i;
}

__int64 MessageHeader::UnpackInt64(OysterByte& bytes)
{
	__int64 i = Packing::Unpacki64(&bytes.GetByteArray()[size]);
	size += sizeof(i);
	return i;
}

unsigned __int64 MessageHeader::UnpackUnsignedInt64(OysterByte& bytes)
{
	unsigned __int64 i = Packing::UnpackI64(&bytes.GetByteArray()[size]);
	size += sizeof(i);
	return i;
}

float MessageHeader::UnpackFloat(OysterByte& bytes)
{
	float i = Packing::Unpackf(&bytes.GetByteArray()[size]);
	size += sizeof(i);
	return i;
}

float* MessageHeader::UnpackFloat(unsigned int& elementCount, OysterByte& bytes)
{
	float* i;

	elementCount = UnpackUnsignedInt(bytes);

	i = new float[elementCount];
	for(int j = 0; j < (int)elementCount; j++)
	{
		i[j] = UnpackFloat(bytes);
	}

	return i;
}

double MessageHeader::UnpackDouble(OysterByte& bytes)
{
	double i = Packing::Unpackd(&bytes.GetByteArray()[size]);
	size += sizeof(i);
	return i;
}

char* MessageHeader::UnpackCStr(OysterByte& bytes)
{
	char* str = Packing::UnpackCStr(&bytes.GetByteArray()[size]);
	size += sizeof(short) + (int)strlen(str);
	return str;
}

std::string MessageHeader::UnpackStr(OysterByte& bytes)
{
	std::string str = Packing::UnpackStr(&bytes.GetByteArray()[size]);
	size += sizeof(short) + (int)str.length();
	return str;
}

void MessageHeader::SetSize(OysterByte& bytes)
{
	Packing::Pack(bytes, size);
}

void MessageHeader::SetSize(unsigned int size)
{
	this->size = size;
}