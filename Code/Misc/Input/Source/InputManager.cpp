/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#include "..\Include\InputManager.h"
#include "..\Include\PreReq.h"


//-- PLATFORM ----------------------------------------------------------------------------------------------------------------------------------//
//Bring in correct Input for current platform
#if defined PLATFORM_WIN32
#  include "..\Include\Win32\Win32Input.h"
#elif defined PLATFORM_LINUX
#  include "..\Include\Linux\LinuxInput.h"
#elif defined PLATFORM_APPLE
#  include "..\Include\Mac\MacInput.h"
#elif defined PLATFORM_IPHONE
#  include "..\Include\IPhone\iPhoneInput.h"
#elif defined PLATFORM_XBOX
#  include "..\Include\XBox\XBoxInput.h"
#endif
//----------------------------------------------------------------------------------------------------------------------------------------------//

using namespace Input;

InputManager *defaultInstance = 0;

InputManager* CreateManager()
{
	InputManager* obj = 0;
#if defined PLATFORM_WIN32
	obj = new Win32Input();
#elif defined PLATFORM_XBOX
	obj = new XBoxInput();
#elif defined PLATFORM_LINUX
	obj = new LinuxInput();
#elif defined PLATFORM_APPLE
	obj = new MacInput();
#elif defined PLATFORM_IPHONE
	obj = new iPhoneInput();
#else
	OIS_EXCEPT(L"No platform library.. check build platform defines!");
#endif 
	return obj;
}

//-- STATIC ------------------------------------------------------------------------------------------------------------------------------------//
InputManager* InputManager::Instance()
{
	if(!defaultInstance)
	{
		defaultInstance = CreateManager();
	}

	return defaultInstance;
}
InputManager* InputManager::CreateInputManager()
{
	return CreateManager();
}
void InputManager::DestroyInputManager()
{
	if(!defaultInstance) return;

	defaultInstance->Destroy();
	delete defaultInstance;
	defaultInstance = 0;
}
void InputManager::DestroyInputManager(InputManager* inputSystem)
{
	if(!inputSystem) return;

	inputSystem->Destroy();
	delete inputSystem;
	inputSystem = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------//


//-- PRIVATE ------------------------------------------------------------------------------------------------------------------------------------//
InputManager::InputManager()
{}
InputManager::~InputManager()
{}





