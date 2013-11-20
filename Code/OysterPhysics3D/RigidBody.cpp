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
	Float4x4 wMomentOfInertiaTensor = TransformMatrix( this->box.rotation, this->momentOfInertiaTensor );

	// updating the linear
	// dv = dt * a = dt * F / m
	// ds = dt * avg_v
	Float3 deltaLinearVelocity = this->impulseForceSum;
	deltaLinearVelocity *= (deltaTime / this->mass);
	Float3 deltaPos = deltaTime * ::Utility::Value::AverageWithDelta( Formula::LinearVelocity(this->mass, this->linearMomentum), deltaLinearVelocity );

	// updating the angular
	// dw = dt * a = dt * ( I^-1 * T )
	// rotation = dt * avg_w
	Float4x4 inversedMomentOfInertiaTensor = wMomentOfInertiaTensor.GetInverse();
	Float3 deltaAngularVelocity = Formula::AngularImpulseAcceleration( inversedMomentOfInertiaTensor, this->impulseTorqueSum ); // I^-1 * T
	deltaAngularVelocity *= deltaTime; 
	Float3 rotationAxis = ::Utility::Value::AverageWithDelta( Formula::AngularVelocity(inversedMomentOfInertiaTensor,this->angularMomentum), deltaAngularVelocity );
	
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

	// update movements and clear impulses
	this->linearMomentum += Formula::LinearMomentum( this->mass, deltaLinearVelocity );
	this->impulseForceSum = Float3::null;
	this->angularMomentum += Formula::AngularMomentum( wMomentOfInertiaTensor, deltaAngularVelocity );
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

Float4x4 & RigidBody::AccessOrientation()
{ // by Dan Andersson
	return this->box.orientation;
}

const Float4x4 & RigidBody::AccessOrientation() const
{ // by Dan Andersson
	return this->box.orientation;
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

const Float4x4 & RigidBody::GetOrientation() const
{ // by Dan Andersson
	return this->box.orientation;
}

Float4x4 RigidBody::GetView() const
{ // by Dan Andersson
	return InverseOrientationMatrix( this->box.orientation );
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

Float3 RigidBody::GetTangentialImpulseForceAt( const Float3 &worldPos ) const
{ // by Dan Andersson
	Float3 worldOffset = worldPos - this->box.center;
	return Formula::TangentialImpulseForce( this->impulseTorqueSum, worldOffset );
}

Float3 RigidBody::GetTangentialLinearMomentumAt( const Float3 &worldPos ) const
{ // by Dan Andersson
	return Formula::TangentialLinearMomentum( this->angularMomentum, worldPos );
}

Float3 RigidBody::GetTangentialImpulseAccelerationAt( const Float3 &worldPos ) const
{ // by Dan Andersson
	return this->GetTangentialImpulseAccelerationAt_Local( (this->GetView() * Float4(worldPos, 1.0f)).xyz ); // should not be any disform thus result.w = 1.0f
}

Float3 RigidBody::GetTangentialLinearVelocityAt( const Float3 &worldPos ) const
{ // by Dan Andersson
	return this->GetTangentialLinearVelocityAt_Local( (this->GetView() * Float4(worldPos, 1.0f)).xyz ); // should not be any disform thus result.w = 1.0f
}

Float3 RigidBody::GetImpulseForceAt( const Float3 &worldPos ) const
{ // by Dan Andersson
	Float4 localForce = Float4( this->GetImpulseForceAt_Local((this->GetView() * Float4(worldPos, 1.0f)).xyz), 0.0f ); // should not be any disform thus result.w = 1.0f
	return (this->box.orientation * localForce).xyz; // should not be any disform thus result.w = 0.0f
}

Float3 RigidBody::GetLinearMomentumAt( const Float3 &worldPos ) const
{ // by Dan Andersson
	// Reminder! Momentum is a world value.
	Float4 localMomentum = Float4( this->GetLinearMomentumAt_Local((this->GetView() * Float4(worldPos, 1.0f)).xyz), 0.0f ); // should not be any disform thus result.w = 1.0f
	return (this->box.orientation * localMomentum).xyz; // should not be any disform thus result.w = 0.0f

	// TODO: angularMomentum is a local value!!
	return this->linearMomentum + Formula::TangentialLinearMomentum( this->angularMomentum, worldPos );
}

Float3 RigidBody::GetImpulseAccelerationAt( const Float3 &worldPos ) const
{ // by Dan Andersson
	// Reminder! Acceleration is a world value.
	return Formula::LinearImpulseAcceleration( this->mass, this->impulseForceSum )
		 + Formula::TangentialImpulseAcceleration( this->momentOfInertiaTensor.GetInverse(), this->impulseTorqueSum, worldPos );
}

Float3 RigidBody::GetLinearVelocityAt( const Float3 &worldPos ) const
{ // by Dan Andersson
	// Reminder! Velocity is a world value.
	return Formula::LinearVelocity( this->mass, this->linearMomentum )
		 + Formula::TangentialLinearVelocity( this->momentOfInertiaTensor.GetInverse(), this->angularMomentum, worldPos );
}

void RigidBody::SetMomentOfInertia( const Float4x4 &localI )
{ // by Dan Andersson
	if( i.GetDeterminant() != 0.0f ) // insanitycheck! momentOfInertiaTensor must be invertable
	{
		this->momentOfInertiaTensor = i;
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
	this->box.orientation = o;
}

void RigidBody::SetSize( const Float3 &widthHeight )
{ // by Dan Andersson
	this->box.boundingOffset = 0.5f * widthHeight;
}

void RigidBody::SetCenter( const Float3 &worldPos )
{ // by Dan Andersson
	this->box.center = p;
}

void RigidBody::SetImpulseTorque( const Float3 &worldT )
{ // by Dan Andersson
	this->impulseTorqueSum = t;
}

void RigidBody::SetAngularMomentum( const Float3 &worldH )
{ // by Dan Andersson
	this->angularMomentum = h;
}

void RigidBody::SetAngularImpulseAcceleration( const Float3 &worldA )
{ // by Dan Andersson
	this->impulseTorqueSum = Formula::ImpulseTorque( this->momentOfInertiaTensor, a );
}

void RigidBody::SetAngularVelocity( const Float3 &worldW )
{ // by Dan Andersson
	this->angularMomentum = Formula::AngularMomentum( this->momentOfInertiaTensor, w );
}

void RigidBody::SetImpulseForce( const Float3 &worldF )
{ // by Dan Andersson
	this->impulseForceSum = f;
}

void RigidBody::SetLinearMomentum( const Float3 &worldG )
{ // by Dan Andersson
	this->linearMomentum = g;
}

void RigidBody::SetLinearImpulseAcceleration( const Float3 &worldA )
{ // by Dan Andersson
	this->impulseForceSum = Formula::ImpulseForce( this->mass, a );
}

void RigidBody::SetLinearVelocity( const Float3 &worldV )
{ // by Dan Andersson
	this->linearMomentum = Formula::LinearMomentum( this->mass, v );
}

void RigidBody::SetImpulseForceAt( const Float3 &worldF, const Float3 &worldPos )
{ // by Dan Andersson
	// Reminder! Impulse force and torque is world values.
	this->impulseForceSum = VectorProjection( worldForce, worldPos );
	this->impulseTorqueSum = Formula::ImpulseTorque( worldForce, worldPos );
}

void RigidBody::SetLinearMomentumAt( const Float3 &worldG, const Float3 &worldPos )
{ // by Dan Andersson
	// Reminder! Linear and angular momentum is world values.
	this->linearMomentum = VectorProjection( worldG, worldPos );
	this->angularMomentum = Formula::AngularMomentum( worldG, worldPos );
}

void RigidBody::SetImpulseAccelerationAt( const Float3 &worldA, const Float3 &pos )
{ // by 

}

void RigidBody::SetLinearVelocityAt( const Float3 &worldV, const Float3 &pos )
{ // by 

}