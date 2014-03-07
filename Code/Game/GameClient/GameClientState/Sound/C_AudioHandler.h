#ifndef DANBIAS_SOUNDMANAGER_H
#define DANBIAS_SOUNDMANAGER_H
#include "AudioAPI.h"
#include <map>
#include "Utilities.h"

namespace DanBias
{
	namespace Client
	{
		const float STANDARD_MUSIC_VOLYM = 0.5f;
		const float STANDARD_EFFECTS_VOLYM = 0.5f;
		enum SoundID
		{
			backgroundSound,
			effectSound, 
			pickUpSound,
			jumppad,
			walk,
			walk1,
			jump,
			shoot,
			shoot1,
			pull, 
			collision,
			ambient,
			mouse_hoover,
			mouse_click,


		};
		enum ChannelID
		{
			ChannelID_backgroundSound,
			ChannelID_effectSound, 
			ChannelID_pickUpSound,
			ChannelID_jumppad,
			ChannelID_walk,
			ChannelID_walk1,
			ChannelID_jump,
			ChannelID_shoot,
			ChannelID_shoot1,
			ChannelID_pull, 
			ChannelID_collision,
			ChannelID_ambient,
			ChannelID_mouse_hoover_button1,
			ChannelID_mouse_click_button1,
			ChannelID_mouse_hoover_button2,
			ChannelID_mouse_click_button2,
			ChannelID_mouse_hoover_button3,
			ChannelID_mouse_click_button3,
		};
		enum PlayerSoundID
		{
			PlayerSoundID_Jump,
			PlayerSoundID_walk,
			PlayerSoundID_shoot,
			PlayerSoundID_pull,
			playerSoundID_dmgRecieved,
			PlayerSoundID_Count,
		};
		struct PlayerSounds
		{
			Sound::IChannel** channels;

			PlayerSounds()
			{
				channels = new Sound::IChannel*[PlayerSoundID_Count];
			}
		};
		struct SoundDesc
		{
			std::string name;
			SoundID ID;

			SoundDesc(){}
			SoundDesc( std::string name, SoundID id)
				:name(name), ID(id){}
		};
		class C_AudioHandler
		{
		private:
			::std::map<int, Sound::ISound*> songs;
			::std::map<int, Sound::ISound*> effects;
			::std::map<int, Sound::IChannel*> channels;

			::std::map<int, PlayerSounds*> playerChannels;

			Sound::IChannel** collisionChannels;
			int maxCollisionSounds;
			int currCollisionSound;
			float music_volume;
			float effects_volume;

		public:
			C_AudioHandler();
			~C_AudioHandler();
			bool Init();
			void addSFX(SoundDesc soundDesc);
			void addMusic(SoundDesc soundDesc);
			void addSFX_3D(SoundDesc soundDesc);
			void addPlayerSound( int playerId);
			void setPlayerChannelPos( int playerId, float* pos, float* vel);
			void addChannel( SoundID id );
			void addChannel( ChannelID id );
			void playCollisionSound();
			Sound::ISound* getSound( SoundID id );
			Sound::ISound* getSFX( SoundID id );
			Sound::IChannel* getChannel( SoundID id );
			Sound::IChannel* getChannel( ChannelID id );
			Sound::IChannel* getPlayerChannel( int  id, PlayerSoundID soundId );
			void SetSoundVolume(float volume);
			void SetEffectVolume(float volume);
			void Release();
		};
	}
}

#endif
