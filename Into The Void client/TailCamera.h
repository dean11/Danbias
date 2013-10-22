#pragma once
#ifndef TAILCAMERA_H
#define TAILCAMERA_H

#include "SateliteCamera.h"
#include "OysterMath.h"

class TailCamera : public ::Oyster::Game::SateliteCamera
{
public:
	TailCamera( const ::Oyster::Math::Float4x4 &rigidBody, const ::Oyster::Math::Float3 &offsetPos, const ::Oyster::Math::Float3 &upVector, const ::Oyster::Math::Float4x4 &projection );

	const ::Oyster::Math::Float4x4 & getView( ) const;
	const ::Oyster::Math::Float4x4 & getViewProjection( ) const;

	void setMaxOffsetRadian( ::Oyster::Math::Float radian );
	void setMaxOffsetDistance( ::Oyster::Math::Float distance );

	void setRevolution( const ::Oyster::Math::Float &unitHorizontal, const ::Oyster::Math::Float &unitVertical );

private:
	::Oyster::Math::Float maxOffsetRadian;
	::Oyster::Math::Float maxOffsetDistance;
};

#endif