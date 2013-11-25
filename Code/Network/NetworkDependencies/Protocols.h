#ifndef NETWORK_DEPENDENCIES_PROTOCOLS_H
#define NETWORK_DEPENDENCIES_PROTOCOLS_H

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
				virtual ~ProtocolHeader() {  }
			};

			struct ProtocolTest : public ProtocolHeader
			{
				std::string textMessage;
				unsigned int numOfFloats;
				float *f;

				ProtocolTest() { this->packageType = package_type_test; }
				virtual ~ProtocolTest() { delete[] f; }
			};

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