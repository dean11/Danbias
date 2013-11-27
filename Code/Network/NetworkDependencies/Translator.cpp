#include "Translator.h"

using namespace Oyster::Network;
using namespace ::Protocols;
using namespace ::Messages;

void Translator::Pack( ProtocolHeader &header, OysterByte& bytes )
{
	MessageHeader *message = NULL;

	switch(header.packageType)
	{
	case package_type_header:
		message = new MessageHeader();
		break;

	case package_type_test:
		message = new MessageTest();
		break;
	}

	if(message != NULL)
	{
		message->Pack(header, bytes);

		delete message;
		message = NULL;
	}
}

ProtocolSet* Translator::Unpack(ProtocolSet* set, OysterByte& bytes )
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
	case package_type_header:
		message = new MessageHeader();
		set->Protocol.pHeader = new ProtocolHeader;
		message->Unpack(bytes, *set->Protocol.pHeader);
		break;

	case package_type_test:
		message = new MessageTest();
		set->Protocol.pTest = new ProtocolTest;
		message->Unpack(bytes, *set->Protocol.pTest);
		break;
	}

	if(message)
	{
		delete message;
	}
	delete header;

	return set;
}