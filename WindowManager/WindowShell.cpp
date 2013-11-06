#include "WindowShell.h"
#include <vector>


struct ChildWin;
struct _PrSt;

#pragma region Declarations

	namespace
	{
		//Private data
		static WindowShell* instance = NULL;
		int childIdCounter = 0;
		_PrSt *pData = NULL;
	}

	struct ChildWin
	{
		int id;
		HWND hWnd;

		ChildWin()
		{
			hWnd = NULL;
			childIdCounter++;
			id = childIdCounter;
		}
		int ID() const { return id; }
	};
	struct _PrSt
	{
		HINSTANCE							hIns;
		HWND								hWnd;
		std::vector<ChildWin>				childWindows;

		_PrSt()
		{
			hIns = NULL;
			hWnd = NULL;
		}
	};

#pragma endregion



WindowShell::WindowShell()
{
	pData = new _PrSt();
}
WindowShell::~WindowShell()
{
	delete pData;
}



bool WindowShell::createWin(INIT_DESC_WINDOW &desc)
{
	if(pData->hWnd)
	{
		MessageBox(0, L"There is already a window registered\nPlease use child windows to create more windows!" ,L"Error", 0);
		return false;
	}
	if(!desc.windowProcCallback)
	{
		MessageBox(0, L"No callback function for window messages was found!" ,L"Error", 0);
		return false;
	}
	if(!desc.hInstance)
	{
		MessageBox(0, L"No HINSTANCE was specified!" ,L"Error", 0);
		return false;
	}
	if(desc.windowSize < 0)
	{
		MessageBox(0, L"Size specified for window is invalid!" ,L"Error", 0);
	}


	pData->hIns = desc.hInstance;


		#pragma region Register
	
	WNDCLASSEX wc;
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.hIconSm		= NULL;
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = desc.windowProcCallback; 
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = pData->hIns;
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = L"MainWindowClass";

	if( !RegisterClassEx(&wc) )
	{
		MessageBox(0, L"Failed to register class", L"Initialization error", 0);
		return false;
	}
		#pragma endregion
	

		#pragma region Create window


	pData->hWnd = CreateWindow(	
								L"MainWindowClass" , 
								desc.windowName.c_str(),
								WS_OVERLAPPEDWINDOW, 
								desc.windowPosition.x, 
								desc.windowPosition.y, 
								desc.windowSize.x, 
								desc.windowSize.y, 
								0, 
								0, 
								pData->hIns, 
								0
							  ); 
	
	if( !pData->hWnd )
	{
		MessageBox(0, L"Failed to create window", L"Error!", 0);
		return false;
	}

		#pragma endregion

	
	//Show and update window
	ShowWindow(pData->hWnd, SW_SHOW);
	UpdateWindow(pData->hWnd);
	
	return true;
}
int WindowShell::createChildWin(INIT_DESC_CHILD_WINDOW &desc)
{
	ChildWin win;


	char idStr[3];
	_itoa_s(win.id, idStr, 10);
	std::string next = idStr;
	std::wstring str = std::wstring(next.begin(), next.end());
	std::wstring childClassName = L"ChildWindow_";
	childClassName += str;

	WNDCLASSEX wcex;
	wcex.cbSize	= sizeof(WNDCLASSEX);
	wcex.style = CS_VREDRAW;
	wcex.lpfnWndProc = desc.windowProcCallback;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = pData->hIns;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(0, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = childClassName.c_str();
	wcex.hIconSm = NULL;

	if(!RegisterClassEx(&wcex))
	{
		MessageBox(0, L"", 0, 0);
	}

	if(!desc.style)
		desc.style = WS_EX_CLIENTEDGE;

	win.hWnd =	CreateWindowEx
				(
					desc.style,
					childClassName.c_str(), 
					desc.name.c_str(), 
					WS_CAPTION | WS_SYSMENU ,
					desc.topLeftPos.x,  desc.topLeftPos.y, 
					desc.windowSize.x, desc.windowSize.y, 
					pData->hWnd,
					NULL,
					pData->hIns, 
					NULL
				);


	if (win.hWnd)
	{
		pData->childWindows.push_back(win);
		ShowWindow(win.hWnd, 5);
		UpdateWindow(win.hWnd);
	}
	else
	{
		DWORD err = GetLastError();
		MessageBox(0, L"Failed to create child window", L"Error!", MB_OK);
		return false;
	}

	return win.id;
}
bool WindowShell::removeChild(int id)
{
	for (int i = 0; i < (int)pData->childWindows.size(); i++)
	{
		if(id == pData->childWindows[i].id)
		{
			pData->childWindows.erase(pData->childWindows.begin() + i);
			return true;
		}
	}

	return false;
}
bool WindowShell::removeChild(HWND hwnd)
{
	for (int i = 0; i < (int)pData->childWindows.size(); i++)
	{
		if(hwnd == pData->childWindows[i].hWnd)
		{
			pData->childWindows.erase(pData->childWindows.begin() + i);
			return true;
		}
	}

	return false;
}



const HINSTANCE WindowShell::getHINSTANCE() const
{
	return pData->hIns;
}
const HWND WindowShell::getHWND() const
{
	return pData->hWnd;
}
const HWND WindowShell::getChildHWND(int id) const
{
	for(int i = 0; i<(int)pData->childWindows.size(); i++)
	{
		if(id == pData->childWindows[i].id)
			return pData->childWindows[i].hWnd;
	}

	return NULL;
}
const int WindowShell::getChildID(HWND hwnd) const
{
	for(int i = 0; i<(int)pData->childWindows.size(); i++)
	{
		if(hwnd == pData->childWindows[i].hWnd)
			return pData->childWindows[i].id;
	}

	return -1;
}



WindowShell* WindowShell::self()
{
	if(!instance)
		instance = new WindowShell();

	return instance;
}
void WindowShell::destroy()
{
	delete instance;
	instance = NULL;
}