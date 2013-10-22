#include <cassert>
#include <algorithm>
#include <functional>

#include "SoundSource.h"
#include "SoundResource.h"
#include "SoundEngine.h"
#include "ThreadPool.h"

SoundSource::SoundSource()
{
	alGenSources(1, &m_source);
	m_resource = NULL;
	m_isPlaying = false;
	m_isLooping = false;
	m_isLoadingBuffer = false;
	m_isLoadingResource = false;
	m_cancelBufferLoading = false;
	m_nextSample = 0;
	m_pendingSeek = -1.0f;
}

SoundSource::~SoundSource()
{
	SetResource(NULL);
	alDeleteSources(1, &m_source);
}

void SoundSource::ReleaseAllQueuedBuffers()
{
	if (!m_resource || !m_resource->IsStreaming())
		return;

	int count = 0;
	ALuint buffer = AL_NONE;

	alSourceStop(m_source);
	alGetSourcei(m_source, AL_BUFFERS_QUEUED, &count);

	for(int i = 0; i < count; i++)
	{
		alSourceUnqueueBuffers(m_source, 1, &buffer);
		SoundEngine::ReleaseBuffer(buffer);
	}
}

void SoundSource::SetResource(const char *name)
{
	if (m_isLoadingResource)
	{
		printf("SoundSource::SetResource(\"%s\"), another resource is already being loaded!\n", name);
		return;
	}

	if (name && m_resource && m_resource->GetName() == std::string(name))
		return;

	if (m_isLoadingBuffer)
		m_cancelBufferLoading = true;

	// The source must be stopped before we can change any buffers.
	alSourceStop(m_source);

	if (m_resource)
	{
		ReleaseAllQueuedBuffers();
		alSourcei(m_source, AL_BUFFER, AL_NONE);
		SoundEngine::ReleaseSoundResource(m_resource);
	}

	m_resource = NULL;

	if (name)
	{
		SoundSource *source = this;

		std::function<void()> task = [source, name]() {
			SoundResource *resource = SoundEngine::GetSoundResource(name);

			if (resource)
			{
				if (!resource->IsStreaming())
				{
					alSourcei(source->m_source, AL_BUFFER, resource->GetBuffer());
					alSourcei(source->m_source, AL_LOOPING, (source->m_isLooping? AL_TRUE : AL_FALSE));
				}
				else
				{
					source->m_nextSample = 0;
					alSourcei(source->m_source, AL_LOOPING, AL_FALSE);
				}

				if (source->m_isPlaying)
					source->Play();
			}

			source->m_resource = resource;
			source->m_isLoadingResource = false;

			if (source->m_pendingSeek > 0.0f)
			{
				source->Seek(source->m_pendingSeek);
				source->m_pendingSeek = -1.0f;
			}
		};

		m_isLoadingResource = true;
		SoundEngine::m_threadPool->EnqueueTask(task);
	}
}

SoundResource *SoundSource::GetResource()
{
	return m_resource;
}

bool SoundSource::IsPlaying()
{
	return m_isPlaying;
}

bool SoundSource::IsLooping()
{
	return m_isLooping;
}

void SoundSource::Play()
{
	alSourcePlay(m_source);
	m_isPlaying = true;
}

void SoundSource::Pause()
{
	alSourcePause(m_source);
	m_isPlaying = false;
}

void SoundSource::Seek(float time)
{
	if (m_isLoadingResource)
	{
		m_pendingSeek = time;
		return;
	}

	if (!m_resource)
		return;

	size_t sample = std::min(m_resource->GetSampleCountForTime(std::max(time, 0.0f)), m_resource->GetTotalSampleCount() - 1);

	if (!m_resource->IsStreaming())
	{
		alSourcei(m_source, AL_SAMPLE_OFFSET, (ALint)sample);
	}
	else
	{
		if (m_isLoadingBuffer)
			m_cancelBufferLoading = true;

		ReleaseAllQueuedBuffers();
		m_nextSample = sample;
	}
}

void SoundSource::SetPosition(const float position[3])
{
	const float scaledPosition[3] = {position[0] * SOUNDENGINE_POSITION_SCALE, position[1] * SOUNDENGINE_POSITION_SCALE, position[2] * SOUNDENGINE_POSITION_SCALE};
	alSourcefv(m_source, AL_POSITION, scaledPosition);
}

void SoundSource::SetVelocity(const float velocity[3])
{
	alSourcefv(m_source, AL_VELOCITY, velocity);
}

void SoundSource::SetVolume(float volume)
{
	alSourcef(m_source, AL_GAIN, volume);
}

void SoundSource::SetLooping(bool looping)
{
	m_isLooping = looping;

	if (m_resource && !m_resource->IsStreaming())
		alSourcei(m_source, AL_LOOPING, (looping? AL_TRUE : AL_FALSE));
}

void SoundSource::SetPitch(float pitch)
{
	alSourcef(m_source, AL_PITCH, pitch);
}

void SoundSource::SetIsRelativeToListener(bool relative)
{
	alSourcei(m_source, AL_SOURCE_RELATIVE, (relative? AL_TRUE : AL_FALSE));
}
