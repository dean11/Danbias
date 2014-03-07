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
			SoundType_Effect
		};
		class SOUND_DLL ISound
		{
		public:
			virtual void ReleaseSound() = 0;
			virtual void Play_Sound(bool paused = false) = 0;
			virtual void setVolym(float volym) = 0;
			virtual void setMinMaxDistance( float min, float max) = 0; 
			virtual void setChannel3DAttributes( float* pos, float* vel) = 0;
			virtual void setMode(SoundMode soundMode) = 0;
			virtual void SetPauseChannel(bool pause) = 0;
			virtual bool GetPauseChannel()= 0;
			virtual void toggleChannelPaused() = 0;
			virtual SoundType getType() = 0;
			virtual void setSoundVolume() = 0;
		};

		class SOUND_DLL AudioAPI
		{
		public:
			static bool Init();
			static void Shutdown();
			static bool Audio_Update();
			static void setListenerPos( float* pos, float* vel, float* forward, float* up ); 
			static ISound* Audio_CreateSound(std::string soundName, bool stream = false);
		};
	}
}
#endif