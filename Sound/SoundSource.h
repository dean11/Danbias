#ifndef SOUNDSOURCE_H
#define SOUNDSOURCE_H

#include <string>

#include <AL/al.h>

class SoundEngine;
class SoundResource;

class SoundSource
{
public:
	virtual ~SoundSource();

	void SetResource(const char *name);
	SoundResource *GetResource();

	bool IsPlaying();
	bool IsLooping();

	void Play();
	void Pause();

	// time is in seconds.
	void Seek(float time);

	void SetPosition(const float position[3]);
	// Used for the doppler effect, should be in the same unit as the speed of sound set in SoundEngine.
	void SetVelocity(const float velocity[3]);

	void SetVolume(float volume);
	void SetLooping(bool looping);
	void SetPitch(float pitch);

	// Sets whether position and velocity should be relative to the listener or not.
	void SetIsRelativeToListener(bool relative);

private:
	// Limit creating SoundSources to the SoundEngine.
	friend class SoundEngine;

	SoundSource();

	void ReleaseAllQueuedBuffers();

private:
	ALuint m_source;
	SoundResource *m_resource;

	bool m_isPlaying;
	bool m_isLooping;

	bool m_isLoadingBuffer;
	bool m_isLoadingResource;
	bool m_cancelBufferLoading;

	size_t m_nextSample;
	float m_pendingSeek;
};

#endif
