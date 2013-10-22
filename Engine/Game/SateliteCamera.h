/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/*
	Target use is by example tail camera, 3rd person perspective
	and such.
*/
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef OYSTER_GAME_SATELITECAMERA_H
#define OYSTER_GAME_SATELITECAMERA_H

#include "OysterMath.h"
#include "Frustrum.h"

namespace Oyster { namespace Game
{
	class SateliteCamera
	{
	public:
		SateliteCamera( const ::Oyster::Math::Float4x4 *rigidBody, const ::Oyster::Math::Float3 &offsetPos, const ::Oyster::Math::Float3 &upVector, const ::Oyster::Math::Float4x4 &projection );

		void setFocus( const ::Oyster::Math::Float4x4 *rigidBody, const ::Oyster::Math::Float3 &offsetPos, const ::Oyster::Math::Float3 &upVector );
		void setProjection( const ::Oyster::Math::Float4x4 &transform );

		const ::Oyster::Math::Float4x4 & getView( ) const;
		const ::Oyster::Math::Float4x4 & getProjection( ) const;
		const ::Oyster::Math::Float4x4 & getViewProjection( ) const;
		const ::Oyster::Collision::Frustrum & getSampler( ) const;

		void revolveLeft( const ::Oyster::Math::Float &deltaRadian );
		void revolveRight( const ::Oyster::Math::Float &deltaRadian );
		void revolveUp( const ::Oyster::Math::Float &deltaRadian );
		void revolveDown( const ::Oyster::Math::Float &deltaRadian );
		void revolveRollLeft( const ::Oyster::Math::Float &deltaRadian );
		void revolveRollRight( const ::Oyster::Math::Float &deltaRadian );
		void setHorizontalRevolution( const ::Oyster::Math::Float &radian );
		void setVerticalRevolution( const ::Oyster::Math::Float &radian );
		void setRollRevolution( const ::Oyster::Math::Float &radian );

		void moveIn( const ::Oyster::Math::Float &deltaLength );
		void moveOut( const ::Oyster::Math::Float &deltaLength );

		void panForward( const ::Oyster::Math::Float &deltaLength );
		void panBackward( const ::Oyster::Math::Float &deltaLength );
		void panLeft( const ::Oyster::Math::Float &deltaLength );
		void panRight( const ::Oyster::Math::Float &deltaLength );
		void panUp( const ::Oyster::Math::Float &deltaLength );
		void panDown( const ::Oyster::Math::Float &deltaLength );

		void stabiliseRoll( const ::Oyster::Math::Float &deltaRadian );
		void stabiliseHorizontally( const ::Oyster::Math::Float &deltaRadian );
		void stabiliseVertically( const ::Oyster::Math::Float &deltaRadian );
		void stabiliseDistance( const ::Oyster::Math::Float &deltaLength );
		void stabilisePanX( const ::Oyster::Math::Float &deltaLength );
		void stabilisePanY( const ::Oyster::Math::Float &deltaLength );
		void stabilisePanZ( const ::Oyster::Math::Float &deltaLength );

	protected:
		mutable ::Oyster::Math::Float3 offsetRadian;
		mutable ::Oyster::Math::Float3 offsetPan;
		mutable ::Oyster::Math::Float offsetDistance;
	private:
		const ::Oyster::Math::Float4x4 *focus;
		::Oyster::Math::Float4x4 defaultRelation, projection;
		mutable ::Oyster::Math::Float4x4 view, viewProjection;
		mutable ::Oyster::Collision::Frustrum sampler;
		mutable bool viewIsOutOfDate, viewProjectionIsOutOfDate, samplerIsOutOfDate;
	};
} }

#endif