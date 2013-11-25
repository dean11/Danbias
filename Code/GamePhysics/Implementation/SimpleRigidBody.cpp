#include "SimpleRigidBody.h"

using namespace ::Oyster::Physics;
using namespace ::Oyster::Math;
using namespace ::Oyster::Collision3D;
using namespace ::Utility::DynamicMemory;

SimpleRigidBody::SimpleRigidBody()
{
	//! @todo TODO: implement stub
}

SimpleRigidBody::~SimpleRigidBody()
{
	//! @todo TODO: implement stub
}

UniquePointer<ICustomBody> SimpleRigidBody::Clone() const	
{
	return new SimpleRigidBody( *this );
}

bool SimpleRigidBody::IsSubscribingCollisions() const
{
	//! @todo TODO: implement stub
	return false;
}

bool SimpleRigidBody::Intersects( const ICustomBody &object, Float &deltaWhen, Float3 &worldPointOfContact ) const
{
	//! @todo TODO: implement stub
	return false;
}

bool SimpleRigidBody::Intersects( const ICollideable &shape ) const
{
	//! @todo TODO: implement stub
	return false;
}

Sphere & SimpleRigidBody::GetBoundingSphere( Sphere &targetMem ) const
{
	//! @todo TODO: implement stub
	return targetMem = Sphere( Float3::null, 0.0f );
}

Float3 & SimpleRigidBody::GetNormalAt( const Float3 &worldPos, Float3 &targetMem ) const
{
	//! @todo TODO: implement stub
	return targetMem = Float3::standard_unit_z;
}

Float3 & SimpleRigidBody::GetCenter( Float3 &targetMem ) const
{
	//! @todo TODO: implement stub
	return targetMem = Float3::null;
}

Float4x4 & SimpleRigidBody::GetRotation( Float4x4 &targetMem ) const
{
	//! @todo TODO: implement stub
	return targetMem = Float4x4::identity;
}

Float4x4 & SimpleRigidBody::GetOrientation( Float4x4 &targetMem ) const
{
	//! @todo TODO: implement stub
	return targetMem = Float4x4::identity;
}

Float4x4 & SimpleRigidBody::GetView( Float4x4 &targetMem ) const
{
	//! @todo TODO: implement stub
	return targetMem = Float4x4::identity;
}

UpdateState SimpleRigidBody::Update( Float timeStepLength )
{
	//! @todo TODO: implement stub
	return resting;
}

void SimpleRigidBody::SetMomentOfInertiaTensor_KeepVelocity( const Float4x4 &localI )
{
	//! @todo TODO: implement stub
}

void SimpleRigidBody::SetMomentOfInertiaTensor_KeepMomentum( const Float4x4 &localI )
{
	//! @todo TODO: implement stub
}

void SimpleRigidBody::SetMass_KeepVelocity( Float m )
{
	//! @todo TODO: implement stub
}

void SimpleRigidBody::SetMass_KeepMomentum( Float m )
{
	//! @todo TODO: implement stub
}

void SimpleRigidBody::SetCenter( const Float3 &worldPos )
{
	//! @todo TODO: implement stub
}

void SimpleRigidBody::SetRotation( const Float4x4 &rotation )
{
	//! @todo TODO: implement stub
}

void SimpleRigidBody::SetOrientation( const Float4x4 &orientation )
{
	//! @todo TODO: implement stub
}