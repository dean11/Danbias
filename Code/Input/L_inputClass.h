//////////////////////////////////////////////////////////////////////////
// Temp input handler, not stable!
// When starting the program, don't click anywhere until the program starts  
// because that breaks the input..
//////////////////////////////////////////////////////////////////////////

#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>



class InputClass
{
private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	bool ReadKeyboard();
	bool ReadMouse();

public:

	InputClass();
	~InputClass();
	
	// Initialize DirectInput8Create and acquire the mouse and keyboard
	bool Initialize(HINSTANCE, HWND, int, int); 

	//read the mouse and keyboard and send back 
	// delta mouse pos and if any button is pressed
	bool Update();

	bool IsKeyPressed(int key);
	bool IsMousePressed();

	// Call if mouse is pressed
	float GetYaw();
	float GetPitch();

};

#endif