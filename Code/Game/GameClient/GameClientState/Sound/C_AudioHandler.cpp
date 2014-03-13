#include "C_AudioHandler.h"
using namespace DanBias::Client;
using namespace Sound;

C_AudioHandler::C_AudioHandler()
{
	maxCollisionSounds = 50;
	currCollisionSound = 0; 
	mute = false;
	collisionChannels = new Sound::IChannel*[maxCollisionSounds];
	for (int i = 0; i < maxCollisionSounds; i ++)
	{
		collisionChannels[i] = AudioAPI::Audio_CreateChannel();	
	}
	channelGroups = new Sound::IChannelGroup*[ChannelGroup_Count];
	for (int i = 0; i < ChannelGroup_Count; i ++)
	{
		channelGroups[i] = AudioAPI::Audio_CreateChannelGroup("name");	
	}
}

C_AudioHandler::~C_AudioHandler()
{

}
bool C_AudioHandler::Init(){return true;}

void C_AudioHandler::addSFX(SoundDesc soundDesc)
{
	if(!effects[soundDesc.soundID])
	{
		effects[soundDesc.soundID] = AudioAPI::Audio_CreateSound( soundDesc.soundName, SoundType_Effect );
		
		// default sound -- sound is missing
		if (!effects[soundDesc.soundID])
		{
			effects[soundDesc.soundID] = AudioAPI::Audio_CreateSound( "jaguar.wav", SoundType_Music );
		}
		
		if (soundDesc.channelID != ChannelID_none)
		{
			addChannel(soundDesc.channelID);
		}
	}
}
void C_AudioHandler::addMusic(SoundDesc soundDesc)
{
	if(!songs[soundDesc.soundID])
	{
		songs[soundDesc.soundID] = AudioAPI::Audio_CreateSound( soundDesc.soundName, SoundType_Music );
		
		// default sound -- sound is missing
		if (!songs[soundDesc.soundID])
		{
			songs[soundDesc.soundID] = AudioAPI::Audio_CreateSound( "jaguar.wav", SoundType_Music );
		}
		
		if (soundDesc.channelID != ChannelID_none)
		{
			addChannel(soundDesc.channelID);
		}
	}
}
void C_AudioHandler::addSFX_3D(SoundDesc soundDesc)
{
	if(!effects[soundDesc.soundID])
	{
		effects[soundDesc.soundID] = AudioAPI::Audio_CreateSound( soundDesc.soundName, SoundType_Effect3D );
		
		// default sound -- sound is missing
		if (!effects[soundDesc.soundID])
		{
			effects[soundDesc.soundID] = AudioAPI::Audio_CreateSound( "jaguar.wav", SoundType_Music );
		}

		if (soundDesc.channelID != ChannelID_none)
		{
			addChannel(soundDesc.channelID);
		}
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
			playerChannels[playerId]->channels[i]->addChannelToGroup( getChannelGroup(ChannelGroup_GameSFX));
		}
		AudioAPI::Audio_PlaySound(getSound(SoundID_Player_Walk), getPlayerChannel(playerId, PlayerSoundID_Walk), true);
		getPlayerChannel(playerId, PlayerSoundID_Walk)->setVolym(0.2f);

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

void C_AudioHandler::addChannel(ChannelID id )
{
	if (!channels[id])
	{
		channels[id] = AudioAPI::Audio_CreateChannel();
	}
}
void C_AudioHandler::PlaySoundOnChannel( ISound* sound, IChannel* channel, PlayMode playMode )
{
	if(!sound || ! channel) return;

	if (mute)
	{
		// don't play sounds while muted
		return;
	}
	if( playMode == PlayMode_Restart)
	{
		if(channel->getChannelPlaying())
		{
			// play from the beginning of the sound on the channel
			channel->restartChannel();
			channel->SetPauseChannel(false);
		}
		else
		{
			// start new sound if nothing was playing
			Sound::AudioAPI::Audio_PlaySound(sound, channel);
		}
	}
	else if( playMode == PlayMode_FinnishSound )
	{
		// start sound only when it is not already playing
		if(!channel->getChannelPlaying())
			Sound::AudioAPI::Audio_PlaySound(sound, channel);
	}
	else if( playMode == PlayMode_AlwaysStart )
	{
		// multiple sounds can play at once on the channel
		Sound::AudioAPI::Audio_PlaySound(sound, channel);
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
Sound::IChannelGroup* C_AudioHandler::getChannelGroup( ChannelGroups id )
{
	return channelGroups[id];
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
		if(channel->second)
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
void C_AudioHandler::MuteSound(bool mute)
{
	this->mute = mute;
	/*for (int i = 0; i < maxCollisionSounds; i ++)
	{
	collisionChannels[i]->SetMuteChannel(true);
	}
	auto channel = this->channels.begin();
	for( ; channel != this->channels.end(); ++channel )
	{
	channel->second->SetMuteChannel(true);
	}
	auto playerChannel = this->playerChannels.begin();
	for( ; playerChannel != this->playerChannels.end(); ++playerChannel )
	{

	for (int i = 0; i < PlayerSoundID_Count; i ++)
	{
	if(playerChannel->second)
	{
	playerChannel->second->channels[i]->SetMuteChannel(true);
	}
	}
	}*/
}
void C_AudioHandler::Release()
{
	for (int i = 0; i < maxCollisionSounds; i ++)
	{
		delete collisionChannels[i];
	}
	delete [] collisionChannels;

	for (int i = 0; i < ChannelGroup_Count; i ++)
	{
		delete channelGroups[i];	
	}
	delete [] channelGroups;

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
