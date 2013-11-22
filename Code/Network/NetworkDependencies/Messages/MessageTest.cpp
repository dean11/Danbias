#include "MessageTest.h"

using namespace Oyster::Network::Messages;
using namespace Oyster::Network::Protocols;

MessageTest::MessageTest()
{
}

MessageTest::~MessageTest()
{
}

void MessageTest::Translate(ProtocolHeader& header, unsigned char msg[])
{
	MessageHeader::Translate(header, msg);

	AddStr(static_cast<ProtocolTest*>(&header)->textMessage, msg);
	SetSize(msg);
}

void MessageTest::Translate(unsigned char message[], ProtocolHeader& header)
{
	MessageHeader::Translate(message, header);

	static_cast<ProtocolTest*>(&header)->textMessage = GetStr(message);
}