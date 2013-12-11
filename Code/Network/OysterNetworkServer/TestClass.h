#ifndef TEST_CLASS_H
#define TEST_CLASS_H
/*
#include "../../Misc/Utilities.h"
#include "../NetworkDependencies/OysterByte.h"
#include "../NetworkDependencies/PostBox.h"
#include "../NetworkAPI/NetworkClient.h"
#include "../NetworkAPI/NetworkServer.h"
//#include "../NetworkDependencies/Translator.h"
#include "../NetworkAPI/CustomNetProtocol.h"
#include "../NetworkDependencies/Protocols.h"
#include <vector>

class Test
{
public:
	Test();
	~Test();

	void mainLoop();

	virtual void ProcFunction(Oyster::Network::CustomNetProtocol& protocol);
	void PrintOutMessage(Oyster::Network::Protocols::ProtocolSet* set);

private:
	std::vector<Oyster::Network::NetworkClient*> clients;
	Oyster::Network::IPostBox<Utility::DynamicMemory::SmartPointer<Oyster::Network::OysterByte>> *recvPostBox;

	//Oyster::Network::Translator t;
	Oyster::Network::Protocols::ProtocolPlayerPos test;
	Utility::DynamicMemory::SmartPointer<Oyster::Network::OysterByte> sendBuffer;
	Utility::DynamicMemory::SmartPointer<Oyster::Network::OysterByte> recvBuffer;

	Oyster::Network::NetworkServer server;

};
*/
#endif