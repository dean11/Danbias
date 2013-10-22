#include <cstdint>
#include <cstdio>
#include <cassert>
#include <fstream>

#include "SoundResourceWAV.h"
#include "SoundEngine.h"
#include "Util.h"

#ifndef WAVE_FORMAT_PCM
#define WAVE_FORMAT_PCM 0x0001
#endif

struct ChunkHeader
{
	uint32_t fourcc;
	uint32_t size;
};

struct HeaderChunk
{
	ChunkHeader base;
	uint32_t format;
};

struct FormatChunk
{
	ChunkHeader base;
	uint16_t format;
	uint16_t channels;
	uint32_t sampleRate;
	uint32_t byteRate;
	uint16_t blockAlign;
	uint16_t bitDepth;
};

SoundResourceWAV *SoundResourceWAV::Load(const std::string &name)
{
	std::fstream file;
	file.open(name, std::ios_base::in | std::ios_base::binary);

	if (!file.is_open())
	{
		printf("Failed to open file %s\n", name.c_str());
		return NULL;
	}

	HeaderChunk header;
	file.read((char *)&header, sizeof(header));

	if (header.base.fourcc != FOURCC('R', 'I', 'F', 'F') || header.format != FOURCC('W', 'A', 'V', 'E'))
	{
		printf("%s is not a valid wav file!\n", name.c_str());
		file.close();
		return NULL;
	}

	FormatChunk format;
	file.read((char *)&format, sizeof(format));

	if (format.base.fourcc != FOURCC('f', 'm', 't', ' ') || format.base.size != 16 || format.format != WAVE_FORMAT_PCM)
	{
		printf("%s is not a valid LPCM wav file!\n", name.c_str());
		file.close();
		return NULL;
	}

	if (format.bitDepth != 8 && format.bitDepth != 16)
	{
		printf("%s has unsupported bitdepth!\n", name.c_str());
		file.close();
		return NULL;
	}

	ALenum bufferFormat;

	switch(format.channels)
	{
	case 1:
		bufferFormat = (format.bitDepth == 8? AL_FORMAT_MONO8 : AL_FORMAT_MONO16);
		break;
	case 2:
		bufferFormat = (format.bitDepth == 8? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16);
		break;
	default:
		printf("%s has unsupported number of channels!\n", name.c_str());
		file.close();
		return NULL;
	}

	ChunkHeader dataHeader;
	file.read((char *)&dataHeader, sizeof(dataHeader));

	while(dataHeader.fourcc != FOURCC('d', 'a', 't', 'a'))
	{
		file.seekg((uint32_t)file.tellg() + dataHeader.size);
		file.read((char *)&dataHeader, sizeof(dataHeader));

		if (file.eof())
		{
			printf("Failed to find data chunk in file %s!\n", name.c_str());
			file.close();
			return NULL;
		}
	}

	SoundResourceInfo info;
	info.format = bufferFormat;
	info.totalSamples = dataHeader.size / (format.bitDepth / 8);
	info.channels = format.channels;
	info.sampleRate = format.sampleRate;
	info.bitDepth = format.bitDepth;

	bool shouldStream = (dataHeader.size > SOUNDENGINE_STREAMING_SIZE_LIMIT? true : false);
	SoundResourceWAV *resource;

	if (shouldStream)
	{
		size_t dataOffset = (size_t)file.tellg();
		file.close();

		resource = new SoundResourceWAV(name, dataOffset, info);
	}
	else
	{
		char *data = new char[dataHeader.size];
		file.read(data, dataHeader.size);
		file.close();

		resource = new SoundResourceWAV(name, data, (size_t)dataHeader.size, info);
		
		delete[] data;
	}

	return resource;
}

SoundResourceWAV::SoundResourceWAV(const std::string &name, const char *data, size_t size, const SoundResourceInfo &info) :
	SoundResource(name, info, false)
{
	m_buffer = SoundEngine::GetBuffer();
	alBufferData(m_buffer, info.format, data, (ALsizei)size, info.sampleRate);
}

SoundResourceWAV::SoundResourceWAV(const std::string &name, size_t dataOffset, const SoundResourceInfo &info) :
	SoundResource(name, info, true)
{
	m_file.open(name, std::ios_base::in | std::ios_base::binary);
	m_dataOffset = dataOffset;

	assert(m_file.is_open());
}

SoundResourceWAV::~SoundResourceWAV()
{
	if (m_isStreaming)
		m_file.close();
	else
		SoundEngine::ReleaseBuffer(m_buffer);
}

ALuint SoundResourceWAV::GetBuffer()
{
	assert(!m_isStreaming);

	return m_buffer;
}

void SoundResourceWAV::FillBuffer(ALuint buffer, size_t sampleOffset, size_t sampleCount)
{
	assert(m_isStreaming);
	assert(m_file.is_open());
	assert(sampleOffset + sampleCount <= m_info.totalSamples);

	size_t offset = m_dataOffset + sampleOffset * (m_info.bitDepth / 8);
	size_t size = sampleCount * (m_info.bitDepth / 8);

	char *data = new char[size];
	m_file.seekg(offset);
	m_file.read(data, size);

	alBufferData(buffer, m_info.format, data, (ALsizei)size, m_info.sampleRate);

	delete[] data;
}
