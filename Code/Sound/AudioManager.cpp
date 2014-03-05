#include "AudioManager.h"

using namespace Sound;
const int   INTERFACE_UPDATETIME = 50;      // 50ms update for interface
const float DISTANCEFACTOR = 1.0f;          // Units per meter.  I.e feet would = 3.28.  centimeters would = 100.
bool AudioManager::instanceFlag = false;
AudioManager* AudioManager::single = NULL;

void SoundData::Play_Sound()
{
	AudioManager::self()->Play_Sound(this);
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
}
AudioManager::~AudioManager(void)
{
}

void AudioManager::intitializeSoundManager()
{
	FMOD_SPEAKERMODE speakerMode;
	int numDrivers;
	FMOD_CAPS caps;
	char name[256];

	// create FMOD system
	result = System_Create(&fmodSystem);
	FmodErrorCheck(result);

	// Get number of sound cards 
	result = fmodSystem->getNumDrivers(&numDrivers);
	FmodErrorCheck(result);

	// No sound cards (disable sound)
	if (numDrivers == 0)
	{
		result = fmodSystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		FmodErrorCheck(result);
	}
	// At least one sound card
	else
	{
		// Get the capabilities of the default (0) sound card
		result = fmodSystem->getDriverCaps(0, &caps, 0, &speakerMode);
		FmodErrorCheck(result);

		// Set the speaker mode to match that in Control Panel
		result = fmodSystem->setSpeakerMode(speakerMode);
		FmodErrorCheck(result);

		// Increase buffer size if user has Acceleration slider set to off
		if (caps & FMOD_CAPS_HARDWARE_EMULATED)
		{
			result = fmodSystem->setDSPBufferSize(1024, 10);
			FmodErrorCheck(result);
		}

		// Get name of driver
		result = fmodSystem->getDriverInfo(0, name, 256, 0);
		FmodErrorCheck(result);

		// SigmaTel sound devices crackle for some reason if the format is PCM 16-bit.
		// PCM floating point output seems to solve it.
		if (strstr(name, "SigmaTel"))
		{
			result = fmodSystem->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0, 0, FMOD_DSP_RESAMPLER_LINEAR);
			FmodErrorCheck(result);
		}
	}
	
	// Initiate 
	result = fmodSystem->init(maxNrOfChannels, FMOD_INIT_3D_RIGHTHANDED,0);
	
	// If the selected speaker mode isn't supported by this sound card, switch it back to stereo
	if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)
	{
		result = fmodSystem->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		FmodErrorCheck(result);

		result = fmodSystem->init(maxNrOfChannels, FMOD_INIT_3D_RIGHTHANDED, 0);
	}
	// If FMOD_INIT_3D_RIGHTHANDED should be the problem, init with Normal
	if(!FmodErrorCheck(result))
	{
		result = fmodSystem->init(maxNrOfChannels, FMOD_INIT_NORMAL, 0);
		FmodErrorCheck(result);
	}

	result = fmodSystem->set3DSettings(1.0f, 1.0f, 1.0f);
}
void AudioManager::shutdownSoundManager()
{
	fmodSystem->close(); 
	fmodSystem->release();
	delete single;
}
void AudioManager::updateSoundManager()
{
	result = fmodSystem->update();
	FmodErrorCheck(result);
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
		if ( !FmodErrorCheck( this->fmodSystem->createSound(soundPath.c_str(), FMOD_DEFAULT, NULL, &sound->sound) ) )
		{
			// error
			delete sound;
			return nullptr;
		}	
	}

	return sound;
}
void AudioManager::DeleteSound(SoundData* sound)
{
	if(sound)
	{
		delete sound;
		sound = nullptr;
	}
}
void AudioManager::Play_Sound(SoundData* sound)
{	
	sound->sound->setMode(FMOD_LOOP_NORMAL);
	bool playing; 

	// don't start sound if it is already playing
	sound->channel->isPlaying(&playing);
	if(!playing)
	{
		result = this->fmodSystem->playSound(FMOD_CHANNEL_FREE, sound->sound, false,&sound->channel);
		//sound->channel->setVolume()
	}
}


void AudioManager::pauseAllSound()
{

}
void AudioManager::toggleSound(SoundData* sound)
{
	bool paused;
	sound->channel->getPaused(&paused);
	sound->channel->setPaused(!paused);
}

void AudioManager::setMusicVolym(float volym)
{
	this->music_volume = volym;
}
void AudioManager::setEffectsVolym(float volym)
{
	this->effects_volume = volym;
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