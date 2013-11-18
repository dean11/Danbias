/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#ifndef OYSTER_PHYSICS_3D_SPRING_H
#define OYSTER_PHYSICS_3D_SPRING_H

#include "OysterMath.h"

namespace Oyster { namespace Physics3D
{
	struct Spring
	{	/// A struct of a spring able to generate push and pull forces
	public:
		::Oyster::Math::Float restingLength,		/// the resting length of the spring in meters
							  springCoefficiant,	/// kg/s^2
							  dampeningCoefficiant;

		Spring( ::Oyster::Math::Float restingLength = 1.0f, ::Oyster::Math::Float springCoefficiant = 1.0f, ::Oyster::Math::Float dampingCoefficiant = 0.0f );

		Spring & operator = ( const Spring &spring );

		::Oyster::Math::Float3 & GetSpringForce( const ::Oyster::Math::Float3 &deltaPosition, ::Oyster::Math::Float3 &targetMem = ::Oyster::Math::Float3() ) const;
		::Oyster::Math::Float3 & GetDampeningForce( const ::Oyster::Math::Float3 &deltaPosition, const ::Oyster::Math::Float3 &deltaLinearVelocity, ::Oyster::Math::Float3 &targetMem = ::Oyster::Math::Float3() ) const;
		::Oyster::Math::Float3 & GetSpringDampeningForce( const ::Oyster::Math::Float3 &deltaPosition, const ::Oyster::Math::Float3 &deltaLinearVelocity, ::Oyster::Math::Float3 &targetMem = ::Oyster::Math::Float3() ) const;
	};
} }

#endif