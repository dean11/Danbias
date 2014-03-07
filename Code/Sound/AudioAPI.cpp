#include "AudioAPI.h"
#include "AudioManager.h"

using namespace Sound;


bool AudioAPI::Init(void)
{
	return AudioManager::self()->intitializeSoundManager();
}
void AudioAPI::Shutdown(void)
{
	AudioManager::self()->shutdownSoundManager();
}
bool AudioAPI::Audio_Update()
{
	return AudioManager::self()->updateSoundManager(); 
}
void AudioAPI::setListenerPos( float* pos, float* vel, float* forward, float* up )
{
	AudioManager::self()->setListener(pos, vel, forward, up);
}
ISound* AudioAPI::Audio_CreateSound(std::string soundName, bool stream)
{
	return (ISound*)AudioManager::self()->CreateSound(soundName, stream);
}
