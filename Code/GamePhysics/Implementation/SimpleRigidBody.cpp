#include "SimpleRigidBody.h"
#include "PhysicsAPI_Impl.h"

using namespace ::Oyster::Physics;
using namespace ::Oyster::Physics3D;
using namespace ::Oyster::Math3D;
using namespace ::Oyster::Collision3D;
using namespace ::Utility::DynamicMemory;
using namespace ::Utility::Value;

SimpleRigidBody::SimpleRigidBody()
{
	this->rigid = RigidBody( Box(Float4x4::identity, Float3::null, Float3(1.0f)), 16.0f, Float4x4::identity );
	this->gravityNormal = Float3::null;
	this->collisionAction = Default::EventAction_Collision;
	this->ignoreGravity = false;
}

SimpleRigidBody::SimpleRigidBody( const API::SimpleBodyDescription &desc )
{
	this->rigid = RigidBody( Box( desc.rotation, desc.centerPosition, desc.size  ),
							 desc.mass,
							 desc.inertiaTensor );
	this->gravityNormal = Float3::null;
	
	if( desc.subscription )
	{
		this->collisionAction = desc.subscription;
	}
	else
	{
		this->collisionAction = Default::EventAction_Collision;
	}

	this->ignoreGravity = desc.ignoreGravity;
}

SimpleRigidBody::~SimpleRigidBody() {}

UniquePointer<ICustomBody> SimpleRigidBody::Clone() const	
{
	return new SimpleRigidBody( *this );
}

SimpleRigidBody::State SimpleRigidBody::GetState() const
{
	return State( this->rigid.box.boundingOffset.xyz, this->rigid.box.center.xyz, AngularAxis(this->rigid.box.rotation).xyz, this->rigid.linearMomentum, this->rigid.angularMomentum );
}

SimpleRigidBody::State & SimpleRigidBody::GetState( SimpleRigidBody::State &targetMem ) const
{
	return targetMem = State( this->rigid.box.boundingOffset.xyz, this->rigid.box.center.xyz, AngularAxis(this->rigid.box.rotation).xyz, this->rigid.linearMomentum, this->rigid.angularMomentum );
}

void SimpleRigidBody::SetState( const SimpleRigidBody::State &state )
{ /** @todo TODO: temporary solution! Need to know it's occtree */
	this->rigid.box.boundingOffset = state.GetReach();
	this->rigid.box.center = state.GetCenterPosition();
	this->rigid.box.rotation = state.GetRotation();
}

ICustomBody::SubscriptMessage SimpleRigidBody::CallSubscription( const ICustomBody *proto, const ICustomBody *deuter )
{
	return this->collisionAction( proto, deuter );
}

bool SimpleRigidBody::IsAffectedByGravity() const
{
	return !this->ignoreGravity;
}

bool SimpleRigidBody::Intersects( const ICollideable &shape ) const
{
	return this->rigid.box.Intersects( shape );
}

bool SimpleRigidBody::Intersects( const ICollideable &shape, Float4 &worldPointOfContact ) const
{
	return this->rigid.box.Intersects( shape, worldPointOfContact );
}

bool SimpleRigidBody::Intersects( const ICustomBody &object, Float4 &worldPointOfContact ) const
{
	return object.Intersects( this->rigid.box, worldPointOfContact );
}

Sphere & SimpleRigidBody::GetBoundingSphere( Sphere &targetMem ) const
{
	return targetMem = Sphere( this->rigid.box.center, this->rigid.box.boundingOffset.GetMagnitude() );
}

Float4 & SimpleRigidBody::GetNormalAt( const Float4 &worldPos, Float4 &targetMem ) const
{
	//! @todo TODO: better implementation needed
	return targetMem = (worldPos - this->rigid.box.center).GetNormalized();
}

Float3 & SimpleRigidBody::GetGravityNormal( Float3 &targetMem ) const
{
	return targetMem = this->gravityNormal;	
}

Float3 & SimpleRigidBody::GetCenter( Float3 &targetMem ) const
{
	return targetMem = this->rigid.box.center;
}

Float4x4 & SimpleRigidBody::GetRotation( Float4x4 &targetMem ) const
{
	return targetMem = this->rigid.box.rotation;
}

Float4x4 & SimpleRigidBody::GetOrientation( Float4x4 &targetMem ) const
{
	return targetMem = this->rigid.GetOrientation();
}

Float4x4 & SimpleRigidBody::GetView( Float4x4 &targetMem ) const
{
	return targetMem = this->rigid.GetView();
}

UpdateState SimpleRigidBody::Update( Float timeStepLength )
{
	this->rigid.Update_LeapFrog( timeStepLength );

	// compare previous and new state and return result
	//return this->current == this->previous ? UpdateState_resting : UpdateState_altered;
	return UpdateState_altered;
}

void SimpleRigidBody::SetSubscription( ICustomBody::EventAction_Collision functionPointer )
{
	if( functionPointer )
	{
		this->collisionAction = functionPointer;
	}
	else
	{
		this->collisionAction = Default::EventAction_Collision;
	}
}

void SimpleRigidBody::SetGravity( bool ignore)
{
	this->ignoreGravity = ignore;
	this->gravityNormal = Float3::null;
}

void SimpleRigidBody::SetGravityNormal( const Float3 &normalizedVector )
{
	this->gravityNormal = normalizedVector;
}

void SimpleRigidBody::SetMomentOfInertiaTensor_KeepVelocity( const Float4x4 &localI )
{
	this->rigid.SetMomentOfInertia_KeepVelocity( localI );
}

void SimpleRigidBody::SetMomentOfInertiaTensor_KeepMomentum( const Float4x4 &localI )
{
	this->rigid.SetMomentOfInertia_KeepMomentum( localI );
}

void SimpleRigidBody::SetMass_KeepVelocity( Float m )
{
	this->rigid.SetMass_KeepVelocity( m );
}

void SimpleRigidBody::SetMass_KeepMomentum( Float m )
{
	this->rigid.SetMass_KeepMomentum( m );
}

void SimpleRigidBody::SetCenter( const Float3 &worldPos )
{
	this->rigid.SetCenter( worldPos );
}

void SimpleRigidBody::SetRotation( const Float4x4 &rotation )
{
	this->rigid.SetRotation( rotation );
}

void SimpleRigidBody::SetOrientation( const Float4x4 &orientation )
{
	this->rigid.SetOrientation( orientation );
}

void SimpleRigidBody::SetSize( const Float3 &size )
{
	this->rigid.SetSize( size );
}

void SimpleRigidBody::SetMomentum( const Float3 &worldG )
{
	this->rigid.SetLinearMomentum( worldG );
}