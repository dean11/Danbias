#include "MessageTest.h"

using namespace Oyster::Network::Messages;
using namespace Oyster::Network::Protocols;

MessageTest::MessageTest()
{
}

MessageTest::~MessageTest()
{
}

void MessageTest::Pack(ProtocolHeader& header, unsigned char msg[])
{
	MessageHeader::Pack(header, msg);

	PackStr(static_cast<ProtocolTest*>(&header)->textMessage, msg);
	PackFloat(static_cast<ProtocolTest*>(&header)->f, static_cast<ProtocolTest*>(&header)->numOfFloats, msg);
	SetSize(msg);
}

void MessageTest::Unpack(unsigned char msg[], ProtocolHeader& header)
{
	MessageHeader::Unpack(msg, header);

	static_cast<ProtocolTest*>(&header)->textMessage = UnpackStr(msg);
	static_cast<ProtocolTest*>(&header)->f = UnpackFloat(static_cast<ProtocolTest*>(&header)->numOfFloats, msg);
}