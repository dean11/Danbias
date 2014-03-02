#ifndef AUDIOCLASS_H
#define AUDIOCLASS_H
#define NOMINMAX
//#if defined(_WIN64)
//#pragma comment(lib, "Dependencies/Lib/FMod/fmodex64_vc.lib")
//#else
//#pragma comment(lib, "Dependencies/Lib/FMod/fmodex_vc.lib")
//#endif
//#include "fmod.hpp"
#include "fmod.hpp"
#include "fmod_errors.h"
//#include <FMod/fmod.hpp>
#include "OysterMath.h"
#include <map>

const float STANDARD_MUSIC_VOLYM = 0.5f;
const float STANDARD_EFFECTS_VOLYM = 0.5f;
const int maxNrOfChannels = 100;
namespace Sound
{
	enum SoundType
	{
		SoundType_Music,
		SoundType_Effect
	};

	enum MenuSounds
	{	
		MenuSoundTrack,
		MenuHover,
		MenuClick,

		SOUNDMENU_COUNT
	};

	enum LevelSounds
	{	
		LevelMenuSoundTrack,
		LevelMenuHover,
		LevelMenuClick,

		FlySound_LevelSoundTrack,
		FlySound_Collision,
		FlySound_EnergyPickup, 
		FlySound_CargoPickup, 
		FlySound_LowEnergy, 
		FlySound_NoEnergy, 
		FlySound_Thrust, 


		FlySound_Wind,

		SOUNDLEVEL_COUNT
	};

	struct SoundData
	{
		FMOD::Sound* sound;
		FMOD::Channel* channel;
		SoundType soundType;
	};

	class AudioManager
	{
	public:
		struct	playerSoundData
		{
			Oyster::Math::Float3 pos;
			float vel; 
		};

		
	private:
		static bool instanceFlag;
		static AudioManager* single;
		FMOD::System * fmodSystem;
		FMOD_RESULT result;

		float music_volume;
		float effects_volume;
		
		AudioManager(void);

		FMOD_VECTOR listenerPos; 
		void unLoadSounds();
		bool FmodErrorCheck(FMOD_RESULT result);
		std::string basePath;
	public:

		~AudioManager(void);

		static AudioManager* self();

		void intitializeSoundManager();
		void shutdownSoundManager();
		void updateSoundManager();
		// set stream to true if it is a big sound file
		SoundData* CreateSound(std::string soundName, bool stream = false);
		void Play_Sound(SoundData* sound);
		void DeleteSound(SoundData* sound);

		void setLoop(SoundData* sound, bool loop);
		// setVolym( SoundData* );

		void setMusicVolym(float volym);
		void setEffectsVolym(float volym);
		// setBasePath();
		
		
		void toggleSound(SoundData* sound);
		void pauseAllSound();

	};
}
#endif
