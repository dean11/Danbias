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
			Translator () { msg = new unsigned char[256]; };
			~Translator() { if(msg != NULL) { delete [] this->msg; }};

			unsigned char* Translate (Protocols::ProtocolHeader &header );
			Protocols::ProtocolSet* Translate (Protocols::ProtocolSet* set, unsigned char msg[] );

		private:
			unsigned char* msg;

		};
	}
}

#endif