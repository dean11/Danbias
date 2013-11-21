//#include "Translator.h"
//
//using namespace Oyster::Network;
//using namespace ::Protocols;
//using namespace ::Messages;
//
//char* Translate ( const ProtocolHeader &header )
//{
//	MessageHeader *message;
//
//	switch(header.packageType)
//	{
//
//	case package_type_header:
//		message = new MessageHeader();
//		break;
//
//	case package_type_test:
//		message = new MessageTest();
//		break;
//	}
//
//	message->Translate(header);
//	return message->GetMsg();
//}
//
//ProtocolHeader& Translator::Translate( char msg[] )
//{
//	ProtocolHeader header;
//	MessageHeader *message = new MessageHeader();
//
//	header = message->translate(message);
//
//	switch(header.packageType)
//	{
//
//	case package_type_header:
//		message = new MessageHeader();
//		break;
//
//	case package_type_test:
//		message = new MessageTest();
//		break;
//	}
//
//	message->Translate(msg);
//	return message->GetProtocol();
//}


