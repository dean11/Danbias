#include "Time.h"
#include "Engine.h"

float Time::secsPerCnt = 1.0f;
__int64 Time::prevTimeStamp = 0;
float Time::deltaTime = 0.0f;

Time::Result Time::init( )
{
	__int64 cntsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
	secsPerCnt = 1.0f / (float)cntsPerSec;

	QueryPerformanceCounter((LARGE_INTEGER*)&prevTimeStamp);

	return Result::Success;
}

void Time::update( )
{
	__int64 currTimeStamp = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);

	deltaTime = (currTimeStamp - prevTimeStamp) * secsPerCnt;

	prevTimeStamp = currTimeStamp;
}

float Time::getDeltaTime_Seconds( )
{
	return deltaTime;
}