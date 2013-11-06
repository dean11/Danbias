#ifndef GLARE_WINDOW_H
#define GLARE_WINDOW_H

#include <Windows.h>
#include <string>


struct Point2D
{
	int x;
	int y;
	Point2D()
	{
		x = 0;
		y = 0;
	}
	Point2D(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
	Point2D(int _p)
	{
		x = _p;
		y = _p;
	}
	operator POINT() const
	{
		return Point2D(x, y);
	}
	bool operator<(int i)
	{
		bool a = x<i;
		bool b = y<i;
		return (a || b);
	}
};

class WindowShell
{
	public:
		struct INIT_DESC_WINDOW
		{
			HINSTANCE			hInstance;
			std::wstring		windowName;
			Point2D				windowSize;
			Point2D				windowPosition;
			WNDPROC				windowProcCallback;
							
			INIT_DESC_WINDOW()
			{
				hInstance			= NULL;
				windowName			= L"Child window";
				windowSize.x		= 800;
				windowSize.y		= 600;
				windowPosition.x	= 0;
				windowPosition.y	= 0;
				windowProcCallback	= NULL;
			}
		};
		struct INIT_DESC_CHILD_WINDOW
		{
			std::wstring	name;
			DWORD			style;
			Point2D			topLeftPos;
			Point2D			windowSize;
			WNDPROC			windowProcCallback;

			INIT_DESC_CHILD_WINDOW()
			{
				name				= L"Child Window";
				style				= WS_CHILD;
				topLeftPos			= Point2D(0,0);
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
		bool			createWin			(INIT_DESC_WINDOW&);
		/*Creates a child window and returns the id of child window or -1 if failed*/
		int				createChildWin		(INIT_DESC_CHILD_WINDOW&);
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