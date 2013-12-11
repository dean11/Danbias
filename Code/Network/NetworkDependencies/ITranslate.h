#ifndef NETWORK_DEPENDENCIES_I_TRANSLATE
#define NETWORK_DEPENDENCIES_I_TRANSLATE

//////////////////////////////////
// Created by Sam Svensson 2013 //
//////////////////////////////////

#include "../../Misc/Utilities.h"

namespace Oyster
{
	namespace Network
	{
		class CustomNetProtocol;
		class OysterByte;
		class ITranslate
		{
		public:
			//packs and unpacks packages for sending or recieving over the connection 
			virtual void Pack (Utility::DynamicMemory::SmartPointer<OysterByte> &bytes, Oyster::Network::CustomNetProtocol* protocol);
			virtual void Unpack (Oyster::Network::CustomNetProtocol* protocol, Utility::DynamicMemory::SmartPointer<OysterByte> &bytes);

		};
	}
}
#endif