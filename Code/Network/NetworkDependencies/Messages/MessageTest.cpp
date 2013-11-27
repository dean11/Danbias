#include "MessageTest.h"

using namespace Oyster::Network;
using namespace Oyster::Network::Messages;
using namespace Oyster::Network::Protocols;

MessageTest::MessageTest()
{
}

MessageTest::~MessageTest()
{
}

void MessageTest::Pack(ProtocolHeader& header, OysterByte& bytes)
{
	MessageHeader::Pack(header, bytes);

	PackStr(static_cast<ProtocolTest*>(&header)->textMessage, bytes);
	PackFloat(static_cast<ProtocolTest*>(&header)->f, static_cast<ProtocolTest*>(&header)->numOfFloats, bytes);
	SetSize(bytes);
}

void MessageTest::Unpack(OysterByte& bytes, ProtocolHeader& header)
{
	MessageHeader::Unpack(bytes, header);

	static_cast<ProtocolTest*>(&header)->textMessage = UnpackStr(bytes);
	static_cast<ProtocolTest*>(&header)->f = UnpackFloat(static_cast<ProtocolTest*>(&header)->numOfFloats, bytes);
}