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

	if(message != NULL)
	{
		message->Translate(header, this->msg);

		delete message;
		message = NULL;
	}

	return msg;
}

ProtocolSet* Translator::Translate(ProtocolSet* set, unsigned char msg[] )
{
	ProtocolHeader *header = new ProtocolHeader();
	MessageHeader *message = new MessageHeader();

	message->Translate(msg, *header);
	delete message;
	message = NULL;

	//Switch to the correct package.
	set->t = (PackageType)header->packageType;
	switch(set->t)
	{
	case package_type_header:
		message = new MessageHeader();
		set->Protocol.pHeader = new ProtocolHeader;
		message->Translate(msg, *set->Protocol.pHeader);
		break;

	case package_type_test:
		message = new MessageTest();
		set->Protocol.pTest = new ProtocolTest;
		message->Translate(msg, *set->Protocol.pTest);
		break;
	}

	if(message)
		delete message;
	delete header;

	return set;
}