#include "Translator.h"

using namespace Oyster::Network;
using namespace ::Protocols;
using namespace ::Messages;

unsigned char* Translator::Translate( ProtocolHeader &header )
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

	message->Translate(header);

	return message->GetMsg();
}

ProtocolHeader* Translator::Translate(unsigned char msg[] )
{
	ProtocolHeader *header = new ProtocolHeader();
	MessageHeader *message = new MessageHeader();

	message->Translate(msg, *header);

	switch(header->packageType)
	{
	case package_type_header:
		message = new MessageHeader();
		header = new ProtocolHeader();
		break;

	case package_type_test:
		message = new MessageTest();
		header = new ProtocolTest();
		break;
	}

	message->Translate(msg, *header);
	return header;
}