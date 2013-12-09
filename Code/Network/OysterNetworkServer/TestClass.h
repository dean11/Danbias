#ifndef TEST_CLASS_H
#define TEST_CLASS_H

#include "RecieverObject.h"
#include "../../Misc/Utilities.h"
#include "../NetworkDependencies/OysterByte.h"
#include "../NetworkDependencies/PostBox.h"
#include "IClient.h"
#include "../NetworkDependencies/Translator.h"
#include <vector>

class Test : public RecieverObject
{
public:
	Test();
	~Test();

	void mainLoop();

	virtual void ProcFunc(Utility::DynamicMemory::SmartPointer<Oyster::Network::OysterByte> msg);
	void PrintOutMessage(Oyster::Network::Protocols::ProtocolSet* set);

private:
	std::vector<IClient*> clients;
	Oyster::Network::IPostBox<Utility::DynamicMemory::SmartPointer<Oyster::Network::OysterByte>> *recvPostBox;

	Oyster::Network::Translator t;

	Oyster::Network::Protocols::ProtocolPlayerPos test;


};

#endif