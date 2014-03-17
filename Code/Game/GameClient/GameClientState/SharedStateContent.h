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
#include "C_obj\C_StaticObj.h"
#include "C_obj\C_DynamicObj.h"
#include "C_obj\C_Player.h"
#include "C_Light.h"
#include "C_obj/C_ClientLogic.h"
#include "NetworkClient.h"
#include "Input.h"
#include "FirstPersonWeapon.h"
#include "DllInterfaces\GFXAPI.h"
#include "Sound\C_AudioHandler.h"

namespace DanBias { namespace Client
{
	struct SharedStateContent
	{
	public:
		::std::map<int, ::Utility::DynamicMemory::UniquePointer<::DanBias::Client::C_StaticObj>> staticObjects;
		::std::map<int, ::Utility::DynamicMemory::UniquePointer<::DanBias::Client::C_DynamicObj>> dynamicObjects;
		::std::map<int, ::Utility::DynamicMemory::UniquePointer<::DanBias::Client::C_Light>> lights;
		::std::map<int, ::Utility::DynamicMemory::UniquePointer<::DanBias::Client::C_StaticObj>> pickups;
		::std::map<int, ::Utility::DynamicMemory::UniquePointer<::DanBias::Client::C_Player>> weapons;
		::std::map<int, ::Utility::DynamicMemory::UniquePointer<::DanBias::Client::C_ClientLogic>> clientObjects;
		C_AudioHandler* soundManager;
		FirstPersonWeapon* weapon;

		Oyster::Graphics::API::Texture		background;
		::Oyster::Network::NetworkClient	*network;
		::Input::Keyboard					*keyboardDevice;
		::Input::Mouse						*mouseDevice;
		::Oyster::Graphics::API::Texture	mouseCursor;
		float								mouseSensitivity;
		std::string							defaultIP;
	};
} }

#endif