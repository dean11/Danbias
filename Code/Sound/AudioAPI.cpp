#include "AudioAPI.h"
#include "AudioManager.h"

using namespace Sound;

bool AudioAPI::Audio_Init(void)
{
	return AudioManager::self()->intitializeSoundManager();
}
void AudioAPI::Audio_Shutdown(void)
{
	AudioManager::self()->shutdownSoundManager();
}
bool AudioAPI::Audio_Update(float deltaTime)
{
	return AudioManager::self()->updateSoundManager(deltaTime); 
}
IChannelGroup* AudioAPI::Audio_CreateChannelGroup(const char* groupName)
{
	return (IChannelGroup*)AudioManager::self()->CreateChannelGroup(groupName);
}
IChannel* AudioAPI::Audio_CreateChannel()
{
	return (IChannel*)AudioManager::self()->CreateChannel();
}
ISound* AudioAPI::Audio_CreateSound(std::string soundName, SoundType soundType)
{
	return (ISound*)AudioManager::self()->CreateSound(soundName, soundType);
}
void AudioAPI::Audio_PlaySound( ISound* sound, IChannel* channel, bool paused)
{
	AudioManager::self()->Play_Sound((SoundData*)sound, (ChannelData*)channel, paused);
}
void AudioAPI::Audio_SetListenerPos( float* pos, float* vel, float* forward, float* up )
{
	AudioManager::self()->setListenerPos(pos, vel, forward, up);
}
