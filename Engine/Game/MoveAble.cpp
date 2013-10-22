/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "MoveAble.h"

using namespace ::Oyster::Game;
using namespace ::Oyster::Math;
using namespace ::Oyster::Collision;

namespace PrivateStatic
{
	Float timeSlice = 1.0f;
}

void MoveAble::setDiscreteTimeSlice( const Float &seconds )
{ PrivateStatic::timeSlice = seconds; }

const Float & MoveAble::getDiscreteTimeSlice( )
{ return PrivateStatic::timeSlice; }

MoveAble::MoveAble( ) : body(Float4x4::identity, Float3(1.0f)), rotationPivot(Float3::null), speed(Float3::null), rotationalSpeed(Float3::null), acceleration(Float3::null), rotationalAcceleration(Float3::null)
{
	this->speed = Float3::null;
}

MoveAble::MoveAble( const MoveAble &obj ) : body(obj.body), rotationPivot(obj.rotationPivot), speed(obj.speed), rotationalSpeed(obj.rotationalSpeed), acceleration(obj.acceleration), rotationalAcceleration(obj.rotationalAcceleration)
{
	this->speed = obj.speed;
}

MoveAble::MoveAble( const Float4x4 &orientation, const Float3 &_rotationPivot ) : body(orientation, Float3(1.0f)), rotationPivot(_rotationPivot), speed(Float3::null), rotationalSpeed(Float3::null), acceleration(Float3::null), rotationalAcceleration(Float3::null)
{
	this->speed = Float3::null;
}

MoveAble::MoveAble( const Box &_body, const Float3 &_rotationPivot ) : body(_body), rotationPivot(_rotationPivot), speed(Float3::null), rotationalSpeed(Float3::null), acceleration(Float3::null), rotationalAcceleration(Float3::null)
{
	this->speed = Float3::null;
}

MoveAble::~MoveAble( ) { /* Nothing needed here */ }

MoveAble & MoveAble::operator = ( const MoveAble &obj )
{
	this->body = obj.body;
	this->rotationPivot = obj.rotationPivot;
	this->speed = obj.speed;
	this->rotationalSpeed = obj.rotationalSpeed;
	this->acceleration = obj.acceleration;
	this->rotationalAcceleration = obj.rotationalAcceleration;
	return *this;
}

void MoveAble::accelerate( const Float3 &deltaSpeed, const Float3 &lever )
{
	this->acceleration += vectorProjection( deltaSpeed, lever );
	this->rotationalAcceleration += deltaAngularAxis( deltaSpeed, lever );
}

void MoveAble::accelerate( const Float3 &deltaSpeed )
{ this->acceleration += deltaSpeed; }

void MoveAble::accelerateTurn( const Float3 &deltaAngularSpeed )
{ this->rotationalAcceleration += deltaAngularSpeed; }

void MoveAble::stop( )
{ this->speed = this->rotationalSpeed = Float3::null; }

void MoveAble::stopRotation( )
{ this->rotationalSpeed = Float3::null; }

void MoveAble::stopMovement( )
{ this->speed = Float3::null; }

void MoveAble::setSize( const Float3 &size )
{ this->body.boundingOffset = size * 0.5f; }

Float3 MoveAble::getSize( ) const
{ return this->body.boundingOffset * 2.0f; }

void MoveAble::setOrientation( const Float4x4 & orientation )
{
//	if( (this->body.orientation.v[3].xyz - orientation.v[3].xyz).length() >= 500.0f )
//	{ // BUG HUNT illegal Teleportation trap
//		const char * debug = "Illegal Teleportation Occured?";
//	}

	this->body.orientation = orientation;
}

const Float4x4 & MoveAble::getOrientation( ) const
{
	return this->body.orientation; 
}

Float3 MoveAble::getMovement( const Float3 &offset ) const
{ return this->speed + particleRotationMovement( this->rotationalSpeed, offset ); }

const Float3 & MoveAble::getMovement( ) const
{ return this->speed; }

const Float3 & MoveAble::getRotation( ) const
{ return this->rotationalSpeed; }

void MoveAble::update( )
{
	this->speed += this->acceleration * PrivateStatic::timeSlice;
	this->rotationalSpeed += this->rotationalAcceleration * PrivateStatic::timeSlice;
	this->acceleration = this->rotationalAcceleration = Float3::null; // clearing acceleration every discrete timeslice (update)

	Float4x4 m;
	rigidBodyMatrix( m, this->rotationalSpeed * PrivateStatic::timeSlice,
					 ( this->speed * PrivateStatic::timeSlice ) + this->body.orientation.v[3].xyz,
					 this->rotationPivot );
	this->body.orientation.v[3].xyz = Float3::null;

	transformMatrix( this->body.orientation, this->body.orientation, m );
}