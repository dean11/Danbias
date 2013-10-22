/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef GAME_MOVEABLE_H
#define GAME_MOVEABLE_H

#include "OysterMath.h"

namespace Game
{
	using namespace Oyster::Math;

	class MoveAble
	{
	public:
		MoveAble( );
		MoveAble( const Float4x4 &orientation, const Float3 &centerOfMass = Float3::null );
		virtual ~MoveAble( );

		void move( const Float3 &movement, const Float3 &displacement );
		void move( const Float3 &movement );
		void turn( const Float3 &deltaAngular );
		void stop( );

	protected:
		union
		{
			struct{ Float4x4 orientation; };
			struct{ Float3 axisX; Float paddingX;
					Float3 axisY; Float paddingY;
					Float3 axisZ; Float paddingZ;
					Float3 position; Float paddingP; };
		};
		Float3 centerOfMass;

		void updateOrientation( );

	private:
		Float3 sumDeltaAngular,
			   sumMovement;
	};
}

#endif