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
Isound* AudioAPI::Audio_CreateSound(std::string soundName, bool stream)
{
	return (Isound*)AudioManager::self()->CreateSound(soundName, stream);
}

//void AudioAPI::Audio_PlaySound(Isound* sound)
//{
//	AudioManager::self()->Play_Sound((SoundData*)sound);
//}
//void AudioAPI::Audio_DeleteSound(Isound* sound)
//{
//	AudioManager::self()->DeleteSound((SoundData*)sound);
//}
void AudioAPI::Audio_Update()
{
	AudioManager::self()->updateSoundManager(); 
}