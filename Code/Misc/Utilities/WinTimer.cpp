#include "WinTimer.h"

using namespace ::Utility;

namespace PrivateStatic
{
	LARGE_INTEGER ticksPerSecond = { 0 };
	double secondsPerTick = 0.0f;

	class WatchDog
	{
	public:
		WatchDog( )
		{
			if( QueryPerformanceFrequency( &ticksPerSecond ) > 0 )
				secondsPerTick = ((double)1.0f) / (double) ticksPerSecond.QuadPart;
		}
	} watchDog;
}

WinTimer::WinTimer( ) : isPaused(false)
{ this->reset(); }

void WinTimer::reset( )
{
	this->isPaused = false;
	this->pauseCount.QuadPart = 0;
	QueryPerformanceCounter( &this->startTick );
}

void WinTimer::pause( )
{
	if( !this->isPaused )
	{
		this->isPaused = true;
		QueryPerformanceCounter( &this->pauseStart );
	}
}

void WinTimer::resume( )
{
	if( this->isPaused )
	{
		this->isPaused = false;
		LARGE_INTEGER currentTick;
		QueryPerformanceCounter( &currentTick );
		this->pauseCount.QuadPart += currentTick.QuadPart - this->pauseStart.QuadPart;
	}
}

double WinTimer::getElapsedSeconds( ) const
{
	if( this->isPaused )
	{
		LARGE_INTEGER totalTick = this->pauseStart;
		totalTick.QuadPart -= this->startTick.QuadPart;
		totalTick.QuadPart -= this->pauseCount.QuadPart;
		return PrivateStatic::secondsPerTick * (double)totalTick.QuadPart;
	}
	else
	{
		LARGE_INTEGER currentTick;
		QueryPerformanceCounter( &currentTick );
		currentTick.QuadPart -= this->startTick.QuadPart;
		currentTick.QuadPart -= this->pauseCount.QuadPart;
		return PrivateStatic::secondsPerTick * (double)currentTick.QuadPart;
	}
}

LARGE_INTEGER WinTimer::getCurrentTick( ) const
{
	LARGE_INTEGER currentTick;
	QueryPerformanceCounter( &currentTick );
	return currentTick;
}