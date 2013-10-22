#ifndef SOUNDENGINE_H
#define SOUNDENGINE_H

#include <cassert>
#include <vector>

#include <AL/al.h>
#include <AL/alc.h>

#include "Listener.h"
#include "SoundSource.h"
#include "SoundResource.h"
#include "ThreadPool.h"
#include "Util.h"

#define SOUNDENGINE_POSITION_SCALE         0.01f
#define SOUNDENGINE_BUFFER_POOL_SIZE       128
// Sound files larger than this will be streamed.
#define SOUNDENGINE_STREAMING_SIZE_LIMIT   (1 * 1024 * 1024)
// Maximum number of buffers that can be queued at once for a streaming source.
#define SOUNDENGINE_STREAMING_BUFFER_COUNT 3
// How many samples, in seconds, each buffer will hold for streaming resources.
#define SOUNDENGINE_STREAMING_BUFFER_SIZE  0.25

class SoundEngine
{
public:
	static bool Init();
	static void Release();

	static void SetDopplerFactor(float dopplerFactor);
	// Used for the doppler effect, default value is 343.3 (Sound travels through air at ~343m/s)
	static void SetSpeedOfSound(float speed);

	static SoundSource *CreateSoundSource();
	static void DestroySoundSource(SoundSource *source);

	static void Update();

private:
	// Allow SoundSources to get SoundResources.
	friend class SoundSource;

	// If the resource is not currently loaded, this function will block while reading the resource from disk.
	// These functions are thread safe.
	static SoundResource *GetSoundResource(const std::string &name);
	static void ReleaseSoundResource(SoundResource *resource);

	// Allow SoundResources to use the buffer pool.
	friend class SoundResourceWAV;

	// These functions are thread safe.
	static ALuint GetBuffer();
	static void ReleaseBuffer(ALuint buffer);

private:
	static ALCdevice *m_device;
	static ALCcontext *m_context;

	static ThreadPool *m_threadPool;

	static ALuint m_bufferPool[SOUNDENGINE_BUFFER_POOL_SIZE];
	static size_t m_bufferPoolSize;

	static Mutex *m_bufferPoolMutex;
	static Mutex *m_resourceMutex;

	static std::vector<RefCounted<SoundResource *>> m_soundResources;
	static std::vector<SoundSource *> m_soundSources;

	static std::vector<SoundSource *> m_sourcesPendingDeletion;
};

#endif
