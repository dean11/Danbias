#include "MessageTest.h"

using namespace Oyster::Network::Messages;

MessageTest::MessageTest()
{
	
}

MessageTest::~MessageTest()
{
}

void MessageTest::Translate()
{
	MessageHeader::Translate();

	AddStr("Hej hur mår du idag?");
}

void MessageTest::Translate(unsigned char message[])
{
	MessageHeader::Translate(message);

	std::string str = GetStr(message);
}