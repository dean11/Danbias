#include "C_AudioHandler.h"
using namespace DanBias::Client;
using namespace Sound;

C_AudioHandler::C_AudioHandler(){}

C_AudioHandler::~C_AudioHandler()
{

}
bool C_AudioHandler::Init(){return true;}
void C_AudioHandler::addSound(SoundDesc soundDesc)
{
	if(!effects[soundDesc.ID])
		effects[soundDesc.ID] = AudioAPI::Audio_CreateSound( soundDesc.name );
}
Sound::ISound* C_AudioHandler::getSound( SoundID id )
{
	return effects[id];
}
void C_AudioHandler::SetSoundVolume(float volume)
{
	auto effect = this->effects.begin();
	for( ; effect != this->effects.end(); ++effect )
	{
		if(effect->second->getType() == Sound::SoundType_Music)
			effect->second->setVolym(volume);
	}
}
void C_AudioHandler::SetEffectVolume(float volume)
{
	auto effect = this->effects.begin();
	for( ; effect != this->effects.end(); ++effect )
	{
		if(effect->second->getType() == Sound::SoundType_Effect)
			effect->second->setVolym(volume);
	}
}
void C_AudioHandler::Release()
{
	
	auto effect = this->effects.begin();
	for( ; effect != this->effects.end(); ++effect )
	{
		effect->second->ReleaseSound();
		delete effect->second;
	}
	effects.clear();
	songs.clear();

}
