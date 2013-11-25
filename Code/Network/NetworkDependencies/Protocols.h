#ifndef NETWORK_DEPENDENCIES_PROTOCOLS_H
#define NETWORK_DEPENDENCIES_PROTOCOLS_H

//////////////////////////////////
// Created by Sam Svensson 2013 //
//////////////////////////////////

#include <string>

namespace Oyster
{
	namespace Network
	{
		namespace Protocols
		{
			enum PackageType
			{
				package_type_header,
				package_type_test,
				package_type_input,
				package_type_update_position
			};

			struct ProtocolHeader
			{
				int size;
				int packageType;
				int clientID;

				ProtocolHeader() { this->packageType = package_type_header; }  
			};

			struct ProtocolTest : public ProtocolHeader
			{
				std::string textMessage;
				ProtocolTest() { this->packageType = package_type_test; }
			};

			class ProtocolSet
			{
			public:
				PackageType t;
				union
				{
					ProtocolHeader* pHeader;
					ProtocolTest *pTest;

				}Protocol;

				void Release()
				{
					switch(t)
					{
					case package_type_header:
						if(Protocol.pHeader)
						{
							delete Protocol.pHeader;
						}
						break;
					case package_type_test:
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