#include <cstdlib>
#include <cstdio>
#include <algorithm>

#include <AL/al.h>

#include "ThreadPool.h"
#include "SoundEngine.h"
#include "SoundSource.h"
#include "SoundResource.h"
#include "SoundResourceWAV.h"

ALCdevice *SoundEngine::m_device = NULL;
ALCcontext *SoundEngine::m_context = NULL;

ThreadPool *SoundEngine::m_threadPool = NULL;

ALuint SoundEngine::m_bufferPool[SOUNDENGINE_BUFFER_POOL_SIZE];
size_t SoundEngine::m_bufferPoolSize = 0;

Mutex *SoundEngine::m_bufferPoolMutex = NULL;
Mutex *SoundEngine::m_resourceMutex = NULL;

std::vector<RefCounted<SoundResource *>> SoundEngine::m_soundResources;
std::vector<SoundSource *> SoundEngine::m_soundSources;

std::vector<SoundSource *> SoundEngine::m_sourcesPendingDeletion;

bool SoundEngine::Init()
{
	m_device = alcOpenDevice(NULL);

	if (!m_device)
	{
		printf("Failed to open OpenAL device\n");
		return false;
	}

	m_context = alcCreateContext(m_device, NULL);

	if (!m_context)
	{
		printf("Failed to create OpenAL context\n");
		alcCloseDevice(m_device);
		return false;
	}

	alcMakeContextCurrent(m_context);

	printf("OpenAL '%s' running on '%s'\n", alGetString(AL_VERSION), alGetString(AL_RENDERER));

#if 1
	ALCenum error = alcGetError(m_device);

	if (error != ALC_NO_ERROR)
		printf("alcGetError=%s\n", alcGetString(m_device, error));
#endif

	m_threadPool = new ThreadPool();

	alGenBuffers(SOUNDENGINE_BUFFER_POOL_SIZE, m_bufferPool);
	m_bufferPoolSize = SOUNDENGINE_BUFFER_POOL_SIZE;

	m_bufferPoolMutex = new Mutex();
	m_resourceMutex = new Mutex();

	return true;
}

void SoundEngine::Release()
{
#if 1
	ALCenum error = alcGetError(m_device);

	if (error != ALC_NO_ERROR)
		printf("alcGetError=%s\n", alcGetString(m_device, error));
#endif

	// I think all currently queued tasks on the thread pool are canceled if we free it.
	delete m_threadPool;

#if 0
	SoundSource *source;

	while(!m_soundSources.empty())
	{
		source = m_soundSources.back();
		m_soundSources.pop_back();

		printf("Releasing sound source in SoundEngine::Release\n");

		delete source;
	}

	while(!m_sourcesPendingDeletion.empty())
	{
		source = m_sourcesPendingDeletion.back();
		m_sourcesPendingDeletion.pop_back();

		printf("Releasing sound source in SoundEngine::Release\n");

		delete source;
	}

	SoundResource *resource;

	while(!m_soundResources.empty())
	{
		resource = m_soundResources.back().GetValue();
		m_soundResources.pop_back();

		printf("Releasing resource '%s' in SoundEngine::Release\n", resource->GetName().c_str());

		delete resource;
	}
#endif

	//assert(m_bufferPoolSize == SOUNDENGINE_BUFFER_POOL_SIZE);
	printf("%i buffers missing from the pool at SoundEngine::Release\n", SOUNDENGINE_BUFFER_POOL_SIZE - (int)m_bufferPoolSize);

	alDeleteBuffers((ALsizei)m_bufferPoolSize, m_bufferPool);

	alcMakeContextCurrent(NULL);
	alcDestroyContext(m_context);
	alcCloseDevice(m_device);

	delete m_bufferPoolMutex;
	delete m_resourceMutex;
}

SoundResource *SoundEngine::GetSoundResource(const std::string &name)
{
	SoundResource *resource = NULL;

	m_resourceMutex->Lock();

	for(auto i = m_soundResources.begin(); i != m_soundResources.end(); i++)
	{
		if ((*i).GetValue()->GetName() == name)
		{
			(*i).IncRefCount();
			resource = (*i).GetValue();

			break;
		}
	}

	m_resourceMutex->Unlock();

	if (!resource)
	{
		resource = SoundResourceWAV::Load(name);

		if (resource)
		{
			m_resourceMutex->Lock();
			m_soundResources.push_back(RefCounted<SoundResource *>(resource));
			m_resourceMutex->Unlock();
		}
	}

	return resource;
}

void SoundEngine::ReleaseSoundResource(SoundResource *resource)
{
	m_resourceMutex->Lock();

	for(auto i = m_soundResources.begin(); i != m_soundResources.end(); i++)
	{
		if ((*i).GetValue() == resource)
		{
			int refCount = (*i).DecRefCount();

			if (refCount == 0)
			{
				//printf("'%s' reached refcount 0, deleting\n", (*i).GetValue()->GetName().c_str());

				delete (*i).GetValue();
				m_soundResources.erase(i);
			}

			break;
		}
	}

	m_resourceMutex->Unlock();
}

void SoundEngine::SetDopplerFactor(float dopplerFactor)
{
	alDopplerFactor(dopplerFactor);
}

void SoundEngine::SetSpeedOfSound(float speed)
{
	alSpeedOfSound(speed);
}

SoundSource *SoundEngine::CreateSoundSource()
{
	SoundSource *source = new SoundSource();
	m_soundSources.push_back(source);

	return source;
}

void SoundEngine::DestroySoundSource(SoundSource *source)
{
	for(auto i = m_soundSources.begin(); i != m_soundSources.end(); i++)
	{
		if ((*i) == source)
		{
			m_soundSources.erase(i);
			break;
		}
	}

	// If we delete it immediately then the resource/buffer loading task will try to access the freed memory.
	if (source->m_isLoadingBuffer || source->m_isLoadingResource)
	{
		alSourceStop(source->m_source);
		m_sourcesPendingDeletion.push_back(source);
	}
	else
	{
		delete source;
	}
}

ALuint SoundEngine::GetBuffer()
{
	ALuint buffer = AL_NONE;

	m_bufferPoolMutex->Lock();

	if (m_bufferPoolSize > 0)
	{
		m_bufferPoolSize--;
		buffer = m_bufferPool[m_bufferPoolSize];
	}
	else
	{
		printf("The sound buffer pool is empty!\n");
	}

	m_bufferPoolMutex->Unlock();

	return buffer;
}

void SoundEngine::ReleaseBuffer(ALuint buffer)
{
	// Free the data stored in the buffer.
	alBufferData(buffer, AL_FORMAT_MONO8, NULL, 0, 0);

	m_bufferPoolMutex->Lock();
	assert(m_bufferPoolSize < SOUNDENGINE_BUFFER_POOL_SIZE);
	m_bufferPool[m_bufferPoolSize++] = buffer;
	m_bufferPoolMutex->Unlock();
}

void SoundEngine::Update()
{
	SoundSource *source;
	SoundResource *resource;
	ALint state;
	ALuint buffer = AL_NONE;
	size_t sampleOffset, sampleCount;
	bool hasReachedEnd, shouldStop;
	int processedCount = 0;
	int queuedCount = 0;
	int j;

	for(size_t i = 0; i < m_sourcesPendingDeletion.size(); i++)
	{
		source = m_sourcesPendingDeletion[i];

		if (!source->m_isLoadingBuffer && !source->m_isLoadingResource)
		{
			m_sourcesPendingDeletion.erase(m_sourcesPendingDeletion.begin() + i);
			i--;

			delete source;

			// printf("Deleted source in m_sourcesPendingDeletion\n");
		}
	}

	for(auto i = m_soundSources.begin(); i != m_soundSources.end(); i++)
	{
		source = (*i);
		resource = source->m_resource;

		if (!resource)
			continue;

		if (!resource->IsStreaming())
		{
			alGetSourcei(source->m_source, AL_SOURCE_STATE, &state);

			if (!source->m_isLoadingResource && source->m_isPlaying && state != AL_PLAYING)
			{
				source->m_isPlaying = false;

				//printf("Reached end of non-streaming source\n");
			}
		}
		else
		{
			alGetSourcei(source->m_source, AL_BUFFERS_PROCESSED, &processedCount);

			for(j = 0; j < processedCount; j++)
			{
				alSourceUnqueueBuffers(source->m_source, 1, &buffer);
				ReleaseBuffer(buffer);
			}

			alGetSourcei(source->m_source, AL_BUFFERS_QUEUED, &queuedCount);
			hasReachedEnd = (source->m_nextSample == resource->GetTotalSampleCount());
			shouldStop = (hasReachedEnd && !source->IsLooping());

			if (shouldStop && queuedCount == 0)
			{
				source->m_isPlaying = false;
				// Immediately start loading samples from the beginning again.
				shouldStop = false;
			}

			if (!source->m_isLoadingBuffer && !shouldStop && queuedCount < SOUNDENGINE_STREAMING_BUFFER_COUNT)
			{
				if (hasReachedEnd)
					source->m_nextSample = 0;

				sampleOffset = source->m_nextSample;
				sampleCount = resource->GetSampleCountForTime(SOUNDENGINE_STREAMING_BUFFER_SIZE);
				sampleCount = std::min(sampleCount, resource->GetTotalSampleCount() - sampleOffset);

				assert(sampleCount > 0);

				std::function<void()> task = [source, resource, sampleOffset, sampleCount]() {
					if (source->m_cancelBufferLoading)
					{
						source->m_cancelBufferLoading = false;
						source->m_isLoadingBuffer = false;

						printf("Buffer loading was canceled\n");

						return;
					}

					ALuint buffer = GetBuffer();
					resource->FillBuffer(buffer, sampleOffset, sampleCount);
					alSourceQueueBuffers(source->m_source, 1, &buffer);

					ALint state;
					alGetSourcei(source->m_source, AL_SOURCE_STATE, &state);

					// If we were playing but ran out of buffers, restart playback.
					if (source->m_isPlaying && state != AL_PLAYING)
						source->Play();

					source->m_nextSample = sampleOffset + sampleCount;
					source->m_cancelBufferLoading = false;
					source->m_isLoadingBuffer = false;
				};

				source->m_isLoadingBuffer = true;
				m_threadPool->EnqueueTask(task);
			}
		}
	}

#if 1
	ALenum error = alGetError();

	if (error != AL_NO_ERROR)
		printf("alGetError=%s (0x%x)\n", alGetString(error), error);
#endif
}
