/////////////////////////////////////////////////////////////////////
// Instanciable Timer class for windows
// © Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef WINTIMER_H
#define WINTIMER_H

#define NOMINMAX
#include <Windows.h>

namespace Utility
{
	class WinTimer
	{
	public:
		WinTimer( );

		void reset( );
		void pause( );
		void resume( );

		double getElapsedSeconds( ) const;
		LARGE_INTEGER getCurrentTick( ) const;

	private:
		LARGE_INTEGER startTick, pauseStart, pauseCount;
		bool isPaused;
	};
}

#endif