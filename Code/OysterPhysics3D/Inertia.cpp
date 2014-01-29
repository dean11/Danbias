/********************************************************************
 * Created by Dan Andersson 2014
 ********************************************************************/

#include "Inertia.h"

using namespace ::Oyster::Math3D;
using namespace ::Oyster::Physics3D;

MomentOfInertia::MomentOfInertia()
{
	this->rotation = Quaternion::identity;
	this->magnitude = Float3( 1.0f );
}

MomentOfInertia::MomentOfInertia( const Quaternion &r, const Float3 &m )
{
	this->rotation = r;
	this->magnitude = m;
}

MomentOfInertia & MomentOfInertia::operator = ( const MomentOfInertia &i )
{
	this->rotation = i.rotation;
	this->magnitude = i.magnitude;
	return *this;
}

Float3 MomentOfInertia::CalculateAngularVelocity( const Quaternion &externR, const Float3 &h ) const
{
	return this->CalculateAngularVelocity( externR, h, Float3() );
}

Float3 & MomentOfInertia::CalculateAngularVelocity( const Quaternion &externR, const Float3 &h, Float3 &targetMem ) const
{ // w = h * | (2/3) * I_M^-1 (R I_R)^-1 h | / |h|
	Float hMagnitudeSquared = h.Dot( h );
	if( hMagnitudeSquared > 0.0f )
	{
		Float4x4 rotationInverse = (RotationMatrix( externR ) * RotationMatrix( this->rotation )).Transpose();
		Float4 v = rotationInverse * Float4( h, 0.0f );
		v.PiecewiseMultiplicationAdd( Float4((2.0f/3.0f) / this->magnitude.x, (2.0f/3.0f) / this->magnitude.y, (2.0f/3.0f) / this->magnitude.z, 0.0f) );
		return targetMem = (Float4( h, 0.0f ) * ( v.GetMagnitude() / ( (Float)::std::sqrt(hMagnitudeSquared)) ) ).xyz;
	}
	else
		return targetMem = Float3::null;
}

Float3 MomentOfInertia::CalculateAngularMomentum( const Quaternion &externR, const Float3 &w ) const
{
	return this->CalculateAngularMomentum( externR, w, Float3() );
}

Float3 & MomentOfInertia::CalculateAngularMomentum( const Quaternion &externR, const Float3 &w, Float3 &targetMem ) const
{ // h = w * | (3/2) * I_M (R I_R)^-1 w | / |w|
	Float wMagnitudeSquared = w.Dot( w );
	if( wMagnitudeSquared > 0.0f )
	{
		Float4x4 rotationInverse = (RotationMatrix( externR ) * RotationMatrix( this->rotation )).Transpose();
		Float4 v = rotationInverse * Float4( w, 0.0f );
		v.PiecewiseMultiplicationAdd( Float4((3.0f/2.0f) * this->magnitude.x, (3.0f/2.0f) * this->magnitude.y, (3.0f/2.0f) * this->magnitude.z, 0.0f) );
		return targetMem = (Float4( w, 0.0f ) * ( v.GetMagnitude() / (Float)::std::sqrt(wMagnitudeSquared) ) ).xyz;
	}
	else
		return targetMem = Float3::null;
}