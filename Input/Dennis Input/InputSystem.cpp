#include "RawInput.h"
#include <WindowsX.h>
#include <algorithm>
#include <ctime>

#include <fstream>
#include <iostream>


static RawInput* gInstance = 0;

template<typename FNC, typename PARAM> void DESTROY_LIST(SubscribeList<FNC, PARAM>* l)
{
	SubscribeList<FNC, PARAM>*	w = l;
	SubscribeList<FNC, PARAM>*	p = 0;

	while (w)
	{
		p = w;
		w = w->next;
		delete p;
	}
}
template<typename FNC, typename PARAM> void PROCESS_SUBSCRIBERS(SubscribeList<FNC, PARAM>* l)
{
	while (l)
	{
		l->fnc(l->param);
		l = l->next;
	}
}








RawInputWrapper*	RawInputWrapper::Self		()
{
	return (RawInputWrapper*)RawInput::Self();
}
void				RawInputWrapper::Destroy	()
{
	RawInput::Destroy();
}
RawInput*			RawInput::Self				()
{
	if(!gInstance)
		gInstance = new RawInput();

	return gInstance;
}
void				RawInput::Destroy			()
{
/************************ Delete subscribers ****************************/
	DESTROY_LIST(RawInput::Self()->_procInput);

/************************ Delete Other stuff ****************************/
	ShowCursor(true);
	RECT r;
	GetWindowRect(GetDesktopWindow(), &r);
	ClipCursor(&r);

/************************ Delete instance ****************************/
	delete gInstance;
	gInstance = NULL;
}





RawInput::RawInput								()
	: _procInput(0)
	, _enabled(1)
	, _mouseEnabled(1)
	, _KeyboardEnabled(1)
	, _exclusive(0)
	, _errorMsg(0)
	, _msgHook(SetWindowsHookEx(WH_GETMESSAGE, WM_INPUT_TRANSLATE, (HINSTANCE)0, GetCurrentThreadId()))
{
	if(!_msgHook)	this->_errorMsg = L"Failed to initiate window message hook";
}
RawInput::~RawInput								()
{}


const wchar_t* RawInput::Input_GetError() const								   
{
	return this->_errorMsg;
}

bool RawInput::Input_AddDevice(IN const HWND& targetApplication)
{
	assert(targetApplication != 0);
	static const UINT c = 2;
	RAWINPUTDEVICE devices[c] = 
	{ 
		{ 0x01, RawInput_Usage_keyboard, RIDEV_NOLEGACY, targetApplication },
		{ 0x01, RawInput_Usage_mouse, RIDEV_NOLEGACY | RIDEV_CAPTUREMOUSE, targetApplication }
	};

	if(! _addDevice( devices , c ) )	return false;

	ShowCursor(FALSE);
	//RECT r;
	//GetWindow
	//GetWindowRect(
	 
	return true;
}
bool RawInput::Input_AddDevice(IN const RAWINPUTDEVICE* d, const int& count)
{
	for (int i = 0; i < count; i++)
		if(!d[i].hwndTarget)
		{
			this->_errorMsg = L"Must specify target application";
			return false;
		}
	if(! _addDevice( d, count ) )	return false;

	return true;
}

//RAWINPUTDEVICE d = { 0x01, type, RIDEV_REMOVE, NULL };
//this->_errorMsg = L"Failed to unregister device";


void RawInput::Input_Disable()	   
{
	this->_enabled = false;
}
void RawInput::Input_Enable()	   
{
	this->_enabled = true;
}

void RawInput::Input_Read()
{
	//for (int i = 0; i < this->_idleKeyData.size(); i++)
	//	this->_proccessRawKeyboardData(this->_idleKeyData.pop());
	//for (int i = 0; i < this->_idleMouseData.size(); i++)
	//	this->_proccessRawMouseData(this->_idleMouseData.pop());
	//
	//this->_idleKeyData.clear();
	//this->_idleMouseData.clear();
}






bool RawInput::_addDevice						(const RAWINPUTDEVICE* k, const int& count)
{
	if(RegisterRawInputDevices(k, count, sizeof(RAWINPUTDEVICE)) == FALSE)
	{
		DWORD h = GetLastError();
		this->_errorMsg = L"Failed to register device";
		return false;
	}

	for (int q = 0; q < count; q++)
	{
		RawInputDeviceInstance i;
		memcpy(&i.description, &k[q], sizeof(RAWINPUTDEVICE));
		this->_deviceList.push(i);
	}

	return true;
}









