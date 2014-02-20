/////////////////////////////////////////////////////////////////////
// Created by [Dan Andersson] [2014]
/////////////////////////////////////////////////////////////////////
#ifndef INPUT_WIN32_APPLICATION_KEBOARD_H
#define INPUT_WIN32_APPLICATION_KEBOARD_H

#include "..\ApplicationKeyboard.h"
//#include <Windows.h>

namespace Input
{
	class Win32ApplicationKeyboard :public AplicationKeyboard
	{
	public:
		Win32ApplicationKeyboard();
		~Win32ApplicationKeyboard();


	private:

	};
}

#endif // !INPUT_WIN32_APPLICATION_KEBOARD_H
