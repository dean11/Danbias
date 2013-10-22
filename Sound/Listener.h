#ifndef LISTENER_H
#define LISTENER_H

class Listener
{
public:
	static void SetMasterVolume(float volume);

	static void SetPosition(const float position[3]);
	static void SetOrientation(const float forward[3], const float up[3]);
	// Used for the doppler effect, should be in the same unit as the speed of sound set in SoundEngine.
	static void SetVelocity(const float velocity[3]);
};

#endif
