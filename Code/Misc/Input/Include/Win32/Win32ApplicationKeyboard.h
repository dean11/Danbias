/////////////////////////////////////////////////////////////////////
// Created by [Dan Andersson] [2014]
/////////////////////////////////////////////////////////////////////
#ifndef INPUT_WIN32_APPLICATION_KEBOARD_H
#define INPUT_WIN32_APPLICATION_KEBOARD_H

#define NOMINMAX
#include <Windows.h>
#include "..\ApplicationKeyboard.h"

namespace Input
{
	class Win32ApplicationKeyboard : public ApplicationKeyboard
	{
	public:
		Win32ApplicationKeyboard();
		~Win32ApplicationKeyboard();

		void CaptureText( UINT msg, WPARAM param );
	};
}

#endif // !INPUT_WIN32_APPLICATION_KEBOARD_H
