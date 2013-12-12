#include "MessageHeader.h"
#include "../Packing.h"
#include <iostream>
using namespace std;

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
	bytes.AddSize(1);
	Packing::Pack(&bytes.GetByteArray()[size], i);
	size += 1;
}

void MessageHeader::PackChar(char i, OysterByte& bytes)
{
	bytes.AddSize(1);
	Packing::Pack(&bytes.GetByteArray()[size], i);
	size += 1;
}

void MessageHeader::PackUnsignedChar(unsigned char i, OysterByte& bytes)
{
	bytes.AddSize(1);
	Packing::Pack(&bytes.GetByteArray()[size], i);
	size += 1;
}

void MessageHeader::PackShort(short i, OysterByte& bytes)
{
	bytes.AddSize(2);
	Packing::Pack(&bytes.GetByteArray()[size], i);
	size += 2;
}

void MessageHeader::PackUnsignedShort(unsigned short i, OysterByte& bytes)
{
	bytes.AddSize(2);
	Packing::Pack(&bytes.GetByteArray()[size], i);
	size += 2;
}

void MessageHeader::PackInt(int i, OysterByte& bytes)
{
	bytes.AddSize(4);
	Packing::Pack(&bytes.GetByteArray()[size], i);
	size += 4;
}

void MessageHeader::PackUnsignedInt(unsigned int i, OysterByte& bytes)
{
	bytes.AddSize(4);
	Packing::Pack(&bytes.GetByteArray()[size], i);
	size += 4;
}

void MessageHeader::PackInt64(__int64 i, OysterByte& bytes)
{
	bytes.AddSize(8);
	Packing::Pack(&bytes.GetByteArray()[size], i);
	size += 8;
}

void MessageHeader::PackUnsignedInt64(unsigned __int64 i, OysterByte& bytes)
{
	bytes.AddSize(8);
	Packing::Pack(&bytes.GetByteArray()[size], i);
	size += 8;
}

void MessageHeader::PackFloat(float i, OysterByte& bytes)
{
	bytes.AddSize(4);
	Packing::Pack(&bytes.GetByteArray()[size], i);
	size += 4;
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
	bytes.AddSize(8);
	Packing::Pack(&bytes.GetByteArray()[size], i);
	size += 8;
}

void MessageHeader::PackStr(char str[], OysterByte& bytes)
{
	int totalSize = 2 + (int)strlen(str);
	bytes.AddSize(totalSize);
	Packing::Pack(&bytes.GetByteArray()[size], str);
	size += totalSize;
}

void MessageHeader::PackStr(std::string str, OysterByte& bytes)
{
	int totalSize = 2 + (int)str.length();
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
	size += 1;
	return i;
}

char MessageHeader::UnpackChar(OysterByte& bytes)
{
	char i = Packing::Unpackc(&bytes.GetByteArray()[size]);
	size += 1;
	return i;
}

unsigned char MessageHeader::UnpackUnsignedChar(OysterByte& bytes)
{
	unsigned char i = Packing::UnpackC(&bytes.GetByteArray()[size]);
	size += 1;
	return i;
}

short MessageHeader::UnpackShort(OysterByte& bytes)
{
	short i = Packing::Unpacks(&bytes.GetByteArray()[size]);
	size += 2;
	return i;
}

unsigned short MessageHeader::UnpackUnsignedShort(OysterByte& bytes)
{
	unsigned short i = Packing::UnpackS(&bytes.GetByteArray()[size]);
	size += 2;
	return i;
}

int MessageHeader::UnpackInt(OysterByte& bytes)
{
	int i = Packing::Unpacki(&bytes.GetByteArray()[size]);
	size += 4;
	return i;
}

unsigned int MessageHeader::UnpackUnsignedInt(OysterByte& bytes)
{
	unsigned int i = Packing::UnpackI(&bytes.GetByteArray()[size]);
	size += 4;
	return i;
}

__int64 MessageHeader::UnpackInt64(OysterByte& bytes)
{
	__int64 i = Packing::Unpacki64(&bytes.GetByteArray()[size]);
	size += 8;
	return i;
}

unsigned __int64 MessageHeader::UnpackUnsignedInt64(OysterByte& bytes)
{
	unsigned __int64 i = Packing::UnpackI64(&bytes.GetByteArray()[size]);
	size += 8;
	return i;
}

float MessageHeader::UnpackFloat(OysterByte& bytes)
{
	float i = Packing::Unpackf(&bytes.GetByteArray()[size]);
	size += 4;
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
	size += 8;
	return i;
}

std::string MessageHeader::UnpackStr(OysterByte& bytes)
{
	std::string str = Packing::UnpackStr(&bytes.GetByteArray()[size]);
	size += 2 + (int)str.length();
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