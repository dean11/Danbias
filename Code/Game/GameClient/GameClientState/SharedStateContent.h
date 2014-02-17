/********************************************************************
 * Created by Dan Andersson, 2014
 * Contains a shared memory struct.
 * Whihc reference passed seamlessly between the game states
 * at their Init calls.
 ********************************************************************/

#ifndef SHAREDSTATECONTENT_H
#define SHAREDSTATECONTENT_H

#include <map>
#include "Utilities.h"
#include "C_Object.h"
#include "NetworkClient.h"
#include "L_inputClass.h"

namespace DanBias { namespace Client
{
	struct SharedStateContent
	{
	public:
		::std::map<int, ::Utility::DynamicMemory::UniquePointer<::DanBias::Client::C_Object>> scene;
		::Oyster::Network::NetworkClient *network;
		InputClass* input;
	};
} }

#endif