#ifndef AUDIOCLASS_H
#define AUDIOCLASS_H
#define NOMINMAX
#include "fmod.hpp"
#include "fmod_errors.h"
#include "AudioAPI.h"
#include "OysterMath.h"
#include <map>


const int maxNrOfChannels = 100;
namespace Sound
{

	struct ChannelGroupData : IChannelGroup
	{
		FMOD::ChannelGroup* channelgroup;
	public:
		ChannelGroupData();
		~ChannelGroupData();
		void setVolym(float volym) override;
		void stop()override;
	};
	struct ChannelData : IChannel
	{
		FMOD_RESULT result;
		FMOD::Channel* channel;
	public:
		ChannelData();
		~ChannelData();
		void setVolym(float volym) override;
		void setMinMaxDistance( float min, float max) override;
		void setChannel3DAttributes( float* pos, float* vel) override;
		void SetPauseChannel(bool pause) override;
		bool GetPauseChannel()override;
		void toggleChannelPaused() override;
		void setSoundVolume()override;
		bool getChannelPlaying()override;
		void stop()override;
		void restartChannel()override;
		void addChannelToGroup(IChannelGroup* channelGroup)override;
		void SetMuteChannel(bool mute)override;
	};
	struct SoundData : ISound
	{
		FMOD::Sound* sound;
		SoundType soundType;

	public:
		SoundData();
		~SoundData();
		void ReleaseSound() override;
		void setMode(SoundMode soundMode) override;
		SoundType getType() override;
	};
	namespace FmodUtil
	{
		void floatArrToFmodVECTOR(FMOD_VECTOR& F_Vec, float* floatArr);
		FMOD_VECTOR floatArrToFmodVECTOR( float* floatArr);
		bool FmodErrorCheck(FMOD_RESULT result);
	}


	class AudioManager
	{
	public:
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

		AudioManager(void);
		ListenerData listenerData;
		void unLoadSounds();
		
		std::string basePath;
		
	public:

		~AudioManager(void);

		static AudioManager* self();

		bool intitializeSoundManager();
		void shutdownSoundManager();
		bool updateSoundManager(float deltaTime);
		ChannelGroupData* CreateChannelGroup(const char* groupName);
		ChannelData* CreateChannel();
		// set stream to true if it is a big sound file
		SoundData* CreateSound(std::string soundName, SoundType soundType);
		void Play_Sound(SoundData* sound, ChannelData* channel, bool paused);

		void setListenerPos( float* pos, float* vel, float* forward, float* up );

		void setLoop(SoundData* sound, bool loop);
		// setVolym( SoundData* );

		void setBasePath(std::string basePath);

	};
}
#endif
