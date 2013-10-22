#pragma once
#ifndef TIME_H
#define TIME_H

class Time
{
public:
	enum Result { Failure, Success };
	static Result init( );

	static void update( );

	static float getDeltaTime_Seconds( );

private:
	static float secsPerCnt;
	static __int64 prevTimeStamp;
	static float deltaTime;
};

#endif