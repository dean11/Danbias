#ifndef SOUNDRESOURCE_H
#define SOUNDRESOURCE_H

#include <cassert>
#include <string>

#include <AL/al.h>

struct SoundResourceInfo
{
	ALenum format;
	size_t totalSamples;
	int channels;
	int sampleRate;
	int bitDepth;
};

class SoundResource
{
public:
	virtual ~SoundResource()
	{
	}
	std::string GetName()
	{
		return m_name;
	}
	bool IsStreaming()
	{
		return m_isStreaming;
	}
	float GetDuration()
	{
		return m_duration;
	}
	// time is in seconds.
	size_t GetSampleCountForTime(float time)
	{
		assert(time >= 0.0f);

		return (size_t)(time * m_info.sampleRate * m_info.channels);
	}
	size_t GetTotalSampleCount()
	{
		return m_info.totalSamples;
	}

	virtual ALuint GetBuffer() = 0;
	// This function will block while it's reading from the disk.
	virtual void FillBuffer(ALuint buffer, size_t sampleOffset, size_t sampleCount) = 0;
	
protected:
	SoundResource(const std::string &name, const SoundResourceInfo &info, bool isStreaming)
	{
		m_name = name;
		m_info = info;
		m_isStreaming = isStreaming;
		m_duration = (info.totalSamples / info.channels) / (float)info.sampleRate;

		//printf("Created SoundResouce '%s'\n", name.c_str());
		//printf("streaming=%s, duration=%.2fs, channels=%i, sampleRate=%.1fkHz, bitDepth=%i\n", (isStreaming? "true" : "false"), m_duration, info.channels, info.sampleRate / 1000.0f, info.bitDepth);
	}

	std::string m_name;
	SoundResourceInfo m_info;
	bool m_isStreaming;
	float m_duration;
};

#endif
