#include "SphericalRigidBody.h"
#include "PhysicsAPI_Impl.h"

using namespace ::Oyster::Physics;
using namespace ::Oyster::Physics3D;
using namespace ::Oyster::Math3D;
using namespace ::Oyster::Collision3D;
using namespace ::Utility::DynamicMemory;
using namespace ::Utility::Value;

SphericalRigidBody::SphericalRigidBody()
	: previous(), current( Box(Float4x4::identity, Float3::null, Float3(1.0f)) ),
	  gravityNormal( 0.0f ),
	  collisionAction(Default::EventAction_Collision),
	  ignoreGravity( false ),
	  body( Float3::null, 0.5f ) {}

SphericalRigidBody::~SphericalRigidBody() {}

UniquePointer<ICustomBody> SphericalRigidBody::Clone() const	
{
	return new SphericalRigidBody( *this );
}

bool SphericalRigidBody::IsAffectedByGravity() const
{
	return !this->ignoreGravity;
}

bool SphericalRigidBody::Intersects( const ICustomBody &object, Float timeStepLength, Float &deltaWhen, Float3 &worldPointOfContact ) const
{
	if( object.Intersects(this->body) )
	{ //! @todo TODO: better implementation needed
		deltaWhen = timeStepLength;
		worldPointOfContact = Average( this->body.center, object.GetCenter() );
		return true;
	}
	else
	{
		return false;
	}
}

bool SphericalRigidBody::Intersects( const ICollideable &shape ) const
{
	return this->current.box.Intersects( shape );
}

Sphere & SphericalRigidBody::GetBoundingSphere( Sphere &targetMem ) const
{
	return targetMem = this->body;
}

Float3 & SphericalRigidBody::GetNormalAt( const Float3 &worldPos, Float3 &targetMem ) const
{
	//! @todo TODO: better implementation needed
	return targetMem = (worldPos - this->current.box.center).GetNormalized();
}

Float3 & SphericalRigidBody::GetGravityNormal( Float3 &targetMem ) const
{
	return targetMem = this->gravityNormal;	
}

Float3 & SphericalRigidBody::GetCenter( Float3 &targetMem ) const
{
	return targetMem = this->current.box.center;
}

Float4x4 & SphericalRigidBody::GetRotation( Float4x4 &targetMem ) const
{
	return targetMem = this->current.box.rotation;
}

Float4x4 & SphericalRigidBody::GetOrientation( Float4x4 &targetMem ) const
{
	return targetMem = this->current.GetOrientation();
}

Float4x4 & SphericalRigidBody::GetView( Float4x4 &targetMem ) const
{
	return targetMem = this->current.GetView();
}

UpdateState SphericalRigidBody::Update( Float timeStepLength )
{
	this->previous = this->current; // memorizing the old state

	this->current.Update_LeapFrog( timeStepLength );
	this->body.center = this->current.GetCenter();

	// compare previous and new state and return result
	return this->current == this->previous ? UpdateState_resting : UpdateState_altered;
}

void SphericalRigidBody::SetSubscription( ICustomBody::EventAction_Collision functionPointer )
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

void SphericalRigidBody::SetGravity( bool ignore)
{
	this->ignoreGravity = ignore;
	this->gravityNormal = Float3::null;
}

void SphericalRigidBody::SetGravityNormal( const Float3 &normalizedVector )
{
	this->gravityNormal = normalizedVector;
}

void SphericalRigidBody::SetMomentOfInertiaTensor_KeepVelocity( const Float4x4 &localI )
{
	this->current.SetMomentOfInertia_KeepVelocity( localI );
}

void SphericalRigidBody::SetMomentOfInertiaTensor_KeepMomentum( const Float4x4 &localI )
{
	this->current.SetMomentOfInertia_KeepMomentum( localI );
}

void SphericalRigidBody::SetMass_KeepVelocity( Float m )
{
	this->current.SetMass_KeepVelocity( m );
}

void SphericalRigidBody::SetMass_KeepMomentum( Float m )
{
	this->current.SetMass_KeepMomentum( m );
}

void SphericalRigidBody::SetCenter( const Float3 &worldPos )
{
	this->current.SetCenter( worldPos );
	this->body.center = worldPos;
}

void SphericalRigidBody::SetRotation( const Float4x4 &rotation )
{
	this->current.SetRotation( rotation );
}

void SphericalRigidBody::SetOrientation( const Float4x4 &orientation )
{
	this->current.SetOrientation( orientation );
	this->body.center = orientation.v[3].xyz;
}

void SphericalRigidBody::SetSize( const Float3 &size )
{
	this->current.SetSize( size );
	this->body.radius = 0.5f * Min( Min( size.x, size.y ), size.z ); // inline Min( FloatN )?
}