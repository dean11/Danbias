#include "AudioManager.h"
using namespace Sound;
using namespace FmodUtil;

SoundData::SoundData()
{
	sound = nullptr;
}
SoundData::~SoundData()
{
	if(sound) 
	{
		delete sound;
		sound = nullptr;
	}
}
void SoundData::ReleaseSound()
{
	if(sound) 
	{
		sound->release();
	}
}
void SoundData::setMode(SoundMode soundMode)
{
	FMOD_RESULT result;
	if(soundMode == Loop_normal)
	{
		result = sound->setMode(FMOD_LOOP_NORMAL);
		FmodErrorCheck(result);
	}
	else if(soundMode == Loop_off)
	{
		result = sound->setMode(FMOD_LOOP_OFF);
		FmodErrorCheck(result);
	}
}
SoundType SoundData::getType()
{
	return this->soundType;
}
