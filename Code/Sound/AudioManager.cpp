#include "AudioManager.h"

using namespace Sound;
const int   INTERFACE_UPDATETIME = 50;      // 50ms update for interface
const float DISTANCEFACTOR = 1.0f;          // Units per meter.  I.e feet would = 3.28.  centimeters would = 100.
bool AudioManager::instanceFlag = false;
AudioManager* AudioManager::single = NULL;

SoundData::SoundData()
{
	sound = nullptr;
	channel = nullptr;
}
SoundData::~SoundData()
{
	if(sound) 
	{
		delete sound;
		sound = nullptr;
	}
	if (channel)
	{
		delete channel;
		channel = nullptr;
	}	
}
 void SoundData::ReleaseSound()
 {
	 if(sound) 
	 {
		 sound->release();
	 }
	 if (channel)
	 {
		 channel->stop();
	 }
 }

void SoundData::Play_Sound( bool paused )
{
	AudioManager::self()->Play_Sound(this, paused);
}
void SoundData::setVolym(float volym)
{
	channel->setVolume(volym);
}
void SoundData::setMinMaxDistance( float min, float max)
{
	channel->set3DMinMaxDistance(min, max);
}
void SoundData::setChannel3DAttributes( float* pos, float* vel)
{
	//FMOD_VECTOR posF = { 1.0f, 2.0f, 3.0f };
	//FMOD_VECTOR velF = { 1.0f, 0.0f, 0.0f };
	//channel->set3DAttributes( &posF, &velF);
	channel->set3DAttributes( &floatArrToFmodVECTOR(pos), &floatArrToFmodVECTOR(vel));
}
void SoundData::setMode(SoundMode soundMode)
{
	if(soundMode == Loop_normal)
		sound->setMode(FMOD_LOOP_NORMAL);
	else if(soundMode == Loop_off)
		sound->setMode(FMOD_LOOP_OFF);
}
void SoundData::SetPauseChannel(bool pause)
{
	channel->setPaused(pause);
}
bool SoundData::GetPauseChannel()
{
	bool pause;
	channel->getPaused(&pause);
	return pause;
}
void SoundData::toggleChannelPaused()
{
	bool paused;
	channel->getPaused(&paused);
	channel->setPaused(!paused);
}
SoundType SoundData::getType()
{
	return this->soundType;
}
void SoundData::setSoundVolume()
{
	FMOD_VECTOR direction = { 0.0f, 1.0f, 0.0f };
	channel->set3DConeOrientation(&direction);
	// 360 grader, 360 grader, 0- 1 volym
	channel->set3DConeSettings(30.0f, 360.0f, 0.2f);
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
	this->music_volume = STANDARD_MUSIC_VOLYM;
	this->effects_volume = STANDARD_EFFECTS_VOLYM;
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
	fmodSystem->close(); 
	fmodSystem->release();
	delete single;
}
bool AudioManager::updateSoundManager()
{
	result = fmodSystem->update();
	if(!FmodErrorCheck(result))
		return false;
	return true;
}


SoundData* AudioManager::CreateSound(std::string soundName, bool stream)
{
	SoundData* sound = new SoundData();
	std::string  soundPath = basePath + soundName;
	
	if(stream)
	{
		if ( !FmodErrorCheck( this->fmodSystem->createStream(soundPath.c_str(), FMOD_DEFAULT, NULL, &sound->sound) ) )
		{
			// error
			delete sound;
			return nullptr;
		}	
	}
	else
	{
		if ( !FmodErrorCheck( this->fmodSystem->createSound(soundPath.c_str(), FMOD_3D, NULL, &sound->sound) ) )
		{
			// error
			delete sound;
			return nullptr;
		}	
	}

	return sound;
}


void AudioManager::Play_Sound(SoundData* sound, bool paused)
{	
	bool playing; 

	// don't start sound if it is already playing
	sound->channel->isPlaying(&playing);
	if(!playing)
	{
		result = this->fmodSystem->playSound(FMOD_CHANNEL_FREE, sound->sound, paused,&sound->channel);
	}
}
void AudioManager::DeleteSound(SoundData* sound)
{
	if(sound)
	{
		delete sound;
		sound = nullptr;
	}
}

void AudioManager::setListener( float* pos, float* vel, float* forward, float* up )
{
	
	Sound::floatArrToFmodVECTOR( listenerData.pos, pos);
	Sound::floatArrToFmodVECTOR( listenerData.vel, vel);
	Sound::floatArrToFmodVECTOR( listenerData.forward, forward);
	Sound::floatArrToFmodVECTOR( listenerData.up, up);

	result = fmodSystem->set3DListenerAttributes(0, &listenerData.pos, &listenerData.vel, &listenerData.forward, &listenerData.up );
	if(!FmodErrorCheck(result))
	{
		int i = 0;
	}
		//return false;
}
void Sound::floatArrToFmodVECTOR(FMOD_VECTOR& F_Vec, float* floatArr)
{
	F_Vec.x = floatArr[0];
	F_Vec.y = floatArr[1];
	F_Vec.z = floatArr[2];
}
FMOD_VECTOR Sound::floatArrToFmodVECTOR( float* floatArr)
{
	FMOD_VECTOR F_Vec;
	F_Vec.x = floatArr[0];
	F_Vec.y = floatArr[1];
	F_Vec.z = floatArr[2];
	return F_Vec;
}

void AudioManager::setBasePath(std::string basePath)
{
	this->basePath = basePath;
}
bool AudioManager::FmodErrorCheck(FMOD_RESULT result)	// Error handling function for FMOD errors
{		
	if (result != FMOD_OK)
	{
		// output error message
		printf( "FMOD error! ( %i ) %s", result, FMOD_ErrorString(result));
		return false;
	}
	return true;
}


void AudioManager::setMusicVolym(float volym)
{
	this->music_volume = volym;
}
void AudioManager::setEffectsVolym(float volym)
{
	this->effects_volume = volym;
}