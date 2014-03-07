#include "C_AudioHandler.h"
using namespace DanBias::Client;
using namespace Sound;

C_AudioHandler::C_AudioHandler()
{
	maxCollisionSounds = 50;
	currCollisionSound = 0; 
	collisionChannels = new Sound::IChannel*[maxCollisionSounds];
	for (int i = 0; i < maxCollisionSounds; i ++)
	{
		collisionChannels[i] = AudioAPI::Audio_CreateChannel();	
	}
}

C_AudioHandler::~C_AudioHandler()
{

}
bool C_AudioHandler::Init(){return true;}

void C_AudioHandler::addSFX(SoundDesc soundDesc)
{
	if(!effects[soundDesc.ID])
	{
		effects[soundDesc.ID] = AudioAPI::Audio_CreateSound( soundDesc.name, SoundType_Effect );
		addChannel(soundDesc.ID);
	}
}
void C_AudioHandler::addMusic(SoundDesc soundDesc)
{
	if(!songs[soundDesc.ID])
	{
		songs[soundDesc.ID] = AudioAPI::Audio_CreateSound( soundDesc.name, SoundType_Music );
		addChannel(soundDesc.ID);
	}
}
void C_AudioHandler::addSFX_3D(SoundDesc soundDesc)
{
	if(!effects[soundDesc.ID])
	{
		effects[soundDesc.ID] = AudioAPI::Audio_CreateSound( soundDesc.name, SoundType_Effect3D );
		addChannel(soundDesc.ID);
	}
}
void C_AudioHandler::addPlayerSound(int playerId)
{
	if(!playerChannels[playerId])
	{
		playerChannels[playerId] = new PlayerSounds();
		for (int i = 0; i < PlayerSoundID_Count; i ++)
		{
			playerChannels[playerId]->channels[i] = AudioAPI::Audio_CreateChannel();
		}
		AudioAPI::Audio_PlaySound(getSound(walk), getPlayerChannel(playerId, PlayerSoundID_walk), true);
		getPlayerChannel(playerId, PlayerSoundID_walk)->setVolym(0.2);
	}
}
void C_AudioHandler::setPlayerChannelPos( int playerId, float* pos, float* vel)
{
	if(playerChannels[playerId])
	{
		for (int i = 0; i < PlayerSoundID_Count; i ++)
		{
			playerChannels[playerId]->channels[i]->setChannel3DAttributes(pos, vel);
		}
	}
}
void C_AudioHandler::playCollisionSound()
{
	//getSound(mouse_click)->setMode(Loop_normal);
	//for (int i = 0; i < maxCollisionSounds; i ++)
	//{
	//	collisionChannels[i] = AudioAPI::Audio_CreateChannel();	
	//	AudioAPI::Audio_PlaySound(getSound(mouse_click), collisionChannels[i]);
	//}
}
void C_AudioHandler::addChannel(SoundID id )
{
	if (!channels[id])
	{
		channels[id] = AudioAPI::Audio_CreateChannel();
	}
}
void C_AudioHandler::addChannel(ChannelID id )
{
	if (!channels[id])
	{
		channels[id] = AudioAPI::Audio_CreateChannel();
	}
}
Sound::ISound* C_AudioHandler::getSound( SoundID id )
{
	ISound* sound = nullptr;
	sound = effects[id]; 
	if(!sound)
		sound = songs[id];
	return sound;
}
Sound::ISound* C_AudioHandler::getSFX( SoundID id )
{
	return effects[id];
}
Sound::IChannel* C_AudioHandler::getChannel( SoundID id )
{
	return channels[id];
}
Sound::IChannel* C_AudioHandler::getChannel( ChannelID id )
{
	return channels[id];
}
Sound::IChannel* C_AudioHandler::getPlayerChannel( int  id, PlayerSoundID soundId )
{
	return playerChannels[id]->channels[soundId];
}
Sound::IChannel* C_AudioHandler::getCollisionChannel( )
{
	if (currCollisionSound >= maxCollisionSounds)
	{
		currCollisionSound = 0;
	}
	return collisionChannels[currCollisionSound++];
}
void C_AudioHandler::pauseAllSounds()
{
	for (int i = 0; i < maxCollisionSounds; i ++)
	{
		collisionChannels[i]->SetPauseChannel(true);
	}
	auto channel = this->channels.begin();
	for( ; channel != this->channels.end(); ++channel )
	{
		channel->second->SetPauseChannel(true);
	}
	auto playerChannel = this->playerChannels.begin();
	for( ; playerChannel != this->playerChannels.end(); ++playerChannel )
	{

		for (int i = 0; i < PlayerSoundID_Count; i ++)
		{
			if(playerChannel->second)
			{
				playerChannel->second->channels[i]->SetPauseChannel(true);
			}
		}
	}
}
void C_AudioHandler::SetSoundVolume(float volume)
{
	auto effect = this->channels.begin();
	//for( ; effect != this->channels.end(); ++effect )
	//{
	//	//if(effect->second->getType() == Sound::SoundType_Music)
	//		//effect->second->setVolym(volume);
	//}
}
void C_AudioHandler::SetEffectVolume(float volume)
{
	auto effect = this->effects.begin();
	//for( ; effect != this->effects.end(); ++effect )
	//{
	//	if(effect->second->getType() == Sound::SoundType_Effect)
	//		effect->second->setVolym(volume);
	//}
}
void C_AudioHandler::Release()
{
	for (int i = 0; i < maxCollisionSounds; i ++)
	{
		delete collisionChannels[i];
	}
	delete [] collisionChannels;

	auto effect = this->effects.begin();
	for( ; effect != this->effects.end(); ++effect )
	{
		if(effect->second )
		{
			effect->second->ReleaseSound();
			delete effect->second;
		}

	}
	auto song = this->songs.begin();
	for( ; song != this->songs.end(); ++song )
	{
		song->second->ReleaseSound();
		delete song->second;
	}
	auto channel = this->channels.begin();
	for( ; channel != this->channels.end(); ++channel )
	{
		delete channel->second;
	}
	auto playerChannel = this->playerChannels.begin();
	for( ; playerChannel != this->playerChannels.end(); ++playerChannel )
	{
		
		for (int i = 0; i < PlayerSoundID_Count; i ++)
		{
			if(playerChannel->second)
			{
				playerChannel->second->channels[i]->stop();
				delete playerChannel->second->channels[i];
			}
		}
		delete [] playerChannel->second->channels;
		delete playerChannel->second;
	}
	effects.clear();
	songs.clear();
	channels.clear();
	playerChannels.clear();
}
