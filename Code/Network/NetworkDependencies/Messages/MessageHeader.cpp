#include "MessageHeader.h"
#include "../Packing.h"

using namespace Oyster::Network::Messages;
using namespace Oyster::Network::Packing;

MessageHeader::MessageHeader()
{
	size = 0;
	msg = new unsigned char[1024];
}

MessageHeader::~MessageHeader()
{
	delete[] msg;
}

void MessageHeader::Translate(/*Message& msg*/)
{
	size = 0;

	AddInt(4);
	AddInt(5);
	AddInt(6);
}

void MessageHeader::Translate(unsigned char message[])
{
	size = 0;

	int i = GetInt(message);
	int j = GetInt(message);
	int k = GetInt(message);
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