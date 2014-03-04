/////////////////////////////////////////////////
// Launcher to launch Danbias server or client //
/////////////////////////////////////////////////
#define NOMINMAX
#include <Windows.h>

#include "NoEdgeEditorWrapper.h"
#include "WindowShell.h"


int WINAPI WinMain( HINSTANCE hinst, HINSTANCE prevInst, PSTR cmdLine, int cmdShow)
{
	if(SetDllDirectory(L"..\\DLL") == FALSE)
	{
		return cmdShow;
	}
	WindowShell::WINDOW_INIT_DESC d;
	d.windowSize = cPOINT(800, 600);
	d.windowPosition = cPOINT(50, 50);
	
	WindowShell::CreateWin(d);
	
	NoEdgeEditorWrapper::EditorInitDesc desc;
	desc.mainOptions.height = 600;
	desc.mainOptions.width = 800;
	desc.mainOptions.renderWindow = WindowShell::GetHWND();
	if(NoEdgeEditorWrapper::Initiate(desc))
	{
		while (WindowShell::Frame())
		{
			NoEdgeEditorWrapper::Frame();
		}
		NoEdgeEditorWrapper::Release();
	}

	return 0;
}