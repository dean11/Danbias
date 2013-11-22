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
			};

			struct ProtocolTest : public ProtocolHeader
			{
				std::string textMessage;
				ProtocolTest() { this->packageType = package_type_test; }
			};
			/*struct Prutt
			{
				PackageType t;
				union PRUTT
				{
					ProtocolTest *ptest,
				};
			};*/
		}
	}
}

#endif