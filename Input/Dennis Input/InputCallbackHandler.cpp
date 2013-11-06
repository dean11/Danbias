#include"RawInput.h"


//DefRawInputProc
//GetRawInputDeviceList
//GetRegisteredRawInputDevices



LRESULT CALLBACK RawInput::WM_INPUT_TRANSLATE	(int nCode, WPARAM wParam, LPARAM lparam)
{
	if (nCode < 0)		return CallNextHookEx(RawInput::Self()->_msgHook, nCode, wParam, lparam); 


	MSG *m = (MSG*)lparam;
	
	if(m->message == WM_INPUT)
	{
		RAWINPUT* raw = RawInput::Self()->_TranslateRawInput(m->lParam);

		if(!raw) goto nextHook;
		if(!RawInput::Self()->Self()->_enabled)
		{
			if(FAILED ( DefRawInputProc(&raw, 1, sizeof(RAWINPUTHEADER)) ) )	
				RawInput::Self()->_errorMsg = L"Failed to proccess default raw input";
			goto _final;
		}
		//	 if(raw->header.dwType == RIM_TYPEMOUSE)	RawInput::Self()->_idleMouseData.insert(raw->data.mouse);
		//else if(raw->header.dwType == RIM_TYPEKEYBOARD)	RawInput::Self()->_proccessRawKeyboardData(raw->data.keyboard);
		

		_final:

		//if(FAILED ( DefRawInputProc(&raw, 1, sizeof(RAWINPUTHEADER)) ) )
		//		RawInput::Self()->_errorMsg = L"Failed to proccess default raw input";

		delete raw;
	}
	else if (m->message == WM_QUIT)
	{
		if(UnhookWindowsHookEx(RawInput::Self()->_msgHook) == FALSE)
		{
			RawInput::Self()->_errorMsg = L"Failed to unhook message hook!";
		}
	}


	nextHook:
	return CallNextHookEx(RawInput::Self()->_msgHook, nCode, wParam, lparam); 
}


RAWINPUT* RawInput::_TranslateRawInput			(LPARAM l)
{
	//Get The size of the raw data buffer
	UINT bufferSize;
	GetRawInputData((HRAWINPUT)l, RID_INPUT, NULL, &bufferSize, sizeof(RAWINPUTHEADER));
	if (bufferSize < 1) 
	{
		//Something went wrong
		RawInput::Self()->_errorMsg = L"Failed to read raw buffer data in input class";
		return 0;
	} 

	//Create and read the raw input data
	LPBYTE rawBufferIn = new BYTE[bufferSize];
	UINT readBytes = GetRawInputData((HRAWINPUT)l, RID_INPUT, rawBufferIn, &bufferSize,  sizeof(RAWINPUTHEADER));
	if ( readBytes != bufferSize )
	{
		RawInput::Self()->_errorMsg = L"Could not read raw input data";
		delete [] rawBufferIn;
		return 0;
	}

	return (RAWINPUT*)rawBufferIn;
}
void RawInput::_proccessRawKeyboardData			(RAWKEYBOARD& k)
{
	if(!this->_KeyboardEnabled)	return;

	//The key is released.
	if(k.Flags == RI_KEY_BREAK || k.Flags == (RI_KEY_BREAK | RI_KEY_E0) || k.Flags == (RI_KEY_BREAK | RI_KEY_E1))
	{
		if(k.Message == WM_SYSKEYUP)
		{
			//	 if(k.VKey == VK_CONTROL)	this->_procCollection.skd.ctrl = false;
			//else if(k.VKey == VK_MENU)		this->_procCollection.skd.alt = false;
			//else if(k.VKey == VK_SHIFT)		this->_procCollection.skd.shift = false;
		}
		else if(k.Message == WM_KEYUP)
		{
			//this->_procCollection.kd.key = (RIK)k.VKey;
			//this->_procCollection.kd.released = true;
		}
	}
	//The key is pressed.
	else if (k.Flags == RI_KEY_MAKE || k.Flags == (RI_KEY_MAKE | RI_KEY_E0) || k.Flags == (RI_KEY_MAKE | RI_KEY_E1))
	{
		if(k.Message == WM_SYSKEYDOWN)
		{
			//	 if(k.VKey == VK_CONTROL)	this->_procCollection.skd.ctrl = true;
			//else if(k.VKey == VK_MENU)		this->_procCollection.skd.alt = true;
			//else if(k.VKey == VK_SHIFT)		this->_procCollection.skd.shift = true;
		}
		else if(k.Message == WM_KEYDOWN)
		{
			//this->_procCollection.kd.key = (RIK)k.VKey;
			//this->_procCollection.kd.released = false;
		}
	}
}
void RawInput::_proccessRawMouseData			(RAWMOUSE& m)
{
	if(!this->_mouseEnabled)	return;

	if(m.lLastX != 0 || m.lLastY != 0)
	{
		//this->_procCollection.mmd.MousePos_X += m.lLastX;
		//this->_procCollection.mmd.MousePos_Y += m.lLastY;
	}
	if( m.usButtonFlags > 0 )
	{

		switch (m.usButtonFlags)
		{
			//Mouse button pressed
			case RI_MOUSE_LEFT_BUTTON_DOWN:
			case RI_MOUSE_RIGHT_BUTTON_DOWN:
			case RI_MOUSE_MIDDLE_BUTTON_DOWN:
			{
				if(m.usButtonFlags == RI_MOUSE_LEFT_BUTTON_DOWN)	
				{
					//MOUSE_INPUT_btnData.key = RIK_LeftBtn;
					//LBTNDOWN = true;
				}
				else if(m.usButtonFlags == RI_MOUSE_MIDDLE_BUTTON_DOWN)	
				{	
					//MOUSE_INPUT_btnData.key = RIK_MiddleBtn;
					//MBTNDOWN = true;
				}
				else if(m.usButtonFlags == RI_MOUSE_RIGHT_BUTTON_DOWN)	
				{	
					//MOUSE_INPUT_btnData.key = RIK_RightBtn;
					//RBTNDOWN = true;
				}
			}
			break;

			//Mouse button Released
			case RI_MOUSE_LEFT_BUTTON_UP:
			case RI_MOUSE_RIGHT_BUTTON_UP:
			case RI_MOUSE_MIDDLE_BUTTON_UP:
			{
				if(m.usButtonFlags == RI_MOUSE_LEFT_BUTTON_UP)	
				{
					//MOUSE_INPUT_btnData.key = RIK_LeftBtn;
					//LBTNDOWN = false;
				}
				else if(m.usButtonFlags == RI_MOUSE_MIDDLE_BUTTON_UP)	
				{
					//MOUSE_INPUT_btnData.key = RIK_MiddleBtn;
					//MBTNDOWN = false;
				}
				else if(m.usButtonFlags == RI_MOUSE_RIGHT_BUTTON_UP)	
				{
					//MOUSE_INPUT_btnData.key = RIK_RightBtn;
					//RBTNDOWN = false;
				}
			}
			break;
			
			case RI_MOUSE_WHEEL:
			{
				int d = ((int)m.usButtonData);

				if(d > 120)	d = -1;
				else		d = 1;
				
				//this->_procCollection.wd = d;
			}
			break;
		}
	}
}









