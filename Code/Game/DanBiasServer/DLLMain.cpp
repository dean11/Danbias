
#include <windows.h>

BOOL WINAPI DllMain( _In_  HINSTANCE hinstDLL, _In_  DWORD fdwReason, _In_  LPVOID lpvReserved )
{
	MessageBox(0, L"DanBiasServer Loaded", 0, 0);
	return TRUE;
}