#include "AudioAPI.h"
#include "AudioManager.h"

using namespace Sound;
void AudioAPI::Init(void)
{
	AudioManager::self()->intitializeSoundManager();
}
void AudioAPI::Shutdown(void)
{
	AudioManager::self()->shutdownSoundManager();
}
SoundData* AudioAPI::Audio_CreateSound(std::string soundName, bool stream)
{
	return AudioManager::self()->CreateSound(soundName, stream);
}

void AudioAPI::Audio_PlaySound(SoundData* sound)
{
	AudioManager::self()->Play_Sound(sound);
}
void AudioAPI::Audio_DeleteSound(SoundData* sound)
{
	AudioManager::self()->DeleteSound(sound);
}
void AudioAPI::Audio_Update()
{
	AudioManager::self()->updateSoundManager(); 
}