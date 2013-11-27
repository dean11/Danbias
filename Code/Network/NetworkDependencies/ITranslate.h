#ifndef NETWORK_DEPENDENCIES_I_TRANSLATE
#define NETWORK_DEPENDENCIES_I_TRANSLATE

//////////////////////////////////
// Created by Sam Svensson 2013 //
//////////////////////////////////

namespace Oyster
{
	namespace Network
	{
		class OysterByte;
		class ITranslate
		{

		public:
			virtual void Pack (Protocols::ProtocolHeader &header, OysterByte& bytes) = 0;
			virtual Protocols::ProtocolSet* Unpack (Protocols::ProtocolSet* set, OysterByte& bytes ) = 0;

		};
	}
}
#endif