#include "SphericalRigidBody.h"
#include "PhysicsAPI_Impl.h"

using namespace ::Oyster::Physics;
using namespace ::Oyster::Physics3D;
using namespace ::Oyster::Math3D;
using namespace ::Oyster::Collision3D;
using namespace ::Utility::DynamicMemory;
using namespace ::Utility::Value;

SphericalRigidBody::SphericalRigidBody()
{
	this->rigid = RigidBody();
	this->rigid.SetMass_KeepMomentum( 10.0f );
	this->gravityNormal = Float3::null;
	this->collisionAction = Default::EventAction_Collision;
	this->ignoreGravity = this->isForwarded = false;
	this->scene = nullptr;
	this->body = Sphere( Float3::null, 0.5f );
}

SphericalRigidBody::SphericalRigidBody( const API::SphericalBodyDescription &desc )
{
	this->rigid = RigidBody();
	this->rigid.SetRotation( desc.rotation );
	this->rigid.centerPos = desc.centerPosition;
	this->rigid.boundingReach = Float4( desc.radius, desc.radius, desc.radius, 0.0f );
	this->rigid.SetMass_KeepMomentum( desc.mass );
	this->rigid.SetMomentOfInertia_KeepMomentum( Formula::MomentOfInertia::CreateSphereMatrix( desc.mass, desc.radius ) );
	this->deltaPos = Float4::null;
	this->deltaAxis = Float4::null;

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
	this->scene = nullptr;
	this->body = Sphere( desc.centerPosition, desc.radius );
}

SphericalRigidBody::~SphericalRigidBody() {}

UniquePointer<ICustomBody> SphericalRigidBody::Clone() const	
{
	return new SphericalRigidBody( *this );
}

SphericalRigidBody::State SphericalRigidBody::GetState() const
{
	return State( this->rigid.GetMass(), this->rigid.restitutionCoeff,
				  this->rigid.frictionCoeff_Static, this->rigid.frictionCoeff_Kinetic,
				  this->rigid.GetMomentOfInertia(), this->rigid.boundingReach,
				  this->rigid.centerPos, this->rigid.axis,
				  this->rigid.momentum_Linear, this->rigid.momentum_Angular );
}

SphericalRigidBody::State & SphericalRigidBody::GetState( SphericalRigidBody::State &targetMem ) const
{
	return targetMem = State( this->rigid.GetMass(), this->rigid.restitutionCoeff,
							  this->rigid.frictionCoeff_Static, this->rigid.frictionCoeff_Kinetic,
							  this->rigid.GetMomentOfInertia(), this->rigid.boundingReach,
							  this->rigid.centerPos, this->rigid.axis,
							  this->rigid.momentum_Linear, this->rigid.momentum_Angular );
}

void SphericalRigidBody::SetState( const SphericalRigidBody::State &state )
{
	this->rigid.centerPos			  = state.GetCenterPosition();
	this->rigid.SetRotation( state.GetRotation() );
	this->rigid.boundingReach		  = state.GetReach();
	this->rigid.momentum_Linear		  = state.GetLinearMomentum();
	this->rigid.momentum_Angular	  = state.GetAngularMomentum();
	this->rigid.impulse_Linear		 += state.GetLinearImpulse();
	this->rigid.impulse_Angular		 += state.GetAngularImpulse();
	this->rigid.restitutionCoeff	  = state.GetRestitutionCoeff();
	this->rigid.frictionCoeff_Static  = state.GetFrictionCoeff_Static();
	this->rigid.frictionCoeff_Kinetic = state.GetFrictionCoeff_Kinetic();

	if( state.IsForwarded() )
	{
		this->deltaPos += state.GetForward_DeltaPos();
		this->deltaAxis += state.GetForward_DeltaAxis();
		this->isForwarded = false;
	}

	if( this->scene )
	{
		if( state.IsSpatiallyAltered() )
		{
			unsigned int tempRef = this->scene->GetTemporaryReferenceOf( this );
			this->scene->SetAsAltered( tempRef );
			this->scene->EvaluatePosition( tempRef );
		}
		else if( state.IsDisturbed() )
		{
			this->scene->SetAsAltered( this->scene->GetTemporaryReferenceOf(this) );
		}
	}
}

ICustomBody::SubscriptMessage SphericalRigidBody::CallSubscription( const ICustomBody *proto, const ICustomBody *deuter )
{
	return this->collisionAction( proto, deuter );
}

bool SphericalRigidBody::IsAffectedByGravity() const
{
	return !this->ignoreGravity;
}

bool SphericalRigidBody::Intersects( const ICollideable &shape ) const
{
	return Sphere( this->rigid.centerPos, this->rigid.boundingReach.x ).Intersects( shape );
}

bool SphericalRigidBody::Intersects( const ICollideable &shape, Float4 &worldPointOfContact ) const
{
	return Sphere( this->rigid.centerPos, this->rigid.boundingReach.x ).Intersects( shape, worldPointOfContact );
}

bool SphericalRigidBody::Intersects( const ICustomBody &object, Float4 &worldPointOfContact ) const
{
	return object.Intersects( Sphere(this->rigid.centerPos, this->rigid.boundingReach.x), worldPointOfContact );
}

Sphere & SphericalRigidBody::GetBoundingSphere( Sphere &targetMem ) const
{
	return targetMem = this->body;
}

Float4 & SphericalRigidBody::GetNormalAt( const Float4 &worldPos, Float4 &targetMem ) const
{
	targetMem = worldPos - this->rigid.centerPos;
	Float magnitude = targetMem.GetMagnitude();
	if( magnitude != 0.0f )
	{ // sanity check
		targetMem.Normalize();
	}

	return targetMem;
}

Float3 & SphericalRigidBody::GetGravityNormal( Float3 &targetMem ) const
{
	return targetMem = this->gravityNormal;	
}

//Float3 & SphericalRigidBody::GetCenter( Float3 &targetMem ) const
//{
//	return targetMem = this->rigid.centerPos;
//}
//
//Float4x4 & SphericalRigidBody::GetRotation( Float4x4 &targetMem ) const
//{
//	return targetMem = this->rigid.box.rotation;
//}
//
//Float4x4 & SphericalRigidBody::GetOrientation( Float4x4 &targetMem ) const
//{
//	return targetMem = this->rigid.GetOrientation();
//}
//
//Float4x4 & SphericalRigidBody::GetView( Float4x4 &targetMem ) const
//{
//	return targetMem = this->rigid.GetView();
//}

//Float3 SphericalRigidBody::GetRigidLinearVelocity() const
//{
//	return this->rigid.GetLinearVelocity();
//}

UpdateState SphericalRigidBody::Update( Float timeStepLength )
{
	if( this->isForwarded )
	{
		this->rigid.Move( this->deltaPos, this->deltaAxis );
		this->deltaPos = Float4::null;
		this->deltaAxis = Float4::null;
		this->isForwarded = false;
	}

	this->rigid.Update_LeapFrog( timeStepLength );
	this->body.center = this->rigid.centerPos;

	// compare previous and new state and return result
	//return this->current == this->previous ? UpdateState_resting : UpdateState_altered;
	return UpdateState_altered;
}

void SphericalRigidBody::Predict( ::Oyster::Math::Float4 &outDeltaPos, ::Oyster::Math::Float4 &outDeltaAxis, const ::Oyster::Math::Float4 &actingLinearImpulse, const ::Oyster::Math::Float4 &actingAngularImpulse, ::Oyster::Math::Float deltaTime )
{
	this->rigid.Predict_LeapFrog( outDeltaPos, outDeltaAxis, actingLinearImpulse, actingAngularImpulse, deltaTime );
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

void SphericalRigidBody::SetScene( void *scene )
{
	this->scene = (Octree*)scene;
}

void SphericalRigidBody::SetGravity( bool ignore )
{
	this->ignoreGravity = ignore;
	this->gravityNormal = Float3::null;
}

void SphericalRigidBody::SetGravityNormal( const Float3 &normalizedVector )
{
	this->gravityNormal = normalizedVector;
}

//void SphericalRigidBody::SetMomentOfInertiaTensor_KeepVelocity( const Float4x4 &localI )
//{
//	this->rigid.SetMomentOfInertia_KeepVelocity( localI );
//}
//
//void SphericalRigidBody::SetMomentOfInertiaTensor_KeepMomentum( const Float4x4 &localI )
//{
//	this->rigid.SetMomentOfInertia_KeepMomentum( localI );
//}
//
//void SphericalRigidBody::SetMass_KeepVelocity( Float m )
//{
//	this->rigid.SetMass_KeepVelocity( m );
//}
//
//void SphericalRigidBody::SetMass_KeepMomentum( Float m )
//{
//	this->rigid.SetMass_KeepMomentum( m );
//}
//
//void SphericalRigidBody::SetCenter( const Float3 &worldPos )
//{
//	this->rigid.SetCenter( worldPos );
//	this->body.center = worldPos;
//}
//
//void SphericalRigidBody::SetRotation( const Float4x4 &rotation )
//{
//	this->rigid.SetRotation( rotation );
//}
//
//void SphericalRigidBody::SetOrientation( const Float4x4 &orientation )
//{
//	this->rigid.SetOrientation( orientation );
//	this->body.center = orientation.v[3].xyz;
//}
//
//void SphericalRigidBody::SetSize( const Float3 &size )
//{
//	this->rigid.SetSize( size );
//	this->body.radius = 0.5f * Min( Min( size.x, size.y ), size.z ); // inline Min( FloatN )?
//}
//
//void SphericalRigidBody::SetMomentum( const Float3 &worldG )
//{
//	this->rigid.SetLinearMomentum( worldG );
//}
