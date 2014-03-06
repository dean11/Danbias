#ifndef DANBIAS_SOUNDMANAGER_H
#define DANBIAS_SOUNDMANAGER_H
#include "AudioAPI.h"
#include <map>
#include "Utilities.h"

namespace DanBias
{
	namespace Client
	{
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
		public:
			C_AudioHandler();
			~C_AudioHandler();
			bool Init();
			void addSound(SoundDesc soundDesc);
			Sound::ISound* getSound( SoundID id );
			void SetSoundVolume(float volume);
			void SetEffectVolume(float volume);
			void Release();
		};
	}
}

#endif
