#include <atomic>
#include <vector>
#include <iostream>
#include <chrono>
#include <ctime>
#include <map>
#include <thread>
#include <chrono>
#include <iomanip>
#include <mutex>
#include <bitset>
#include <Windows.h>

#include "Include\Input.h"
#include "WindowShell.h"

using namespace std;
using namespace Input;
using namespace Input::Enum;

Input::Keyboard* keyboard = 0;	
Input::Mouse* mouse = 0;		

void KeyPress(Input::Enum::SAKI key, Input::Keyboard* sender)
{
	if(key == SAKI_A)
	{
		if(mouse->IsActive())	mouse->Deactivate();
		else					mouse->Activate();
		
	}
}

void MouseVelocity(Input::Struct::SAIPointInt2D vel, Input::Mouse* sender)
{
	int i = vel.Length();
	if(abs(i) > 2)
		i = 0;
}

int WINAPI WinMain( HINSTANCE hinst, HINSTANCE prevInst, PSTR cmdLine, int cmdShow)
{
	std::wstring text;

	WindowShell::CreateWin(WindowShell::WINDOW_INIT_DESC());
	WindowShell::CreateConsoleWindow();
	keyboard = Input::InputManager::Instance()->CreateKeyboardDevice(WindowShell::GetHWND());
	mouse = Input::InputManager::Instance()->CreateMouseDevice(WindowShell::GetHWND());

	mouse->AddOnMouseMoveVelocityCallback(MouseVelocity);
	keyboard->BindTextTarget( &text );
	keyboard->AddOnKeyPressCallback(KeyPress);

	int oldLen = 0;

	while (WindowShell::Frame())
	{
		if(text.length() != oldLen)
		{
			wprintf(text.c_str());
			oldLen =text.length();
		}
	}

	system("pause");

	return cmdShow;
}