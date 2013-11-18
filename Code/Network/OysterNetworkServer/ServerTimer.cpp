#include "ServerTimer.h"
ServerTimer::ServerTimer()
	: 
	c_SecondsPerCount(0.0), 
	c_DeltaTime(-1.0), 
	c_BaseTime(0), 
	c_PausedTime(0), 
	c_PrevTime(0), 
	c_CurrTime(0), 
	c_Stopped(false)
{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	c_SecondsPerCount =1.0 / (double)countsPerSec;

	QueryPerformanceCounter((LARGE_INTEGER*)&c_PrevTime);
}

void ServerTimer::start()
{
	__int64 p_StartTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&p_StartTime);
	if(c_Stopped)
	{
		c_PausedTime += (p_StartTime-c_StopTime);
		c_PrevTime = p_StartTime;
		c_StopTime = 0;
		c_Stopped = false;
	}
}
__int64 ServerTimer::getTime()
{
	__int64 testInt;
	return QueryPerformanceCounter((LARGE_INTEGER*)&testInt);
	return testInt;
}

void ServerTimer::stop()
{
	if(!c_Stopped)
	{
		__int64 p_CurrTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&p_CurrTime);
		c_StopTime = p_CurrTime;
		c_Stopped = true;
	}
}
void ServerTimer::reset()
{
	__int64 p_CurrTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&p_CurrTime);
	c_BaseTime	= p_CurrTime;
	c_PrevTime	= p_CurrTime;
	c_StopTime	= 0;
	c_Stopped	= false;
}
void ServerTimer::tick()
{
	if (c_Stopped)
	{
		c_DeltaTime= 0.0;
		return;
	}
	__int64 p_CurrTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&p_CurrTime);
	c_CurrTime=p_CurrTime;

	c_DeltaTime=(c_CurrTime-c_PrevTime)*c_SecondsPerCount;
	c_PrevTime=c_CurrTime;
	if(c_DeltaTime<0.0) c_DeltaTime=0.0;
}
float ServerTimer::getGameTime() const
{
	if(c_Stopped)
	{
		return (float)((c_StopTime-c_BaseTime)*c_SecondsPerCount);
	} else
	{
		return (float)(((c_CurrTime-c_PausedTime)-c_BaseTime)*c_SecondsPerCount);
	}
}
float ServerTimer::getDeltaTime() const
{
	return (float)c_DeltaTime;
}