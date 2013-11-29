#ifndef NETWORK_DEPENDENCIES_PROTOCOLS_H
#define NETWORK_DEPENDENCIES_PROTOCOLS_H

//////////////////////////////////////
// Created by Sam Svensson 2013 
// Holder structs for our protocols
// with the use of union.
// each packagetyp
// is linked to a protocol 
//////////////////////////////////////

#include <string>

namespace Oyster
{
	namespace Network
	{
		namespace Protocols
		{
			enum PackageType
			{
				PackageType_header,
				PackageType_test,
				PackageType_input,
				PackageType_update_position
			};

			struct ProtocolHeader
			{
				int size;
				int packageType;
				int clientID;

				ProtocolHeader() { this->packageType = PackageType_header; }
				virtual ~ProtocolHeader() {  }
			};

			struct ProtocolTest : public ProtocolHeader
			{
				std::string textMessage;
				unsigned int numOfFloats;
				float *f;

				ProtocolTest() { this->packageType = PackageType_test; }
				virtual ~ProtocolTest() { delete[] f; }
			};


			//Holding every protocol in an union.
			//Used because we now don't have to type case our protocol when we recieve them.
			class ProtocolSet
			{
			public:
				PackageType type;
				union
				{
					ProtocolHeader* pHeader;
					ProtocolTest *pTest;

				}Protocol;

				void Release()
				{
					switch(type)
					{
					case PackageType_header:
						if(Protocol.pHeader)
						{
							delete Protocol.pHeader;
						}
						break;
					case PackageType_test:
						if(Protocol.pTest)
						{
							delete Protocol.pTest;
						}
						break;
					}
				}
			};
		}
	}
}

#endif