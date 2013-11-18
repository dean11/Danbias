#include "ServerInclude.h"
#ifndef _GAME_TIMER_H
#define _GAME_TIMER_H
class ServerTimer
{
private:
	double	c_SecondsPerCount;
	double	c_DeltaTime;
	__int64	c_BaseTime;
	__int64	c_PausedTime;
	__int64	c_StopTime;
	__int64	c_PrevTime;
	__int64	c_CurrTime;
	bool	c_Stopped;
public:
	ServerTimer();
	__int64 getTime();
	void start();
	void stop();
	void reset();
	void tick();
	float getGameTime() const;
	float getDeltaTime() const;
};
#endif