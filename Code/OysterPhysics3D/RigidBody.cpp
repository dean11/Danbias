/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson & Robin Engman 2013
/////////////////////////////////////////////////////////////////////

#include "RigidBody.h"
#include "Utilities.h"

using namespace ::Oyster::Collision3D;
using namespace ::Oyster::Physics3D;
using namespace ::Oyster::Math3D;
using namespace ::Utility::Value;

RigidBody::RigidBody( )
{ // by Dan Andersson
	this->centerPos = Float4::standard_unit_w;
	this->axis = Float4::null;
	this->boundingReach = Float4( 0.5f, 0.5f, 0.5f, 0.0f );
	this->momentum_Linear = Float4::null;
	this->momentum_Angular = Float4::null;
	this->impulse_Linear = Float4::null;
	this->impulse_Angular = Float4::null;
	this->restitutionCoeff = 1.0f;
	this->frictionCoeff_Static = 0.5f;
	this->frictionCoeff_Kinetic = 1.0f;
	this->mass = 10;
	this->momentOfInertiaTensor = Float4x4::identity;
	this->rotation = Quaternion::identity;
}

RigidBody & RigidBody::operator = ( const RigidBody &body )
{ // by Dan Andersson
	this->centerPos = body.centerPos;
	this->axis = body.axis;
	this->boundingReach = body.boundingReach;
	this->momentum_Linear = body.momentum_Linear;
	this->momentum_Angular = body.momentum_Angular;
	this->impulse_Linear = body.impulse_Linear;
	this->impulse_Angular = body.impulse_Angular;
	this->restitutionCoeff = body.restitutionCoeff;
	this->frictionCoeff_Static = body.frictionCoeff_Static;
	this->frictionCoeff_Kinetic = body.frictionCoeff_Kinetic;
	this->mass = body.mass;
	this->momentOfInertiaTensor = body.momentOfInertiaTensor;
	this->rotation = body.rotation;
	return *this;
}

void RigidBody::Update_LeapFrog( Float updateFrameLength )
{ // by Dan Andersson: Euler leap frog update when Runga Kutta is not needed
	
	// updating the linear
	// ds = dt * Formula::LinearVelocity( m, avg_G ) = dt * avg_G / m = (dt / m) * avg_G
	this->centerPos += ( updateFrameLength / this->mass ) * AverageWithDelta( this->momentum_Linear, this->impulse_Linear );

	// updating the angular
	Float4x4 rotationMatrix; ::Oyster::Math3D::RotationMatrix( this->rotation, rotationMatrix );
	// Important! The member data is all world data except the Inertia tensor. Thus a new InertiaTensor needs to be created to be compatible with the rest of the world data.
	Float4x4 wMomentOfInertiaTensor = TransformMatrix( rotationMatrix, this->momentOfInertiaTensor ); // RI

	// dO = dt * Formula::AngularVelocity( (RI)^-1, avg_H ) = dt * (RI)^-1 * avg_H
	this->axis += Radian( Formula::AngularVelocity(wMomentOfInertiaTensor.GetInverse(), AverageWithDelta(this->momentum_Angular, this->impulse_Angular)) );
	this->rotation = Rotation( this->axis );

	// update momentums and clear impulse_Linear and impulse_Angular
	this->momentum_Linear += this->impulse_Linear;
	this->impulse_Linear = Float4::null;
	this->momentum_Angular += this->impulse_Angular;
	this->impulse_Angular = Float4::null;
}

void RigidBody::Predict_LeapFrog( Float4 &outDeltaPos, Float4 &outDeltaAxis, const Float4 &actingLinearImpulse, const Float4 &actingAngularImpulse, Float deltaTime )
{
	// updating the linear
	// ds = dt * Formula::LinearVelocity( m, avg_G ) = dt * avg_G / m = (dt / m) * avg_G
	outDeltaPos = ( deltaTime / this->mass ) * AverageWithDelta( this->momentum_Linear, actingLinearImpulse );

	// updating the angular
	Float4x4 rotationMatrix; ::Oyster::Math3D::RotationMatrix( this->rotation, rotationMatrix );
	Float4x4 wMomentOfInertiaTensor = TransformMatrix( rotationMatrix, this->momentOfInertiaTensor ); // RI

	// dO = dt * Formula::AngularVelocity( (RI)^-1, avg_H ) = dt * (RI)^-1 * avg_H
	outDeltaAxis = Formula::AngularVelocity( wMomentOfInertiaTensor.GetInverse(), AverageWithDelta(this->momentum_Angular, actingAngularImpulse) );
}

void RigidBody::Move( const Float4 &deltaPos, const Float4 &deltaAxis )
{
	this->centerPos += deltaPos;
	this->axis += deltaAxis;
	this->rotation = Rotation( this->axis );
}

void RigidBody::ApplyImpulse( const Float4 &worldJ, const Float4 &atWorldPos )
{ // by Dan Andersson
	Float4 worldOffset = atWorldPos - this->centerPos;
	if( worldOffset != Float4::null )
	{
		this->impulse_Linear += VectorProjection( worldJ, atWorldPos );
		this->impulse_Angular += Formula::ImpulseTorque( worldJ, atWorldPos );
	}
	else
	{
		this->impulse_Linear += worldJ;
	}
}

const Float4x4 & RigidBody::GetMomentOfInertia() const
{ // by Dan Andersson
	return this->momentOfInertiaTensor;
}

Float RigidBody::GetMass() const
{ // by Dan Andersson
	return this->mass;
}

const Quaternion & RigidBody::GetRotation() const
{ // by Dan Andersson
	return this->rotation;
}

Float4x4 RigidBody::GetRotationMatrix() const
{ // by Dan Andersson
	return RotationMatrix( this->rotation );
}

Float4x4 RigidBody::GetOrientation() const
{ // by Dan Andersson
	return ::Oyster::Math3D::OrientationMatrix( this->rotation, this->centerPos );
}

Float4x4 RigidBody::GetView() const
{ // by Dan Andersson
	return ViewMatrix( this->rotation, this->centerPos );
}

Float4 RigidBody::GetVelocity_Linear() const
{ // by Dan Andersson
	return Formula::LinearVelocity( this->mass, this->momentum_Linear );
}

Float4 RigidBody::GetVelocity_Angular() const
{ // by Dan Andersson
	return Formula::AngularVelocity( this->momentOfInertiaTensor.GetInverse(), this->momentum_Angular );
}

Float4 RigidBody::GetLinearMomentum( const Float4 &atWorldPos ) const
{ // by Dan Andersson
	return this->momentum_Linear + Formula::TangentialLinearMomentum( this->momentum_Angular, atWorldPos - this->centerPos );
}

void RigidBody::SetMomentOfInertia_KeepVelocity( const Float4x4 &localTensorI )
{ // by Dan Andersson
	if( localTensorI.GetDeterminant() != 0.0f )
	{ // insanity check! MomentOfInertiaTensor must be invertable
		Float4x4 rotationMatrix; RotationMatrix( this->rotation, rotationMatrix );

		Float4 w = Formula::AngularVelocity( (rotationMatrix * this->momentOfInertiaTensor).GetInverse(), this->momentum_Angular );
		this->momentOfInertiaTensor = localTensorI;
		this->momentum_Angular = Formula::AngularMomentum( rotationMatrix * localTensorI, w );
	}
}

void RigidBody::SetMomentOfInertia_KeepMomentum( const Float4x4 &localTensorI )
{ // by Dan Andersson
	if( localTensorI.GetDeterminant() != 0.0f )
	{ // insanity check! MomentOfInertiaTensor must be invertable
		this->momentOfInertiaTensor = localTensorI;
	}
}

void RigidBody::SetMass_KeepVelocity( const Float &m )
{ // by Dan Andersson
	if( m != 0.0f )
	{ // insanity check! Mass must be invertable
		Float4 v = Formula::LinearVelocity( this->mass, this->momentum_Linear );
		this->mass = m;
		this->momentum_Linear = Formula::LinearMomentum( this->mass, v );
	}
}

void RigidBody::SetMass_KeepMomentum( const Float &m )
{ // by Dan Anderson
	if( m != 0.0f )
	{ // insanity check! Mass must be invertable
		this->mass = m;
	}
}

void RigidBody::SetOrientation( const Float4x4 &o )
{ // by Dan Andersson
	this->axis = ExtractAngularAxis( o );
	this->rotation = Rotation( this->axis );
	this->centerPos = o.v[3].xyz;
}

void RigidBody::SetRotation( const Float4x4 &r )
{ // by Dan Andersson
	this->axis = ExtractAngularAxis( r );
	this->rotation = Rotation( this->axis );
}

void RigidBody::SetMomentum_Linear( const Float4 &worldG, const Float4 &atWorldPos )
{ // by Dan Andersson
	Float4 worldOffset = atWorldPos - this->centerPos;
	this->momentum_Linear = VectorProjection( worldG, worldOffset );
	this->momentum_Angular = Formula::AngularMomentum( worldG, worldOffset );
}

void RigidBody::SetVelocity_Linear( const Float4 &worldV )
{ // by Dan Andersson
	this->momentum_Linear = Formula::LinearMomentum( this->mass, worldV );
}

void RigidBody::SetVelocity_Linear( const Float4 &worldV, const Float4 &atWorldPos )
{ // by Dan Andersson
	Float4 worldOffset = atWorldPos - this->centerPos;
	this->momentum_Linear = Formula::LinearMomentum( this->mass, VectorProjection(worldV, worldOffset) );
	this->momentum_Angular = Formula::AngularMomentum( RotationMatrix(this->rotation) * this->momentOfInertiaTensor, Formula::AngularVelocity(worldV, worldOffset) );
}

void RigidBody::SetVelocity_Angular( const Float4 &worldW )
{ // by Dan Andersson
	this->momentum_Angular = Formula::AngularMomentum( this->momentOfInertiaTensor, worldW );
}

void RigidBody::SetImpulse_Linear( const Float4 &worldJ, const Float4 &atWorldPos )
{ // by Dan Andersson
	Float4 worldOffset = atWorldPos - this->centerPos;
	this->impulse_Linear = VectorProjection( worldJ, worldOffset );
	this->impulse_Angular = Formula::ImpulseTorque( worldJ, worldOffset );
}