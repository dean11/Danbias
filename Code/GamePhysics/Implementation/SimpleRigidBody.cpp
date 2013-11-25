#include "SimpleRigidBody.h"
#include "PhysicsAPI_Impl.h"

using namespace ::Oyster::Physics;
using namespace ::Oyster::Math3D;
using namespace ::Oyster::Collision3D;
using namespace ::Utility::DynamicMemory;
using namespace ::Utility::Value;

SimpleRigidBody::SimpleRigidBody() : previous(), current() {}

SimpleRigidBody::~SimpleRigidBody() {}

UniquePointer<ICustomBody> SimpleRigidBody::Clone() const	
{
	return new SimpleRigidBody( *this );
}

bool SimpleRigidBody::IsSubscribingCollisions() const
{ // Assumption
	return true;
}

bool SimpleRigidBody::Intersects( const ICustomBody &object, Float timeStepLength, Float &deltaWhen, Float3 &worldPointOfContact ) const
{
	if( object.Intersects(this->current.box) )
	{ //! @todo TODO: better implementation needed
		deltaWhen = timeStepLength;
		worldPointOfContact = Average( this->current.box.center, object.GetCenter() );
		return true;
	}
	else
	{
		return false;
	}
}

bool SimpleRigidBody::Intersects( const ICollideable &shape ) const
{
	return this->current.box.Intersects( shape );
}

Sphere & SimpleRigidBody::GetBoundingSphere( Sphere &targetMem ) const
{
	return targetMem = Sphere( this->current.box.center, this->current.box.boundingOffset.GetMagnitude() );
}

Float3 & SimpleRigidBody::GetNormalAt( const Float3 &worldPos, Float3 &targetMem ) const
{
	//! @todo TODO: better implementation needed
	return targetMem = (worldPos - this->current.box.center).GetNormalized();
}

Float3 & SimpleRigidBody::GetCenter( Float3 &targetMem ) const
{
	return targetMem = this->current.box.center;
}

Float4x4 & SimpleRigidBody::GetRotation( Float4x4 &targetMem ) const
{
	return targetMem = this->current.box.rotation;
}

Float4x4 & SimpleRigidBody::GetOrientation( Float4x4 &targetMem ) const
{
	return targetMem = this->current.GetOrientation();
}

Float4x4 & SimpleRigidBody::GetView( Float4x4 &targetMem ) const
{
	return targetMem = this->current.GetView();
}

UpdateState SimpleRigidBody::Update( Float timeStepLength )
{
	this->previous = this->current; // memorizing the old state

	this->current.Update_LeapFrog( timeStepLength );

	// compare previous and new state and return result
	return this->current == this->previous ? resting : altered;
}

void SimpleRigidBody::SetMomentOfInertiaTensor_KeepVelocity( const Float4x4 &localI )
{
	this->current.SetMomentOfInertia_KeepVelocity( localI );
}

void SimpleRigidBody::SetMomentOfInertiaTensor_KeepMomentum( const Float4x4 &localI )
{
	this->current.SetMomentOfInertia_KeepMomentum( localI );
}

void SimpleRigidBody::SetMass_KeepVelocity( Float m )
{
	this->current.SetMass_KeepVelocity( m );
}

void SimpleRigidBody::SetMass_KeepMomentum( Float m )
{
	this->current.SetMass_KeepMomentum( m );
}

void SimpleRigidBody::SetCenter( const Float3 &worldPos )
{
	this->current.SetCenter( worldPos );
}

void SimpleRigidBody::SetRotation( const Float4x4 &rotation )
{
	this->current.SetRotation( rotation );
}

void SimpleRigidBody::SetOrientation( const Float4x4 &orientation )
{
	this->current.SetOrientation( orientation );
}