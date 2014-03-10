#include "AudioManager.h"
using namespace Sound;
using namespace FmodUtil;

//if (result == FMOD_ERR_CHANNEL_STOLEN)
//{
//	int i = 0; 
//}
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
	if(channel)
	{
		result = channel->setVolume(volym);
		if (result != FMOD_OK && result != FMOD_ERR_INVALID_HANDLE )
		{
			FmodErrorCheck(result);
		}
	}
}
void ChannelData::setMinMaxDistance( float min, float max)
{
	if(channel)
	{
		result = channel->set3DMinMaxDistance(min, max);
		if (result != FMOD_OK && result != FMOD_ERR_INVALID_HANDLE )
		{
			FmodErrorCheck(result);
		}
	}
}
void ChannelData::setChannel3DAttributes( float* pos, float* vel)
{
	if(channel)
	{
		result = channel->set3DAttributes( &floatArrToFmodVECTOR(pos), &floatArrToFmodVECTOR(vel));
		if (result != FMOD_OK && result != FMOD_ERR_INVALID_HANDLE )
		{
			FmodErrorCheck(result);
		}
	}
}
void ChannelData::SetPauseChannel(bool pause)
{
	if(channel)
	{
		result = channel->setPaused(pause);
		if (result != FMOD_OK && result != FMOD_ERR_INVALID_HANDLE )
		{
			FmodErrorCheck(result);
		}
	}
}
bool ChannelData::GetPauseChannel()
{
	bool pause = false;
	if(channel)
	{
		result = channel->getPaused(&pause);
		if (result != FMOD_OK && result != FMOD_ERR_INVALID_HANDLE )
		{
			FmodErrorCheck(result);
		}
	}
	return pause;
}
void ChannelData::toggleChannelPaused()
{
	if(channel)
	{
		bool paused;
		result = channel->getPaused(&paused);
		if (result != FMOD_OK && result != FMOD_ERR_INVALID_HANDLE )
		{
			FmodErrorCheck(result);
		}
		result = channel->setPaused(!paused);
		if (result != FMOD_OK && result != FMOD_ERR_INVALID_HANDLE )
		{
			FmodErrorCheck(result);
		}
	}
}
void ChannelData::setSoundVolume()
{
	if(channel)
	{
		FMOD_VECTOR direction = { 0.0f, 1.0f, 0.0f };
		result = channel->set3DConeOrientation(&direction);
		if (result != FMOD_OK && result != FMOD_ERR_INVALID_HANDLE )
		{
			FmodErrorCheck(result);
		}
		// 360 grader, 360 grader, 0- 1 volym
		result = channel->set3DConeSettings(30.0f, 360.0f, 0.2f);
		if (result != FMOD_OK && result != FMOD_ERR_INVALID_HANDLE )
		{
			FmodErrorCheck(result);
		}
	}
}
bool ChannelData::getChannelPlaying()
{
	bool playing = false;
	if(channel)
	{
		result = channel->isPlaying(&playing);
		if (result != FMOD_OK && result != FMOD_ERR_INVALID_HANDLE )
		{
			FmodErrorCheck(result);
		}
	}
	return playing; 
}
void ChannelData::stop()
{
	if(channel)
	{
		result = channel->stop();
		if (result != FMOD_OK && result != FMOD_ERR_INVALID_HANDLE )
		{
			FmodErrorCheck(result);
		}
	}
}
void ChannelData::restartChannel()
{
	if(channel)
	{
		result = channel->setPosition(0, FMOD_TIMEUNIT_MS);
		if (result != FMOD_OK && result != FMOD_ERR_INVALID_HANDLE )
		{
			FmodErrorCheck(result);
		}
	}
}
void ChannelData::addChannelToGroup(IChannelGroup* channelGroup)
{
	channel->setChannelGroup(((ChannelGroupData*)channelGroup)->channelgroup);
	if (result != FMOD_OK && result != FMOD_ERR_INVALID_HANDLE )
	{
		FmodErrorCheck(result);
	}
}
void ChannelData::SetMuteChannel(bool mute)
{
	if (channel)
	{
		result = channel->setMute(mute);
		FmodErrorCheck(result);
	}
}