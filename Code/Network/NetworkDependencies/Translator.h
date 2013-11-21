#ifndef NETWORK_DEPENDENCIES_TRANSLATOR_H
#define NETWORK_DEPENDENCIES_TRANSLATOR_H

#include "Messages/MessagesInclude.h"
#include "Protocols.h"
#include "ITranslate.h"

namespace Oyster
{
	namespace Network
	{
		class Translator : public ITranslate
		{
		public:
			Translator (){};
			~Translator(){};

			unsigned char* Translate (Protocols::ProtocolHeader &header );
			Protocols::ProtocolHeader& Translate (unsigned char msg[] ); 
		};
	}
}

#endif