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
{ // w = (R * I_R) * I_M^-1 * (R * I_R)^-1 * h
	Float4x4 rotation = RotationMatrix( externR ) * RotationMatrix( this->rotation );
	Float4 w = rotation.GetInverse() * Float4( h, 0.0f );
	return targetMem = rotation * w.PiecewiseMultiplicationAdd( Float4(1.0f / this->magnitude.x, 1.0f / this->magnitude.y, 1.0f / this->magnitude.z, 0.0f) );
}

Float3 MomentOfInertia::CalculateAngularMomentum( const Quaternion &externR, const Float3 &w ) const
{
	return this->CalculateAngularMomentum( externR, w, Float3() );
}

Float3 & MomentOfInertia::CalculateAngularMomentum( const Quaternion &externR, const Float3 &w, Float3 &targetMem ) const
{ // h = (R * I_R) * I_M * (R * I_R)^-1 * w
	Float4x4 rotation = RotationMatrix( externR ) * RotationMatrix( this->rotation );
	Float4 h = rotation.GetInverse() * Float4( w, 0.0f );
	return targetMem = rotation * h.PiecewiseMultiplicationAdd( Float4(this->magnitude.x, this->magnitude.y, this->magnitude.z, 0.0f) );
}