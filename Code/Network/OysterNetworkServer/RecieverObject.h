#ifndef OYSTER_NETWORK_SERVER_RECIEVER_OBJECT_H
#define OYSTER_NETWORK_SERVER_RECIEVER_OBJECT_H

/////////////////////////////////////
// Created by Pontus Fransson 2013 //
/////////////////////////////////////

#include "../NetworkDependencies/Protocols.h"
#include "../NetworkDependencies/OysterByte.h"
#include "../../Misc/Utilities.h"

class RecieverObject
{
public:
	virtual void ProcFunc(Utility::DynamicMemory::SmartPointer<Oyster::Network::OysterByte> msg) = 0;

};

#endif