#include "MessageHeader.h"
#include "../Packing.h"

using namespace Oyster::Network::Messages;
using namespace Oyster::Network::Protocols;

MessageHeader::MessageHeader()
{
	size = 0;
}

MessageHeader::~MessageHeader()
{
}

void MessageHeader::Pack(ProtocolHeader& header, unsigned char msg[] )
{
	size = 0;
	
	PackInt(header.size, msg);
	PackInt(header.packageType, msg);
	PackInt(header.clientID, msg);
	SetSize(msg);
}

void MessageHeader::Unpack(unsigned char msg[], ProtocolHeader& header)
{
	size = 0;

	header.clientID = UnpackInt(msg);
	header.packageType = UnpackInt(msg);
	header.size = UnpackInt(msg);
}

/**************************
		Pack
**************************/

void MessageHeader::PackBool(bool i, unsigned char msg[])
{
	Packing::Pack(&msg[size], i);
	size += 1;
}

void MessageHeader::PackChar(char i, unsigned char msg[])
{
	Packing::Pack(&msg[size], i);
	size += 1;
}

void MessageHeader::PackUnsignedChar(unsigned char i, unsigned char msg[])
{
	Packing::Pack(&msg[size], i);
	size += 1;
}

void MessageHeader::PackShort(short i, unsigned char msg[])
{
	Packing::Pack(&msg[size], i);
	size += 2;
}

void MessageHeader::PackUnsignedShort(unsigned short i, unsigned char msg[])
{
	Packing::Pack(&msg[size], i);
	size += 2;
}

void MessageHeader::PackInt(int i, unsigned char msg[])
{
	Packing::Pack(&msg[size], i);
	size += 4;
}

void MessageHeader::PackUnsignedInt(unsigned int i, unsigned char msg[])
{
	Packing::Pack(&msg[size], i);
	size += 4;
}

void MessageHeader::PackInt64(__int64 i, unsigned char msg[])
{
	Packing::Pack(&msg[size], i);
	size += 8;
}

void MessageHeader::PackUnsignedInt64(unsigned __int64 i, unsigned char msg[])
{
	Packing::Pack(&msg[size], i);
	size += 8;
}

void MessageHeader::PackFloat(float i, unsigned char msg[])
{
	Packing::Pack(&msg[size], i);
	size += 4;
}

void MessageHeader::PackFloat(float i[], unsigned int elementCount, unsigned char msg[])
{
	//Pack number of elements
	PackUnsignedInt(elementCount, msg);

	//Pack all elements
	for(int j = 0; j < elementCount; j++)
	{
		PackFloat(i[j], msg);
	}
}

void MessageHeader::PackDouble(double i, unsigned char msg[])
{
	Packing::Pack(&msg[size], i);
	size += 8;
}

void MessageHeader::PackStr(char str[], unsigned char msg[])
{
	Packing::Pack(&msg[size], str);
	size += 2 + strlen(str);
}

void MessageHeader::PackStr(std::string str, unsigned char msg[])
{
	Packing::Pack(&msg[size], str);
	size += 2 + str.length();
}

/**************************
		Unpack
**************************/

bool MessageHeader::UnpackBool(unsigned char msg[])
{
	bool i = Packing::Unpackb(&msg[size]);
	size += 1;
	return i;
}

char MessageHeader::UnpackChar(unsigned char msg[])
{
	char i = Packing::Unpackc(&msg[size]);
	size += 1;
	return i;
}

unsigned char MessageHeader::UnpackUnsignedChar(unsigned char msg[])
{
	unsigned char i = Packing::UnpackC(&msg[size]);
	size += 1;
	return i;
}

short MessageHeader::UnpackShort(unsigned char msg[])
{
	short i = Packing::Unpacks(&msg[size]);
	size += 2;
	return i;
}

unsigned short MessageHeader::UnpackUnsignedShort(unsigned char msg[])
{
	unsigned short i = Packing::UnpackS(&msg[size]);
	size += 2;
	return i;
}

int MessageHeader::UnpackInt(unsigned char msg[])
{
	int i = Packing::Unpacki(&msg[size]);
	size += 4;
	return i;
}

unsigned int MessageHeader::UnpackUnsignedInt(unsigned char msg[])
{
	unsigned int i = Packing::UnpackI(&msg[size]);
	size += 4;
	return i;
}

__int64 MessageHeader::UnpackInt64(unsigned char msg[])
{
	__int64 i = Packing::Unpacki64(&msg[size]);
	size += 8;
	return i;
}

unsigned __int64 MessageHeader::UnpackUnsignedInt64(unsigned char msg[])
{
	unsigned __int64 i = Packing::UnpackI64(&msg[size]);
	size += 8;
	return i;
}

float MessageHeader::UnpackFloat(unsigned char msg[])
{
	float i = Packing::Unpackf(&msg[size]);
	size += 4;
	return i;
}

float* MessageHeader::UnpackFloat(unsigned int& elementCount, unsigned char msg[])
{
	float* i;

	elementCount = UnpackUnsignedInt(msg);

	i = new float[elementCount];
	for(int j = 0; j < elementCount; j++)
	{
		i[j] = UnpackFloat(msg);
	}

	return i;
}

double MessageHeader::UnpackDouble(unsigned char msg[])
{
	double i = Packing::Unpackd(&msg[size]);
	size += 8;
	return i;
}

std::string MessageHeader::UnpackStr(unsigned char msg[])
{
	std::string str = Packing::UnpackStr(&msg[size]);
	size += 2 + str.length();
	return str;
}

void MessageHeader::SetSize(unsigned char msg[])
{
	Packing::Pack(&msg[0], size);
}