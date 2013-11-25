#include "..\PhysicsAPI.h"

using namespace ::Oyster::Physics;
using namespace ::Oyster::Physics::Error;
using namespace ::Oyster::Math;
using namespace ::Oyster::Collision3D;
using namespace ::Utility::DynamicMemory;

NullBody::~NullBody() {}

UniquePointer<ICustomBody> NullBody::Clone() const	
{
	return new NullBody( *this );
}

bool NullBody::IsSubscribingCollisions() const
{
	return false;
}

bool NullBody::Intersects( const ICustomBody &object, Float &deltaWhen, Float3 &worldPointOfContact ) const
{
	return false;
}

bool NullBody::Intersects( const ICollideable &shape ) const
{
	return false;
}

Sphere & NullBody::GetBoundingSphere( Sphere &targetMem ) const
{
	return targetMem = Sphere( Float3::null, 0.0f );
}

Float3 & NullBody::GetNormalAt( const Float3 &worldPos, Float3 &targetMem ) const
{
	return targetMem = Float3::standard_unit_z;
}

Float3 & NullBody::GetCenter( Float3 &targetMem ) const
{
	return targetMem = Float3::null;
}

Float4x4 & NullBody::GetRotation( Float4x4 &targetMem ) const
{
	return targetMem = Float4x4::identity;
}

Float4x4 & NullBody::GetOrientation( Float4x4 &targetMem ) const
{
	return targetMem = Float4x4::identity;
}

Float4x4 & NullBody::GetView( Float4x4 &targetMem ) const
{
	return targetMem = Float4x4::identity;
}

UpdateState NullBody::Update( Float timeStepLength )
{
	return resting;
}

void NullBody::SetMomentOfInertiaTensor_KeepVelocity( const Float4x4 &localI ) {}

void NullBody::SetMomentOfInertiaTensor_KeepMomentum( const Float4x4 &localI ) {}

void NullBody::SetMass_KeepVelocity( Float m ) {}

void NullBody::SetMass_KeepMomentum( Float m ) {}

void NullBody::SetCenter( const Float3 &worldPos ) {}

void NullBody::SetRotation( const Float4x4 &rotation ) {}

void NullBody::SetOrientation( const Float4x4 &orientation ) {}
