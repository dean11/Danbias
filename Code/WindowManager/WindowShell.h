#ifndef GLARE_WINDOW_H
#define GLARE_WINDOW_H

#include <Windows.h>
#include <string>


class WindowShell
{
	public:
		struct WINDOW_INIT_DESC
		{
			HINSTANCE			hInstance;
			std::wstring		windowName;
			POINT				windowSize;
			POINT				windowPosition;
			WNDPROC				windowProcCallback;
			UINT				windowClassStyle;
			UINT				windowStyle;
							
			WINDOW_INIT_DESC()
			{
				hInstance			= NULL;
				windowName			= L"MADAFACKA";
				windowSize.x		= 800;
				windowSize.y		= 600;
				windowPosition.x	= 0;
				windowPosition.y	= 0;
				windowProcCallback	= NULL;
				windowClassStyle	= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
				windowStyle			= WS_OVERLAPPEDWINDOW;
			}
		};
		struct CHILD_WINDOW_INIT_DESC
		{
			std::wstring	name;
			DWORD			style;
			POINT			topLeftPos;
			POINT			windowSize;
			WNDPROC			windowProcCallback;

			CHILD_WINDOW_INIT_DESC()
			{
				name				= L"Child Window";
				style				= WS_CHILD;
				memset(&topLeftPos, 0, sizeof(POINT));
				windowSize.x		= 300;
				windowSize.y		= 200;
				windowProcCallback	= NULL;
			}
		};


	private:
		WindowShell							();
		WindowShell							(const WindowShell&);
		void operator=						(const WindowShell&);
		virtual~WindowShell					();

	public:
		const HINSTANCE	getHINSTANCE		() const;
		/* Returns NULL if no hwnd exists */
		const HWND		getHWND				() const;
		/* Returns NULL if not found */
		const HWND		getChildHWND		(int id) const;
		/* Returns -1 if not found */
		const int		getChildID			(HWND hwnd) const;

		/* Creates an empty window */
		bool			createWin			(WINDOW_INIT_DESC&);
		/*Creates a child window and returns the id of child window or -1 if failed*/
		int				createChildWin		(CHILD_WINDOW_INIT_DESC&);
		/* Removes a child window */
		bool			removeChild			(int id);
		/* Removes a child window */
		bool			removeChild			(HWND hwnd);


		/* Returns a pointer to this class, dont forget to destroy on exit */
		static WindowShell*	self();
		/* Deletes the instance */
		static void destroy();
};

#endif