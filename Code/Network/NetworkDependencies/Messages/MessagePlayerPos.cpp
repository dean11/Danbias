#include "MessagePlayerPos.h"

using namespace Oyster::Network;
using namespace Oyster::Network::Messages;
using namespace Oyster::Network::Protocols;

MessagePlayerPos::MessagePlayerPos()
{
}

MessagePlayerPos::~MessagePlayerPos()
{
}

void MessagePlayerPos::Pack(Protocols::ProtocolHeader& header, OysterByte& bytes)
{
	MessageHeader::Pack(header, bytes);

	PackInt(static_cast<ProtocolPlayerPos*>(&header)->ID, bytes);
	PackFloat(static_cast<ProtocolPlayerPos*>(&header)->matrix, static_cast<ProtocolPlayerPos*>(&header)->nrOfFloats, bytes);
	SetSize(bytes);
}

void MessagePlayerPos::Unpack(OysterByte& bytes, Protocols::ProtocolHeader& header)
{
	MessageHeader::Unpack(bytes, header);

	static_cast<ProtocolPlayerPos*>(&header)->ID = UnpackInt(bytes);
	static_cast<ProtocolPlayerPos*>(&header)->matrix = UnpackFloat(static_cast<ProtocolPlayerPos*>(&header)->nrOfFloats, bytes);
}