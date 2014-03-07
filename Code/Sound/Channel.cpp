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
	channel->setVolume(volym);
}
void ChannelData::setMinMaxDistance( float min, float max)
{
	channel->set3DMinMaxDistance(min, max);
}
void ChannelData::setChannel3DAttributes( float* pos, float* vel)
{
	//FMOD_VECTOR posF = { 1.0f, 2.0f, 3.0f };
	//FMOD_VECTOR velF = { 1.0f, 0.0f, 0.0f };
	//channel->set3DAttributes( &posF, &velF);
	channel->set3DAttributes( &floatArrToFmodVECTOR(pos), &floatArrToFmodVECTOR(vel));
}
void ChannelData::SetPauseChannel(bool pause)
{
	channel->setPaused(pause);
}
bool ChannelData::GetPauseChannel()
{
	bool pause;
	channel->getPaused(&pause);
	return pause;
}
void ChannelData::toggleChannelPaused()
{
	bool paused;
	channel->getPaused(&paused);
	channel->setPaused(!paused);
}
void ChannelData::setSoundVolume()
{
	FMOD_VECTOR direction = { 0.0f, 1.0f, 0.0f };
	channel->set3DConeOrientation(&direction);
	// 360 grader, 360 grader, 0- 1 volym
	channel->set3DConeSettings(30.0f, 360.0f, 0.2f);
}
bool ChannelData::getChannelPlaying()
{
	FMOD_RESULT result;
	bool playing;
	result = channel->isPlaying(&playing);
	FmodErrorCheck(result);
	
	return playing; 
}
void ChannelData::stop()
{
	channel->stop();
}