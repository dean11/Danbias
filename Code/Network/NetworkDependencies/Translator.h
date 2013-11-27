#ifndef NETWORK_DEPENDENCIES_TRANSLATOR_H
#define NETWORK_DEPENDENCIES_TRANSLATOR_H

//////////////////////////////////
// Created by Sam Svensson 2013 //
//////////////////////////////////

#include "Messages/MessagesInclude.h"
#include "Protocols.h"
#include "ITranslate.h"
#include "OysterByte.h"

namespace Oyster
{
	namespace Network
	{
		class Translator : public ITranslate
		{
		public:
			Translator () { /*msg = new unsigned char[1601];*/ };
			~Translator() { /*if(msg != NULL) { delete [] this->msg; }*/ };

			void Pack (Protocols::ProtocolHeader &header, OysterByte& bytes );
			Protocols::ProtocolSet* Unpack (Protocols::ProtocolSet* set, OysterByte& bytes );

		private:
			//unsigned char* msg;
			//OysterByte bytes;

		};
	}
}

#endif