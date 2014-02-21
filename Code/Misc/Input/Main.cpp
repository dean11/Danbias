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
#include "..\WindowManager\WindowShell.h"

using namespace std;

int main(int agrc, char*args)
{
	WindowShell::CreateWin(WindowShell::WINDOW_INIT_DESC());

	Input::Keyboard* app = Input::InputManager::Instance()->CreateKeyboardDevice();
	app->Deactivate();
	std::wstring text;
	app->BindTextTarget( &text );
	int oldLen = 0;

	while (WindowShell::Frame())
	{
		if(text.length() != oldLen)
		{
			wprintf(text.c_str());
		}
	}

	system("pause");
}