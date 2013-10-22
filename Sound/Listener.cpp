#include <cstdlib>
#include <cstdio>

#include <AL/al.h>

#include "Listener.h"
#include "SoundEngine.h"

void Listener::SetMasterVolume(float volume)
{
	alListenerf(AL_GAIN, volume);
}

void Listener::SetPosition(const float position[3])
{
	const float scaledPosition[3] = {position[0] * SOUNDENGINE_POSITION_SCALE, position[1] * SOUNDENGINE_POSITION_SCALE, position[2] * SOUNDENGINE_POSITION_SCALE};
	alListenerfv(AL_POSITION, scaledPosition);
}

void Listener::SetOrientation(const float forward[3], const float up[3])
{
	float values[6] = {forward[0], forward[1], forward[2], up[0], up[1], up[2]};
	alListenerfv(AL_ORIENTATION, values);
}

void Listener::SetVelocity(const float velocity[3])
{
	alListenerfv(AL_VELOCITY, velocity);
}
