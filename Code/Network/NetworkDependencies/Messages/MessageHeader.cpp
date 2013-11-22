#include "MessageHeader.h"
#include "../Packing.h"

using namespace Oyster::Network::Messages;
using namespace Oyster::Network::Packing;
using namespace Oyster::Network::Protocols;

MessageHeader::MessageHeader()
{
	size = 0;

}

MessageHeader::~MessageHeader()
{
	
}

void MessageHeader::Translate(ProtocolHeader& header, unsigned char msg[] )
{
	size = 0;

	AddInt(header.clientID, msg);
	AddInt(header.packageType, msg);
	AddInt(header.size, msg);

	
}

void MessageHeader::Translate(unsigned char message[], ProtocolHeader& header)
{
	size = 0;

	header.clientID = GetInt(message);
	header.packageType = GetInt(message);
	header.size = GetInt(message);
}


void MessageHeader::AddInt(int i, unsigned char msg[])
{
	Pack(&msg[size], i);
	size += 4;
}

void MessageHeader::AddStr(std::string str, unsigned char msg[])
{
	Pack(&msg[size], str);
	size += 2 + str.length();
}

int MessageHeader::GetInt(unsigned char message[])
{
	int i = Unpacki(&message[size]);
	size += 4;
	return i;
}

std::string MessageHeader::GetStr(unsigned char message[])
{
	std::string std = UnpackStr(&message[size]);
	size += 2 + std.length();
	return std;
}