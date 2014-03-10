#ifndef AUDIO_API_H
#define AUDIO_API_H
#define NOMINMAX
#include <vld.h>
#include <string>

#ifdef SOUND_DLL_EXPORT
#define SOUND_DLL __declspec(dllexport)
#else
#define SOUND_DLL __declspec(dllimport)
#endif

namespace Sound
{
	extern "C"
	{
		enum SoundMode
		{
			Loop_normal,
			Loop_off
		};
		enum SoundType
		{
			SoundType_Music,
			SoundType_Effect,
			SoundType_Effect3D
		};
		class SOUND_DLL IChannelGroup
		{
		public:
			virtual void setVolym(float volym) = 0;
			virtual void stop() = 0;
		};

		class SOUND_DLL IChannel
		{
		public:
			virtual void setVolym(float volym) = 0;
			virtual void setMinMaxDistance( float min, float max) = 0; 
			virtual void setChannel3DAttributes( float* pos, float* vel) = 0;
			virtual void SetPauseChannel(bool pause) = 0;
			virtual bool GetPauseChannel()= 0;
			virtual void toggleChannelPaused() = 0;
			virtual void setSoundVolume() = 0;
			virtual bool getChannelPlaying() = 0;
			virtual void stop() = 0;
			virtual void restartChannel() = 0;
			virtual void addChannelToGroup(IChannelGroup* channelGroup) = 0;
			virtual void SetMuteChannel(bool mute) = 0;
		};

		class SOUND_DLL ISound
		{
		public:
			virtual void ReleaseSound() = 0;
			virtual void setMode(SoundMode soundMode) = 0;
			virtual SoundType getType() = 0;
		};

		class SOUND_DLL AudioAPI
		{
		public:
			static bool			Audio_Init();
			static void			Audio_Shutdown();
			static bool			Audio_Update( float deltaTime);
			static IChannelGroup*	Audio_CreateChannelGroup(const char* groupName);
			static IChannel*	Audio_CreateChannel();
			static ISound*		Audio_CreateSound(std::string soundName, SoundType soundType);
			static void			Audio_PlaySound( ISound* sound, IChannel* channel, bool paused = false);
			static void			Audio_SetListenerPos( float* pos, float* vel, float* forward, float* up ); 
		};
	}
}
#endif