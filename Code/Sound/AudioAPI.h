#ifndef AUDIO_API_H
#define AUDIO_API_H
#define NOMINMAX
#include <vld.h>
#include "AudioManager.h"

#ifdef SOUND_DLL_EXPORT
#define SOUND_DLL __declspec(dllexport)
#else
#define SOUND_DLL __declspec(dllimport)
#endif

namespace Sound
{
	extern "C"
	{
		class SOUND_DLL AudioAPI
		{
		public:

			static void Init();
			static void Shutdown();
			static SoundData* Audio_CreateSound(std::string soundName, bool stream = false);
			static void Audio_PlaySound(SoundData* sound);
			static void Audio_DeleteSound(SoundData* sound);
			//static void Audio_Update(float* pos, float speed);
			static void Audio_Update();
		};
	}
}
#endif