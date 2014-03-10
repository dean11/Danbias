#include "AudioManager.h"

using namespace Sound;
using namespace FmodUtil;
bool AudioManager::instanceFlag = false;
AudioManager* AudioManager::single = NULL;

void FmodUtil::floatArrToFmodVECTOR(FMOD_VECTOR& F_Vec, float* floatArr)
{
	F_Vec.x = floatArr[0];
	F_Vec.y = floatArr[1];
	F_Vec.z = floatArr[2];
}
FMOD_VECTOR FmodUtil::floatArrToFmodVECTOR( float* floatArr)
{
	FMOD_VECTOR F_Vec;
	F_Vec.x = floatArr[0];
	F_Vec.y = floatArr[1];
	F_Vec.z = floatArr[2];
	return F_Vec;
}
bool FmodUtil::FmodErrorCheck(FMOD_RESULT result)	// Error handling function for FMOD errors
{		
	if (result != FMOD_OK)
	{
		// output error message
		//printf( "FMOD error! ( %i ) %s", result, FMOD_ErrorString(result));
		return false;
	}
	return true;
}

AudioManager* AudioManager::self()
{
	if(!instanceFlag)
	{
		single = new AudioManager();
		instanceFlag = true;
		return single;
	}
	else
	{
		return single;
	}
}
AudioManager::AudioManager(void)
{
	this->basePath = "..\\Content\\Sound\\";

	FMOD_VECTOR pos = { 2, 3, 4};
	FMOD_VECTOR vel = { 1, 0, 0};
	FMOD_VECTOR fow = { 1, 0, 0};
	FMOD_VECTOR up = { 0, 1, 0};
	listenerData.pos = pos;
	listenerData.vel = vel;
	listenerData.forward = fow;
	listenerData.up = up;

}
AudioManager::~AudioManager(void)
{
}

bool AudioManager::intitializeSoundManager()
{
	FMOD_SPEAKERMODE speakerMode;
	int numDrivers;
	FMOD_CAPS caps;
	char name[256];

	// create FMOD system
	result = System_Create(&fmodSystem);
	if(!FmodErrorCheck(result))
		return false;

	// Get number of sound cards 
	result = fmodSystem->getNumDrivers(&numDrivers);
	if(!FmodErrorCheck(result))
		return false;

	// No sound cards (disable sound)
	if (numDrivers == 0)
	{
		result = fmodSystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		if(!FmodErrorCheck(result))
			return false;
	}
	// At least one sound card
	else
	{
		// Get the capabilities of the default (0) sound card
		result = fmodSystem->getDriverCaps(0, &caps, 0, &speakerMode);
		if(!FmodErrorCheck(result))
			return false;

		// Set the speaker mode to match that in Control Panel
		result = fmodSystem->setSpeakerMode(speakerMode);
		if(!FmodErrorCheck(result))
			return false;

		// Increase buffer size if user has Acceleration slider set to off
		if (caps & FMOD_CAPS_HARDWARE_EMULATED)
		{
			result = fmodSystem->setDSPBufferSize(1024, 10);
			if(!FmodErrorCheck(result))
				return false;
		}

		// Get name of driver
		result = fmodSystem->getDriverInfo(0, name, 256, 0);
		if(!FmodErrorCheck(result))
			return false;

		// SigmaTel sound devices crackle for some reason if the format is PCM 16-bit.
		// PCM floating point output seems to solve it.
		if (strstr(name, "SigmaTel"))
		{
			result = fmodSystem->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0, 0, FMOD_DSP_RESAMPLER_LINEAR);
			if(!FmodErrorCheck(result))
				return false;
		}
	}
	
	// Initiate 
	result = fmodSystem->init(maxNrOfChannels, FMOD_INIT_3D_RIGHTHANDED,0);
	
	// If the selected speaker mode isn't supported by this sound card, switch it back to stereo
	if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)
	{
		result = fmodSystem->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		if(!FmodErrorCheck(result))
			return false;

		result = fmodSystem->init(maxNrOfChannels, FMOD_INIT_3D_RIGHTHANDED, 0);
	}
	// If FMOD_INIT_3D_RIGHTHANDED should be the problem, init with Normal
	if(!FmodErrorCheck(result))
	{
		result = fmodSystem->init(maxNrOfChannels, FMOD_INIT_NORMAL, 0);
		if(!FmodErrorCheck(result))
			return false;
	}

	// use default distance
	/*result = fmodSystem->set3DSettings(1.0f, 1.0f, 1.0f);
	if(!FmodErrorCheck(result))
	return false;*/

	return true;
}
void AudioManager::shutdownSoundManager()
{
	result = fmodSystem->close(); 
	FmodErrorCheck(result);
	result = fmodSystem->release();
	FmodErrorCheck(result);
	delete single;
}
bool AudioManager::updateSoundManager(float deltaTime)
{
	result = fmodSystem->update();
	if(!FmodErrorCheck(result))
		return false;
	return true;
}

ChannelGroupData* AudioManager::CreateChannelGroup(const char* groupName)
{
	ChannelGroupData* channelGroup = new ChannelGroupData();
	result = this->fmodSystem->createChannelGroup( groupName, &channelGroup->channelgroup);
	return channelGroup;
}
ChannelData* AudioManager::CreateChannel()
{
	ChannelData* channel = new ChannelData();
	return channel;
}
SoundData* AudioManager::CreateSound(std::string soundName, SoundType soundType)
{
	SoundData* sound = new SoundData();
	std::string  soundPath = basePath + soundName;
	
	if(soundType == SoundType_Music )
	{
		if ( !FmodErrorCheck( this->fmodSystem->createStream(soundPath.c_str(), FMOD_DEFAULT, NULL, &sound->sound) ) )
		{
			// error
			delete sound;
			return nullptr;
		}	
	}
	else if(soundType == SoundType_Effect)
	{
		if ( !FmodErrorCheck( this->fmodSystem->createSound(soundPath.c_str(), FMOD_DEFAULT, NULL, &sound->sound) ) )
		{
			// error
			delete sound;
			return nullptr;
		}	
	}
	else if(soundType == SoundType_Effect3D)
	{
		if ( !FmodErrorCheck( this->fmodSystem->createSound(soundPath.c_str(), FMOD_3D, NULL, &sound->sound) ) )
		{
			// error
			delete sound;
			return nullptr;
		}	
	}
	else
	{
		delete sound;
		return nullptr;
	}
	return sound;
}
void AudioManager::Play_Sound(SoundData* soundData, ChannelData* channelData, bool paused)
{	
	result = this->fmodSystem->playSound(FMOD_CHANNEL_FREE, soundData->sound, paused, &channelData->channel);
	if (result != FMOD_OK && result != FMOD_ERR_INVALID_HANDLE )
	{
		FmodErrorCheck(result);
	}
}
void AudioManager::setListenerPos( float* pos, float* vel, float* forward, float* up )
{
	
	floatArrToFmodVECTOR( listenerData.pos, pos);
	floatArrToFmodVECTOR( listenerData.vel, vel);
	floatArrToFmodVECTOR( listenerData.forward, forward);
	floatArrToFmodVECTOR( listenerData.up, up);

	result = fmodSystem->set3DListenerAttributes(0, &listenerData.pos, &listenerData.vel, &listenerData.forward, &listenerData.up );
	FmodErrorCheck(result);
}
void AudioManager::setBasePath(std::string basePath)
{
	this->basePath = basePath;
}
