#include "Translator.h"

using namespace Oyster::Network;
using namespace ::Protocols;
using namespace ::Messages;

void Translator::Pack( ProtocolHeader &header, OysterByte& bytes )
{
	MessageHeader *message = NULL;

	switch(header.packageType)
	{
	case PackageType_header:
		message = new MessageHeader();
		break;

	case PackageType_test:
		message = new MessageTest();
		break;

	case PackageType_player_pos:
		message = new MessagePlayerPos();
		break;
	}

	if(message != NULL)
	{
		message->Pack(header, bytes);

		delete message;
		message = NULL;
	}
}

void Translator::Unpack(ProtocolSet* set, OysterByte& bytes )
{
	ProtocolHeader *header = new ProtocolHeader();
	MessageHeader *message = new MessageHeader();

	message->Unpack(bytes, *header);
	delete message;
	message = NULL;

	//Switch to the correct package.
	set->type = (PackageType)header->packageType;
	switch(set->type)
	{
	case PackageType_header:
		message = new MessageHeader();
		set->Protocol.pHeader = new ProtocolHeader;
		message->Unpack(bytes, *set->Protocol.pHeader);
		break;

	case PackageType_test:
		message = new MessageTest();
		set->Protocol.pTest = new ProtocolTest;
		message->Unpack(bytes, *set->Protocol.pTest);
		break;

	case PackageType_player_pos:
		message = new MessagePlayerPos();
		set->Protocol.pPlayerPos = new ProtocolPlayerPos;
		message->Unpack(bytes, *set->Protocol.pPlayerPos);
		break;
	}

	if(message)
	{
		delete message;
	}
	delete header;

	//return set;
}