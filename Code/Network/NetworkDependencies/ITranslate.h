#ifndef NETWORK_DEPENDENCIES_I_TRANSLATE
#define NETWORK_DEPENDENCIES_I_TRANSLATE

//////////////////////////////////
// Created by Sam Svensson 2013 //
//////////////////////////////////

namespace Oyster
{
	namespace Network
	{
		class ITranslate
		{

		public:
			virtual unsigned char* Pack (Protocols::ProtocolHeader &header ) = 0;
			virtual Protocols::ProtocolSet* Unpack (Protocols::ProtocolSet* set, unsigned char message[] ) = 0;

		};
	}
}
#endif