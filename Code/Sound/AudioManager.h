#ifndef AUDIOCLASS_H
#define AUDIOCLASS_H
#define NOMINMAX
#include "fmod.hpp"
#include "fmod_errors.h"
#include "AudioAPI.h"
#include "OysterMath.h"
#include <map>

const float STANDARD_MUSIC_VOLYM = 0.5f;
const float STANDARD_EFFECTS_VOLYM = 0.5f;
const int maxNrOfChannels = 100;
namespace Sound
{


	struct SoundData : ISound
	{
		FMOD::Sound* sound;
		FMOD::Channel* channel;
		SoundType soundType;

	public:
		SoundData();
		~SoundData();
		void ReleaseSound() override;
		void Play_Sound(bool paused) override;
		void setVolym(float volym) override;
		void setMinMaxDistance( float min, float max) override;
		void setChannel3DAttributes( float* pos, float* vel) override;
		void setMode(SoundMode soundMode) override;
		void SetPauseChannel(bool pause) override;
		bool GetPauseChannel()override;
		void toggleChannelPaused() override;
		void setSoundVolume()override;
		SoundType getType() override;
	};
	void floatArrToFmodVECTOR(FMOD_VECTOR& F_Vec, float* floatArr);
	FMOD_VECTOR floatArrToFmodVECTOR( float* floatArr);

	class AudioManager
	{
	public:
		struct	playerSoundData
		{
			Oyster::Math::Float3 pos;
			float vel; 
		};

		struct ListenerData
		{
			FMOD_VECTOR pos;
			FMOD_VECTOR vel;
			FMOD_VECTOR forward;
			FMOD_VECTOR up;
		};
		
	private:
		static bool instanceFlag;
		static AudioManager* single;
		FMOD::System * fmodSystem;
		FMOD_RESULT result;

		float music_volume;
		float effects_volume;
		
		AudioManager(void);

		ListenerData listenerData;
		void unLoadSounds();
		bool FmodErrorCheck(FMOD_RESULT result);
		std::string basePath;
		
	public:

		~AudioManager(void);

		static AudioManager* self();

		bool intitializeSoundManager();
		void shutdownSoundManager();
		bool updateSoundManager();
		// set stream to true if it is a big sound file
		SoundData* CreateSound(std::string soundName, bool stream = false);
		void Play_Sound(SoundData* sound, bool paused);
		void DeleteSound(SoundData* sound);

		void setListener( float* pos, float* vel, float* forward, float* up );

		void setLoop(SoundData* sound, bool loop);
		// setVolym( SoundData* );

		void setMusicVolym(float volym);
		void setEffectsVolym(float volym);
		void setBasePath(std::string basePath);

	};
}
#endif
