#include "AudioManager.h"
using namespace Sound;
using namespace FmodUtil;

ChannelData::ChannelData()
{
	channel = nullptr;
}
ChannelData::~ChannelData()
{
	if (channel)
	{
		delete channel;
		channel = nullptr;
	}	
}
void ChannelData::setVolym(float volym)
{
	result = channel->setVolume(volym);
	FmodErrorCheck(result);
}
void ChannelData::setMinMaxDistance( float min, float max)
{
	result = channel->set3DMinMaxDistance(min, max);
	FmodErrorCheck(result);
}
void ChannelData::setChannel3DAttributes( float* pos, float* vel)
{
	//FMOD_VECTOR posF = { 1.0f, 2.0f, 3.0f };
	//FMOD_VECTOR velF = { 1.0f, 0.0f, 0.0f };
	//channel->set3DAttributes( &posF, &velF);
	result = channel->set3DAttributes( &floatArrToFmodVECTOR(pos), &floatArrToFmodVECTOR(vel));
	//FmodErrorCheck(result);
}
void ChannelData::SetPauseChannel(bool pause)
{
	result = channel->setPaused(pause);
	FmodErrorCheck(result);
}
bool ChannelData::GetPauseChannel()
{
	bool pause;
	result = channel->getPaused(&pause);
	FmodErrorCheck(result);
	return pause;
}
void ChannelData::toggleChannelPaused()
{
	bool paused;
	result = channel->getPaused(&paused);
	FmodErrorCheck(result);
	result = channel->setPaused(!paused);
	FmodErrorCheck(result);
}
void ChannelData::setSoundVolume()
{
	FMOD_VECTOR direction = { 0.0f, 1.0f, 0.0f };
	result = channel->set3DConeOrientation(&direction);
	FmodErrorCheck(result);
	// 360 grader, 360 grader, 0- 1 volym
	result = channel->set3DConeSettings(30.0f, 360.0f, 0.2f);
	FmodErrorCheck(result);
}
bool ChannelData::getChannelPlaying()
{
	bool playing;
	result = channel->isPlaying(&playing);
	//FmodErrorCheck(result);
	return playing; 
}
void ChannelData::stop()
{
	result = channel->stop();
	FmodErrorCheck(result);
}
void ChannelData::restartChannel()
{
	result = channel->setPosition(0, FMOD_TIMEUNIT_MS);
}