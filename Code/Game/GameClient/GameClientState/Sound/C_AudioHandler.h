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

		enum PlayMode
		{
			PlayMode_Restart,
			PlayMode_FinnishSound,
			PlayMode_AlwaysStart,
		};
		enum SoundID
		{
			// menu sounds 
			SoundID_Mouse_Click,
			SoundID_Mouse_Hover,
			
			// soundtracks
			SoundID_Menu_SoundTrack,
			SoundID_Game_SoundTrack,
			SoundID_Game_GameOver,

			// event sounds
			SoundID_PickUpHealth,
			SoundID_Jumppad,
			SoundID_CrystalCollision,
			SoundID_CrateExplosion,
			SoundID_BoxVsBoxCollision, 
			SoundID_BoxVsPlayerCollision,
			SoundID_Portal,

			// player sounds
			SoundID_Player_Walk,
			SoundID_Player_Jump,
			SoundID_Player_WeaponPull,
			SoundID_Player_WeaponPush, 
			SoundID_Player_WeaponShoot,
			SoundID_Player_DMGtaken, 
			SoundID_Player_Heal, 
			SoundID_Player_Respawn, 
			SoundID_Player_Die, 
			SoundID_Player_Join, 

			//ambient,
			SoundID_Pickup_Music,

		};
		enum ChannelID
		{
			ChannelID_Mouse_Hover_Button1,
			ChannelID_Mouse_Click_Button1,
			ChannelID_Mouse_Hover_Button2,
			ChannelID_Mouse_Click_Button2,
			ChannelID_Mouse_Hover_Button3,
			ChannelID_Mouse_Click_Button3,
			ChannelID_Mouse_Hover_Button4,
			ChannelID_Mouse_Click_Button4,

			ChannelID_Menu_Soundtrack, 
			ChannelID_Game_Soundtrack, 
			ChannelID_Game_GameOver,
			
			ChannelID_effectSound, 
			//ChannelID_pickUpSound,
			ChannelID_jumppad,
			ChannelID_collision,
			ChannelID_portal,
			ChannelID_ambient,

			ChannelID_none,
		};
		enum PlayerSoundID
		{
			PlayerSoundID_Walk,
			PlayerSoundID_Jump,
			PlayerSoundID_Pull,
			PlayerSoundID_Push,
			PlayerSoundID_Shoot,
			PlayerSoundID_DMGtaken,
			PlayerSoundID_Heal,
			PlayerSoundID_Respawn, 
			PlayerSoundID_Join, 
			PlayerSoundID_Die,
			PlayerSoundID_Count,
		};
		enum ChannelGroups
		{
			ChannelGroup_Master,
			ChannelGroup_MenuMusic,
			ChannelGroup_MenuSFX,
			ChannelGroup_GameMusic,
			ChannelGroup_GameSFX,
			ChannelGroup_Count,
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
			std::string soundName;
			SoundID soundID;
			ChannelID channelID;
			SoundDesc(){}
			SoundDesc( std::string soundName, SoundID soundID, ChannelID channelID = ChannelID_none)
				:soundName(soundName), soundID(soundID), channelID(channelID){}
		};
		class C_AudioHandler
		{
		private:
			::std::map<int, Sound::ISound*> songs;
			::std::map<int, Sound::ISound*> effects;
			::std::map<int, Sound::IChannel*> channels;

			::std::map<int, PlayerSounds*> playerChannels;

			Sound::IChannelGroup** channelGroups;
			Sound::IChannel** collisionChannels;
			int maxCollisionSounds;
			int currCollisionSound;

			float music_volume;
			float effects_volume;
			bool mute;

		public:
			C_AudioHandler();
			~C_AudioHandler();
			bool Init();
			void addSFX(SoundDesc soundDesc);
			void addMusic(SoundDesc soundDesc);
			void addSFX_3D(SoundDesc soundDesc);
			void addPlayerSound( int playerId);
			void setPlayerChannelPos( int playerId, float* pos, float* vel);
			void addChannel( ChannelID id );
			void PlaySoundOnChannel( Sound::ISound* sound, Sound::IChannel* channel, PlayMode playMode );
			Sound::ISound* getSound( SoundID id );
			Sound::ISound* getSFX( SoundID id );
			Sound::IChannelGroup* getChannelGroup( ChannelGroups id );
			Sound::IChannel* getChannel( ChannelID id );
			Sound::IChannel* getPlayerChannel( int  id, PlayerSoundID soundId );
			Sound::IChannel* getCollisionChannel();
			void pauseAllSounds();
			void SetSoundVolume(float volume);
			void SetEffectVolume(float volume);
			void MuteSound(bool mute);
			void Release();
		};
	}
}

#endif
