#define NOMINMAX

#include <windows.h>
#include "OysterMath.h"

BOOL WINAPI DllMain( _In_  HINSTANCE hinstDLL,
					 _In_  DWORD fdwReason,
					 _In_  LPVOID lpvReserved )
{
	return ::Oyster::Math::IsSupported();
}