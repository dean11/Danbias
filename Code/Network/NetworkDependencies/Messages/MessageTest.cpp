#include "MessageTest.h"

using namespace Oyster::Network::Messages;
using namespace Oyster::Network::Protocols;

MessageTest::MessageTest()
{
	
}

MessageTest::~MessageTest()
{
}

void MessageTest::Translate(ProtocolHeader& header)
{
	MessageHeader::Translate(header);

	AddStr(static_cast<ProtocolTest*>(&header)->textMessage);
}

void MessageTest::Translate(unsigned char message[], ProtocolHeader& header)
{
	MessageHeader::Translate(message, header);

	static_cast<ProtocolTest*>(&header)->textMessage = GetStr(message);
}