/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson & Robin Engman 2013
/////////////////////////////////////////////////////////////////////

#include "RigidBody.h"
#include "Utilities.h"

using namespace ::Oyster::Collision3D;
using namespace ::Oyster::Physics3D;
using namespace ::Oyster::Math3D;

RigidBody::RigidBody( const Box &b, Float m )
	: box(b), angularMomentum(0.0f), linearMomentum(0.0f), impulseTorqueSum(0.0f), impulseForceSum(0.0f)
{ // by Dan Andersson
	if( m != 0.0f )
	{
		this->mass = m;
	}
	else
	{
		this->mass = ::Utility::Value::numeric_limits<Float>::epsilon();
	}

	this->momentOfInertiaTensor = Float4x4::identity;
}

RigidBody & RigidBody::operator = ( const RigidBody &body )
{ // by Dan Andersson
	this->box = body.box;
	this->angularMomentum = body.angularMomentum;
	this->linearMomentum = body.linearMomentum;
	this->impulseTorqueSum = body.impulseTorqueSum;
	this->impulseForceSum = body.impulseForceSum;
	this->mass = body.mass;
	this->momentOfInertiaTensor = body.momentOfInertiaTensor;
	return *this;
}

void RigidBody::Update_LeapFrog( Float deltaTime )
{ // by Dan Andersson: Euler leap frog update when Runga Kutta is not needed
	
	// Important! The member data is all world data except the Inertia tensor. Thus a new InertiaTensor needs to be created to be compatible with the rest of the world data.
	Float4x4 wMomentOfInertiaTensor = TransformMatrix( this->box.rotation, this->momentOfInertiaTensor ); // RI

	// updating the linear
	// dG = F  * dt
	// ds = dt * Formula::LinearVelocity( m, avg_G ) = dt * avg_G / m = (dt / m) * avg_G
	Float3 linearImpulse =  this->impulseForceSum * deltaTime; // aka deltaLinearMomentum
	Float3 deltaPos = ( deltaTime / this->mass ) * ::Utility::Value::AverageWithDelta( this->linearMomentum, linearImpulse );

	// updating the angular
	// dH = T * dt
	// dO = dt * Formula::AngularVelocity( (RI)^-1, avg_H ) = dt * (RI)^-1 * avg_H
	Float3 angularImpulse = this->impulseTorqueSum * deltaTime; // aka deltaAngularMomentum
	Float3 rotationAxis = Formula::AngularVelocity( wMomentOfInertiaTensor.GetInverse(),
													::Utility::Value::AverageWithDelta(this->angularMomentum, angularImpulse) );
	
	Float deltaRadian = rotationAxis.Dot( rotationAxis );
	if( deltaRadian != 0.0f )
	{ // branch depending if there is rotation
		deltaRadian = ::std::sqrt( deltaRadian );
		rotationAxis /= deltaRadian;

		// using rotationAxis, deltaRadian and deltaPos to create a matrix to update the box
		this->box.center += deltaPos;
		TransformMatrix( RotationMatrix(deltaRadian, rotationAxis), this->box.rotation, this->box.rotation );
	}
	else
	{ // no rotation, only use deltaPos to translate the RigidBody
		this->box.center += deltaPos;
	}

<<<<<<< HEAD
	// update movements and clear impulseForceSum and impulseTorqueSum
=======
	// update momentums and clear impulseForceSum and impulseTorqueSum
>>>>>>> origin/RigidBody
	this->linearMomentum += linearImpulse;
	this->impulseForceSum = Float3::null;
	this->angularMomentum += angularImpulse;
	this->impulseTorqueSum = Float3::null;
}

void RigidBody::ApplyImpulseForce( const Float3 &worldF )
{ // by Dan Andersson
	this->impulseForceSum += worldF;
}

void RigidBody::ApplyImpulseForceAt( const Float3 &worldF, const Float3 &worldPos )
{ // by Dan Andersson
	Float3 worldOffset = worldPos - this->box.center;
	if( worldOffset != Float3::null )
	{
		this->impulseForceSum += VectorProjection( worldF, worldOffset );
		this->impulseTorqueSum += Formula::ImpulseTorque( worldF, worldOffset );
	}
	else
	{
		this->impulseForceSum += worldF;
	}
}

void RigidBody::ApplyLinearImpulseAcceleration( const Float3 &worldA )
{ // by Dan Andersson
	this->impulseForceSum += Formula::ImpulseForce( this->mass, worldA );
}

void RigidBody::ApplyLinearImpulseAccelerationAt( const Float3 &worldA, const Float3 &worldPos )
{ // by Dan Andersson
	Float3 worldOffset = worldPos - this->box.center;
	if( worldOffset != Float3::null )
	{
		this->impulseForceSum += Formula::ImpulseForce( this->mass, VectorProjection(worldA, worldOffset) );

		// tanAcc = angularAcc x localPosition
		// angularAcc = localPosition x tanAcc = localPosition x linearAcc
		// T = I * angularAcc
		this->impulseTorqueSum += Formula::ImpulseTorque( this->momentOfInertiaTensor, Formula::AngularImpulseAcceleration(worldA, worldOffset) );
	}
	else
	{
		this->impulseForceSum += Formula::ImpulseForce( this->mass, worldA );
	}
}

void RigidBody::ApplyImpulseTorque( const Float3 &worldT )
{ // by Dan Andersson
	this->impulseTorqueSum += worldT;
}

void RigidBody::ApplyAngularImpulseAcceleration( const Float3 &worldA )
{ // by Dan Andersson
	this->impulseTorqueSum += Formula::ImpulseTorque( this->momentOfInertiaTensor, worldA );
}

Float3 & RigidBody::AccessBoundingReach()
{ // by Dan Andersson
	return this->box.boundingOffset;
}

const Float3 & RigidBody::AccessBoundingReach() const
{ // by Dan Andersson
	return this->box.boundingOffset;
}

Float3 & RigidBody::AccessCenter()
{ // by Dan Andersson
	return this->box.center;
}

const Float3 & RigidBody::AccessCenter() const
{ // by Dan Andersson
	return this->box.center;
}

const Float4x4 & RigidBody::GetMomentOfInertia() const
{ // by Dan Andersson
	return this->momentOfInertiaTensor;
}

const Float & RigidBody::GetMass() const
{ // by Dan Andersson
	return this->mass;
}

const Float4x4 RigidBody::GetOrientation() const
{ // by Dan Andersson
	return OrientationMatrix( this->box.rotation, this->box.center );
}

Float4x4 RigidBody::GetView() const
{ // by Dan Andersson
	return InverseOrientationMatrix( this->GetOrientation() );
}

const Float3 & RigidBody::GetBoundingReach() const
{ // by Dan Andersson
	return this->box.boundingOffset;
}

Float3 RigidBody::GetSize() const
{ // by Dan Andersson
	return 2.0f * this->box.boundingOffset;
}

const Float3 & RigidBody::GetCenter() const
{ // by Dan Andersson
	return this->box.center;
}

const Float3 & RigidBody::GetImpulsTorque() const
{ // by Dan Andersson
	return this->impulseTorqueSum;
}

const Float3 & RigidBody::GetAngularMomentum() const
{ // by Dan Andersson
	return this->angularMomentum;
}

Float3 RigidBody::GetAngularImpulseAcceleration() const
{ // by Dan Andersson
	return Formula::AngularImpulseAcceleration( this->momentOfInertiaTensor.GetInverse(), this->impulseTorqueSum );
}

Float3 RigidBody::GetAngularVelocity() const
{ // by Dan Andersson
	return Formula::AngularVelocity( this->momentOfInertiaTensor.GetInverse(), this->angularMomentum );
}

const Float3 & RigidBody::GetImpulseForce() const
{ // by Dan Andersson
	return this->impulseForceSum;
}

const Float3 & RigidBody::GetLinearMomentum() const
{ // by Dan Andersson
	return this->linearMomentum;
}

Float3 RigidBody::GetLinearImpulseAcceleration() const
{ // by Dan Andersson
	return Formula::LinearImpulseAcceleration( this->mass, this->impulseForceSum );
}

Float3 RigidBody::GetLinearVelocity() const
{ // by Dan Andersson
	return Formula::LinearVelocity( this->mass, this->linearMomentum );
}

void RigidBody::GetMomentumAt( const Float3 &worldPos, const Float3 &surfaceNormal, Float3 &normalMomentum, Float3 &tangentialMomentum ) const
{
	Float3 worldOffset = worldPos - this->box.center;
	Float3 momentum = Formula::TangentialLinearMomentum( this->angularMomentum, worldOffset );
	momentum += this->linearMomentum;

	normalMomentum = NormalProjection( momentum, surfaceNormal );
	tangentialMomentum = momentum - normalMomentum;
}

void RigidBody::SetMomentOfInertia( const Float4x4 &localI )
{ // by Dan Andersson
	if( localI.GetDeterminant() != 0.0f ) // insanitycheck! momentOfInertiaTensor must be invertable
	{
		this->momentOfInertiaTensor = localI;
	}
}

void RigidBody::SetMass_KeepVelocity( const Float &m )
{ // by Dan Andersson
	if( m != 0.0f ) // insanitycheck! mass must be invertable
	{
		Float3 velocity = Formula::LinearVelocity( this->mass, this->linearMomentum );
		this->mass = m;
		this->linearMomentum = Formula::LinearMomentum( this->mass, velocity );
	}
}

void RigidBody::SetMass_KeepMomentum( const Float &m )
{ // by Dan Anderson
	if( m != 0.0f ) // insanitycheck! mass must be invertable
	{
		this->mass = m;
	}
}

void RigidBody::SetOrientation( const Float4x4 &o )
{ // by Dan Andersson
	 ExtractRotationMatrix( o, this->box.rotation );
	 this->box.center = o.v[3].xyz;
}

void RigidBody::SetSize( const Float3 &widthHeight )
{ // by Dan Andersson
	this->box.boundingOffset = 0.5f * widthHeight;
}

void RigidBody::SetCenter( const Float3 &worldPos )
{ // by Dan Andersson
	this->box.center = worldPos;
}

void RigidBody::SetImpulseTorque( const Float3 &worldT )
{ // by Dan Andersson
	this->impulseTorqueSum = worldT;
}

void RigidBody::SetAngularMomentum( const Float3 &worldH )
{ // by Dan Andersson
	this->angularMomentum = worldH;
}

void RigidBody::SetAngularImpulseAcceleration( const Float3 &worldA )
{ // by Dan Andersson
	this->impulseTorqueSum = Formula::ImpulseTorque( this->momentOfInertiaTensor, worldA );
}

void RigidBody::SetAngularVelocity( const Float3 &worldW )
{ // by Dan Andersson
	this->angularMomentum = Formula::AngularMomentum( this->momentOfInertiaTensor, worldW );
}

void RigidBody::SetImpulseForce( const Float3 &worldF )
{ // by Dan Andersson
	this->impulseForceSum = worldF;
}

void RigidBody::SetLinearMomentum( const Float3 &worldG )
{ // by Dan Andersson
	this->linearMomentum = worldG;
}

void RigidBody::SetLinearImpulseAcceleration( const Float3 &worldA )
{ // by Dan Andersson
	this->impulseForceSum = Formula::ImpulseForce( this->mass, worldA );
}

void RigidBody::SetLinearVelocity( const Float3 &worldV )
{ // by Dan Andersson
	this->linearMomentum = Formula::LinearMomentum( this->mass, worldV );
}

void RigidBody::SetImpulseForceAt( const Float3 &worldForce, const Float3 &worldPos )
{ // by Dan Andersson
	Float3 worldOffset = worldPos - this->box.center;
	this->impulseForceSum = VectorProjection( worldForce, worldOffset );
	this->impulseTorqueSum = Formula::ImpulseTorque( worldForce, worldOffset );
}

void RigidBody::SetLinearMomentumAt( const Float3 &worldG, const Float3 &worldPos )
{ // by Dan Andersson
	Float3 worldOffset = worldPos - this->box.center;
	this->linearMomentum = VectorProjection( worldG, worldOffset );
	this->angularMomentum = Formula::AngularMomentum( worldG, worldOffset );
}

void RigidBody::SetImpulseAccelerationAt( const Float3 &worldA, const Float3 &worldPos )
{ // by Dan Andersson
	Float3 worldOffset = worldPos - this->box.center;
	this->impulseForceSum = Formula::ImpulseForce( this->mass, VectorProjection(worldA, worldOffset) );
	this->impulseTorqueSum = Formula::ImpulseTorque( this->box.rotation * this->momentOfInertiaTensor,
													 Formula::AngularImpulseAcceleration(worldA, worldOffset) );
}

void RigidBody::SetLinearVelocityAt( const Float3 &worldV, const Float3 &worldPos )
{ // by Dan Andersson
	Float3 worldOffset = worldPos - this->box.center;
	this->linearMomentum = Formula::LinearMomentum( this->mass, VectorProjection(worldV, worldOffset) );
	this->angularMomentum = Formula::AngularMomentum( this->box.rotation * this->momentOfInertiaTensor,
													  Formula::AngularVelocity(worldV, worldOffset) );
}