// Wrapper class for the soundsystem

#pragma once
#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H

#include "SoundEngine.h"
#include "SoundSource.h"
#include "Listener.h"

#include "OysterMath.h"

class SoundSystem
{
public:
	enum Result { Success, Failure };

	static Result init( )
	{
		if (!SoundEngine::Init())
			return Failure;

		return Success;
	}

	static void release( )
	{
		SoundEngine::Release();
	}

	static void update( )
	{
		SoundEngine::Update();
	}

	static void setTransformation(const Oyster::Math::Float4x4 *transformation)
	{
		const float *position = transformation->v[3].xyz.element;
		const float *forward = (-transformation->v[2]).xyz.element;
		const float *up = transformation->v[1].xyz.element;

		Listener::SetPosition(position);
		Listener::SetOrientation(forward, up);
	}
};

#endif
