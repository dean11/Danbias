#ifndef SOUNDRESOURCEWAV_H
#define SOUNDRESOURCEWAV_H

#include <string>
#include <fstream>

#include <AL/al.h>

#include "SoundResource.h"

class SoundResourceWAV : public SoundResource
{
public:
	virtual ~SoundResourceWAV();

	ALuint GetBuffer();
	void FillBuffer(ALuint buffer, size_t sampleOffset, size_t sampleCount);

private:
	// Limit creating SoundResources to the SoundEngine.
	friend class SoundEngine;

	static SoundResourceWAV *Load(const std::string &name);

	// Constructor for non-streaming.
	SoundResourceWAV(const std::string &name, const char *data, size_t size, const SoundResourceInfo &info);
	// Constructor for streaming.
	SoundResourceWAV(const std::string &name, size_t dataOffset, const SoundResourceInfo &info);

private:
	// Non-streaming specific members.
	ALuint m_buffer;

	// Streaming specific members.
	std::fstream m_file;
	size_t m_dataOffset;
};

#endif
