/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "Spring.h"

using namespace ::Oyster::Math3D;
using namespace ::Oyster::Physics3D;

Spring::Spring( Float r, Float s, Float d )
	: restingLength(r), springCoefficiant(s), dampeningCoefficiant(d) {}

Spring & Spring::operator = ( const Spring &spring )
{
	this->restingLength = spring.restingLength;
	this->springCoefficiant = spring.springCoefficiant;
	this->dampeningCoefficiant = spring.dampeningCoefficiant;
	return *this;
}

Float3 & Spring::GetSpringForce( const Float3 &deltaPosition, Float3 &targetMem ) const
{
	Float length = deltaPosition.Dot( deltaPosition );
	if( length != 0.0f )
	{
		length = sqrt( length );
		return targetMem = deltaPosition * ( this->springCoefficiant * (length - this->restingLength) / length );
	}
	else return targetMem = Float3::null;
}

Float3 & Spring::GetDampeningForce( const Float3 &deltaPosition, const Float3 &deltaLinearVelocity, Float3 &targetMem ) const
{
	Float projectedSpeed = deltaPosition.Dot(deltaPosition);
	if( projectedSpeed != 0.0f )
	{
		projectedSpeed = deltaLinearVelocity.Dot(deltaPosition) / projectedSpeed;
		return targetMem = deltaPosition * ( this->dampeningCoefficiant * projectedSpeed );
	}
	else return targetMem = Float3::null;
}

Float3 & Spring::GetSpringDampeningForce( const Float3 &deltaPosition, const Float3 &deltaLinearVelocity, Float3 &targetMem) const
{
	Float length = deltaPosition.Dot( deltaPosition );
	if( length != 0.0f )
	{ // f = sprCoeff * (|dP| - rL) + (dV dot dP) * (dampCoeff / |dP|) * (dP / |dP|)
		length = sqrt( length );
		Float force = this->springCoefficiant * (length - this->restingLength);
		force += deltaLinearVelocity.Dot(deltaPosition) * (this->dampeningCoefficiant / length);
		return targetMem = deltaPosition * (force / length);
	}
	else return targetMem = Float3::null;
}