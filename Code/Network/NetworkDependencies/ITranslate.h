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
			//packs and unpacks packages for sending or recieving over the connection 
			virtual void Pack (Protocols::ProtocolHeader &header, OysterByte& bytes) = 0;
			virtual void Unpack (Protocols::ProtocolSet* set, OysterByte& bytes ) = 0;

		};
	}
}
#endif