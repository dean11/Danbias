#ifndef NETWORK_DEPENDENCIES_TRANSLATOR_H
#define NETWORK_DEPENDENCIES_TRANSLATOR_H

//////////////////////////////////
// Created by Sam Svensson 2013 //
// ----------------------------//
// Packs our dynamic protocols  //
//////////////////////////////////

/*
	It packs a header in front of the actual message.
	Header looks like this:
	 - Size of the entire package
	 - String containing all the types of data that is packed in the package.

*/

/*
	Possible optimizing:
	If there are several of the same type of data in a row, 
	we can instead of saving a character for each type we can instead save a number and the character.

	Example:
	If we are packing 100 floats.
	"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF..."
	Instead of that we can do this:
	"100F"
*/

#ifdef CUSTOM_NET_PROTOCOL_EXPORT
	#define NET_PROTOCOL_EXPORT __declspec(dllexport)
#else
	#define NET_PROTOCOL_EXPORT __declspec(dllimport)
#endif

#include "../../Misc/Utilities.h"

namespace Oyster
{
	namespace Network
	{
		extern "C"
		{
			class OysterByte;
			class CustomNetProtocol;
			class NET_PROTOCOL_EXPORT Translator
			{
			public:
				Translator ();
				~Translator();
				Translator(const Translator& obj);
				const Translator& operator=(const Translator& obj);

				void Pack(Utility::DynamicMemory::SmartPointer<OysterByte> &bytes, CustomNetProtocol& protocol);

				//Returns false if it discovers any faulty stuff with the package.
				bool Unpack(CustomNetProtocol& protocol, Utility::DynamicMemory::SmartPointer<OysterByte> &bytes);

			private:
				struct PrivateData;
				PrivateData* privateData;
			};
		}
	}
}

#endif