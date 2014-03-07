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
	if(soundMode == Loop_normal)
		sound->setMode(FMOD_LOOP_NORMAL);
	else if(soundMode == Loop_off)
		sound->setMode(FMOD_LOOP_OFF);
}
SoundType SoundData::getType()
{
	return this->soundType;
}