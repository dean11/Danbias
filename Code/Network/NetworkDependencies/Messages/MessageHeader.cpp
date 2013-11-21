#include "MessageHeader.h"
#include "../Packing.h"

using namespace Oyster::Network::Messages;
using namespace Oyster::Network::Packing;
using namespace Oyster::Network::Protocols;

MessageHeader::MessageHeader()
{
	size = 0;
	msg = new unsigned char[1024];
}

MessageHeader::~MessageHeader()
{
	delete[] msg;
}

void MessageHeader::Translate(ProtocolHeader& header)
{
	size = 0;

	AddInt(header.clientID);
	AddInt(header.packageType);
	AddInt(header.size);
}

void MessageHeader::Translate(unsigned char message[], ProtocolHeader& header)
{
	size = 0;

	header.clientID = GetInt(message);
	header.packageType = GetInt(message);
	header.size = GetInt(message);
}

unsigned char* MessageHeader::GetMsg()
{
	return msg;
}

void MessageHeader::AddInt(int i)
{
	Pack(&msg[size], i);
	size += 4;
}

void MessageHeader::AddStr(std::string str)
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