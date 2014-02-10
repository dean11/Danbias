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
	this->quaternion = Quaternion(Float3(0, 0, 0), 1);
	this->boundingReach = Float4( 0.5f, 0.5f, 0.5f, 0.0f );
	this->momentum_Linear = Float4::null;
	this->momentum_Angular = Float4::null;
	this->impulse_Linear = Float4::null;
	this->impulse_Angular = Float4::null;
	this->restitutionCoeff = 1.0f;
	this->frictionCoeff_Static = 0.5f;
	this->frictionCoeff_Kinetic = 1.0f;
	this->mass = 10;
	this->momentOfInertiaTensor = MomentOfInertia();
}

RigidBody & RigidBody::operator = ( const RigidBody &body )
{ // by Dan Andersson
	this->centerPos = body.centerPos;
	this->quaternion = body.quaternion;
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
	return *this;
}

void RigidBody::Update_LeapFrog( Float updateFrameLength )
{ // by Dan Andersson: Euler leap frog update when Runga Kutta is not needed
	
	// updating the linear
	//Decrease momentum with 1% as "fall-off"
	//! HACK: @todo Add real solution with fluid drag
	this->momentum_Linear = this->momentum_Linear*0.99f;
	this->momentum_Angular = this->momentum_Angular*0.99f;

	// ds = dt * Formula::LinearVelocity( m, avg_G ) = dt * avg_G / m = (dt / m) * avg_G
	Float3 delta = this->momentum_Linear;
	Float3 newPos = (updateFrameLength)*this->momentum_Linear;
	this->centerPos += newPos;

	// updating the angular
	// dO = dt * Formula::AngularVelocity( (RI)^-1, avg_H ) = dt * (RI)^-1 * avg_H	
	/*this->axis += updateFrameLength*this->momentOfInertiaTensor.CalculateAngularVelocity( this->rotation, this->momentum_Angular );
	this->rotation = Rotation( this->axis );*/

	// update momentums and clear impulse_Linear and impulse_Angular
	this->momentum_Linear += this->impulse_Linear;
	this->impulse_Linear = Float4::null;

	this->momentum_Angular += this->impulse_Angular; //! HACK: @todo Rotation temporary disabled
	this->impulse_Angular = Float4::null;
}

void RigidBody::Predict_LeapFrog( Float3 &outDeltaPos, Float3 &outDeltaAxis, const Float3 &actingLinearImpulse, const Float3 &actingAngularImpulse, Float deltaTime )
{
	// updating the linear
	// ds = dt * Formula::LinearVelocity( m, avg_G ) = dt * avg_G / m = (dt / m) * avg_G
	outDeltaPos = ( deltaTime / this->mass ) * AverageWithDelta( this->momentum_Linear, actingLinearImpulse );

	// updating the angular
	//Float4x4 rotationMatrix; ::Oyster::Math3D::RotationMatrix( this->rotation, rotationMatrix );
	//Float4x4 wMomentOfInertiaTensor = TransformMatrix( rotationMatrix, this->momentOfInertiaTensor ); // RI

	// dO = dt * Formula::AngularVelocity( (RI)^-1, avg_H ) = dt * (RI)^-1 * avg_H
	//outDeltaAxis = Formula::AngularVelocity( wMomentOfInertiaTensor.GetInverse(), AverageWithDelta(this->momentum_Angular, actingAngularImpulse) );
	//utDeltaAxis = this->momentOfInertiaTensor.CalculateAngularVelocity( this->rotation, AverageWithDelta(this->momentum_Angular, this->impulse_Angular) );
}

void RigidBody::Move( const Float3 &deltaPos, const Float3 &deltaAxis )
{
	//this->centerPos += deltaPos;
	//this->axis += deltaAxis;
	//this->rotation = Rotation( this->axis );
}

void RigidBody::ApplyImpulse( const Float3 &worldJ, const Float3 &atWorldPos )
{ // by Dan Andersson
	Float3 worldOffset = atWorldPos - this->centerPos;
	if( worldOffset != Float3::null )
	{
		this->impulse_Linear += VectorProjection( worldJ, atWorldPos );
		this->impulse_Angular += Formula::ImpulseTorque( worldJ, atWorldPos );
	}
	else
	{
		this->impulse_Linear += worldJ;
	}
}

const MomentOfInertia & RigidBody::GetMomentOfInertia() const
{ // by Dan Andersson
	return this->momentOfInertiaTensor;
}

Float RigidBody::GetMass() const
{ // by Dan Andersson
	return this->mass;
}

const Quaternion & RigidBody::GetRotationQuaternion() const
{ // by Dan Andersson
	return this->quaternion;
}

Float4x4 RigidBody::GetRotationMatrix() const
{ // by Dan Andersson
	return RotationMatrix( quaternion );
}

Float4x4 RigidBody::GetOrientation() const
{ // by Dan Andersson
	return ::Oyster::Math3D::OrientationMatrix( this->quaternion, this->centerPos );
}

Float4x4 RigidBody::GetView() const
{ // by Dan Andersson
	return ViewMatrix( this->quaternion, this->centerPos );
}

Float3 RigidBody::GetVelocity_Linear() const
{ // by Dan Andersson
	return Formula::LinearVelocity( this->mass, this->momentum_Linear );
}

Float3 RigidBody::GetVelocity_Angular() const
{ // by Dan Andersson
	return Float3(0, 0, 0);
}

Float3 RigidBody::GetLinearMomentum( const Float3 &atWorldPos ) const
{ // by Dan Andersson
	Float3 offset = atWorldPos - this->centerPos;
	if( offset.Dot(offset) > 0.0f )
	{
		return this->momentum_Linear + Formula::TangentialLinearMomentum( this->momentum_Angular, offset );
	}
	return this->momentum_Linear;
}

void RigidBody::SetMomentOfInertia_KeepVelocity( const MomentOfInertia &localTensorI )
{ // by Dan Andersson
	
}

void RigidBody::SetMomentOfInertia_KeepMomentum( const MomentOfInertia &localTensorI )
{ // by Dan Andersson
	this->momentOfInertiaTensor = localTensorI;
}

void RigidBody::SetMass_KeepVelocity( const Float &m )
{ // by Dan Andersson
	if( m != 0.0f )
	{ // insanity check! Mass must be invertable
		Float3 v = Formula::LinearVelocity( this->mass, this->momentum_Linear );
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

void RigidBody::SetRotation( const ::Oyster::Math::Quaternion &quaternion )
{ // by Dan Andersson
	this->quaternion = quaternion;
}

void RigidBody::SetMomentum_Linear( const Float3 &worldG, const Float3 &atWorldPos )
{ // by Dan Andersson
	Float3 worldOffset = atWorldPos - this->centerPos;
	this->momentum_Linear = VectorProjection( worldG, worldOffset );
	this->momentum_Angular = Formula::AngularMomentum( worldG, worldOffset );
}

void RigidBody::SetVelocity_Linear( const Float3 &worldV )
{ // by Dan Andersson
	this->momentum_Linear = Formula::LinearMomentum( this->mass, worldV );
}

void RigidBody::SetVelocity_Linear( const Float3 &worldV, const Float3 &atWorldPos )
{ // by Dan Andersson
	Float3 worldOffset = atWorldPos - this->centerPos;
	this->momentum_Linear  = Formula::LinearMomentum( this->mass, VectorProjection(worldV, worldOffset) );
	this->momentum_Angular = this->momentOfInertiaTensor.CalculateAngularMomentum( this->quaternion, Formula::AngularVelocity(worldV, worldOffset) );
}

void RigidBody::SetVelocity_Angular( const Float3 &worldW )
{ // by Dan Andersson
	this->momentum_Angular = this->momentOfInertiaTensor.CalculateAngularMomentum( this->quaternion, worldW );
}

void RigidBody::SetImpulse_Linear( const Float3 &worldJ, const Float3 &atWorldPos )
{ // by Dan Andersson
	Float3 worldOffset = atWorldPos - this->centerPos;
	this->impulse_Linear = VectorProjection( worldJ, worldOffset );
	this->impulse_Angular = Formula::ImpulseTorque( worldJ, worldOffset );
}