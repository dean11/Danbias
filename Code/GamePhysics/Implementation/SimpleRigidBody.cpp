#include "SimpleRigidBody.h"
#include "PhysicsAPI_Impl.h"

using namespace ::Oyster::Physics;
using namespace ::Oyster::Physics3D;
using namespace ::Oyster::Math3D;
using namespace ::Oyster::Collision3D;
using namespace ::Utility::DynamicMemory;
using namespace ::Utility::Value;

namespace Private
{
	const Float epsilon = (const Float)1e-20;

	// Float calculations can suffer roundingerrors. Which is where epsilon = 1e-20 comes into the picture
	inline bool EqualsZero( const Float &value )
	{ // by Dan Andersson
		return Abs( value ) < epsilon;
	}

	inline bool Contains( const Plane &container, const Float4 &pos )
	{ // by Dan Andersson
		return EqualsZero( container.normal.Dot( pos ) + container.phasing );
	}

	// revision of Ray Vs Plane intersect test, there ray is more of an axis
	bool Intersects( const Ray &axis, const Plane &plane, Float &connectDistance )
	{ // by Dan Andersson
		Float c = plane.normal.Dot(axis.direction);
		if( EqualsZero(c) )
		{ // axis is parallell with the plane. (axis direction orthogonal with the planar normal)
			connectDistance = 0.0f;
			return Contains( plane, axis.origin );
		}

		connectDistance = -plane.phasing;
		connectDistance -= plane.normal.Dot( axis.origin );
		connectDistance /= c;

		return true;
	}
}

SimpleRigidBody::SimpleRigidBody()
{
	this->rigid = RigidBody();
	this->rigid.SetMass_KeepMomentum( 16.0f );
	this->gravityNormal = Float3::null;
	this->onCollision = Default::EventAction_Collision;
	this->onMovement = Default::EventAction_Move;
	this->scene = nullptr;
	this->customTag = nullptr;
	this->ignoreGravity = this->isForwarded = false;
}

SimpleRigidBody::SimpleRigidBody( const API::SimpleBodyDescription &desc )
{
	this->rigid.SetRotation( desc.rotation );
	this->rigid.centerPos = desc.centerPosition;
	this->rigid.SetSize( desc.size );
	this->rigid.SetMass_KeepMomentum( desc.mass );
	this->rigid.SetMomentOfInertia_KeepMomentum( desc.inertiaTensor );
	this->deltaPos = Float4::null;
	this->deltaAxis = Float4::null;

	this->gravityNormal = Float3::null;
	
	if( desc.subscription_onCollision )
	{
		this->onCollision = desc.subscription_onCollision;
	}
	else
	{
		this->onCollision = Default::EventAction_Collision;
	}

	if( desc.subscription_onMovement )
	{
		this->onMovement= desc.subscription_onMovement;
	}
	else
	{
		this->onMovement = Default::EventAction_Move;
	}

	this->scene = nullptr;
	this->customTag = nullptr;
	this->ignoreGravity = desc.ignoreGravity;
}

SimpleRigidBody::~SimpleRigidBody() {}

UniquePointer<ICustomBody> SimpleRigidBody::Clone() const	
{
	return new SimpleRigidBody( *this );
}

SimpleRigidBody::State SimpleRigidBody::GetState() const
{
	return State( this->rigid.GetMass(), this->rigid.restitutionCoeff,
				  this->rigid.frictionCoeff_Static, this->rigid.frictionCoeff_Kinetic,
				  this->rigid.GetMomentOfInertia(), this->rigid.boundingReach,
				  this->rigid.centerPos, this->rigid.axis,
				  this->rigid.momentum_Linear, this->rigid.momentum_Angular, 
				  this->rigid.gravityNormal );
}

SimpleRigidBody::State & SimpleRigidBody::GetState( SimpleRigidBody::State &targetMem ) const
{
	return targetMem = State( this->rigid.GetMass(), this->rigid.restitutionCoeff,
							  this->rigid.frictionCoeff_Static, this->rigid.frictionCoeff_Kinetic,
							  this->rigid.GetMomentOfInertia(), this->rigid.boundingReach,
							  this->rigid.centerPos, this->rigid.axis,
							  this->rigid.momentum_Linear, this->rigid.momentum_Angular,
							  this->rigid.gravityNormal );
}

void SimpleRigidBody::SetState( const SimpleRigidBody::State &state )
{
	this->rigid.centerPos			  = state.GetCenterPosition();
	//this->rigid.SetRotation( state.GetRotation() ); //! HACK: @todo Rotation temporary disabled
	this->rigid.boundingReach		  = state.GetReach();
	this->rigid.momentum_Linear		  = state.GetLinearMomentum();
	this->rigid.momentum_Angular	  = state.GetAngularMomentum();
	this->rigid.impulse_Linear		 += state.GetLinearImpulse();
	this->rigid.impulse_Angular		 += state.GetAngularImpulse();
	this->rigid.restitutionCoeff	  = state.GetRestitutionCoeff();
	this->rigid.frictionCoeff_Static  = state.GetFrictionCoeff_Static();
	this->rigid.frictionCoeff_Kinetic = state.GetFrictionCoeff_Kinetic();
	this->rigid.SetMass_KeepMomentum( state.GetMass() );
	this->rigid.SetMomentOfInertia_KeepMomentum( state.GetMomentOfInertia() );
	this->rigid.gravityNormal		  = state.GetGravityNormal();

	if( state.IsForwarded() )
	{
		this->deltaPos += state.GetForward_DeltaPos();
		this->deltaAxis += state.GetForward_DeltaAxis();
		this->isForwarded;
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

ICustomBody::SubscriptMessage SimpleRigidBody::CallSubscription_Collision( const ICustomBody *deuter )
{
	return this->onCollision( this, deuter );
}

void SimpleRigidBody::CallSubscription_Move()
{
	this->onMovement( this );
}

bool SimpleRigidBody::IsAffectedByGravity() const
{
	return !this->ignoreGravity;
}

bool SimpleRigidBody::Intersects( const ICollideable &shape ) const
{
	return Box( this->rigid.GetRotationMatrix(), this->rigid.centerPos, this->rigid.GetSize() ).Intersects( shape );
}

bool SimpleRigidBody::Intersects( const ICollideable &shape, Float4 &worldPointOfContact ) const
{
	return Box( this->rigid.GetRotationMatrix(), this->rigid.centerPos, this->rigid.GetSize() ).Intersects( shape, worldPointOfContact );
}

bool SimpleRigidBody::Intersects( const ICustomBody &object, Float4 &worldPointOfContact ) const
{
	return object.Intersects( Box(this->rigid.GetRotationMatrix(), this->rigid.centerPos, this->rigid.GetSize()), worldPointOfContact );
}

Sphere & SimpleRigidBody::GetBoundingSphere( Sphere &targetMem ) const
{
	return targetMem = Sphere( this->rigid.centerPos, this->rigid.boundingReach.GetMagnitude() );
}

Float4 & SimpleRigidBody::GetNormalAt( const Float4 &worldPos, Float4 &targetMem ) const
{
	Float4 offset = worldPos - this->rigid.centerPos;
	Float distance = offset.Dot( offset );
	Float3 normal = Float3::null;

	if( distance != 0.0f )
	{ // sanity check
		Ray axis( Float4::standard_unit_w, offset / (Float)::std::sqrt(distance) );
		Float minDistance = numeric_limits<Float>::max();
		Float4x4 rotationMatrix = this->rigid.GetRotationMatrix();
		
		if( Private::Intersects(axis, Plane(rotationMatrix.v[0], -this->rigid.boundingReach.x), axis.collisionDistance) )
		{ // check along x-axis
			if( axis.collisionDistance < 0.0f )
				normal = -rotationMatrix.v[0].xyz;
			else
				normal = rotationMatrix.v[0].xyz;

			minDistance = Abs( axis.collisionDistance );
		}

		if( Private::Intersects(axis, Plane(rotationMatrix.v[1], -this->rigid.boundingReach.y), axis.collisionDistance) )
		{ // check along y-axis
			distance = Abs( axis.collisionDistance ); // recycling memory
			if( minDistance > distance )
			{
				if( axis.collisionDistance < 0.0f )
					normal = -rotationMatrix.v[1].xyz;
				else
					normal = rotationMatrix.v[1].xyz;

				minDistance = distance;
			}
		}

		if( Private::Intersects(axis, Plane(rotationMatrix.v[2], -this->rigid.boundingReach.z), axis.collisionDistance) )
		{ // check along z-axis
			if( minDistance > Abs( axis.collisionDistance ) )
			{
				if( axis.collisionDistance < 0.0f )
					normal = -rotationMatrix.v[2].xyz;
				else
					normal = rotationMatrix.v[2].xyz;
			}
		}
	}
	targetMem.xyz = normal;
	targetMem.w = 0.0f;
	return targetMem;
}

Float3 & SimpleRigidBody::GetGravityNormal( Float3 &targetMem ) const
{
	return targetMem = this->gravityNormal;	
}

void * SimpleRigidBody::GetCustomTag() const
{
	return this->customTag;
}

//Float3 & SimpleRigidBody::GetCenter( Float3 &targetMem ) const
//{
//	return targetMem = this->rigid.centerPos;
//}
//
//Float4x4 & SimpleRigidBody::GetRotation( Float4x4 &targetMem ) const
//{
//	return targetMem = this->rigid.box.rotation;
//}
//
//Float4x4 & SimpleRigidBody::GetOrientation( Float4x4 &targetMem ) const
//{
//	return targetMem = this->rigid.GetOrientation();
//}
//
//Float4x4 & SimpleRigidBody::GetView( Float4x4 &targetMem ) const
//{
//	return targetMem = this->rigid.GetView();
//}

//Float3 SimpleRigidBody::GetRigidLinearVelocity() const
//{
//	return this->rigid.GetLinearVelocity();
//}


UpdateState SimpleRigidBody::Update( Float timeStepLength )
{
	if( this->isForwarded )
	{
		this->rigid.Move( this->deltaPos, this->deltaAxis );
		this->deltaPos = Float4::null;
		this->deltaAxis = Float4::null;
		this->isForwarded = false;
	}

	this->rigid.Update_LeapFrog( timeStepLength );

	//! @todo TODO: compare previous and new state and return result
	//return this->current == this->previous ? UpdateState_resting : UpdateState_altered;
	return UpdateState_altered;
}

void SimpleRigidBody::Predict( Float4 &outDeltaPos, Float4 &outDeltaAxis, const Float4 &actingLinearImpulse, const Float4 &actingAngularImpulse, Float deltaTime )
{
	this->rigid.Predict_LeapFrog( outDeltaPos, outDeltaAxis, actingLinearImpulse, actingAngularImpulse, deltaTime );
}

void SimpleRigidBody::SetScene( void *scene )
{
	this->scene = (Octree*)scene;
}

void SimpleRigidBody::SetSubscription( ICustomBody::EventAction_Collision functionPointer )
{
	if( functionPointer )
	{
		this->onCollision = functionPointer;
	}
	else
	{
		this->onCollision = Default::EventAction_Collision;
	}
}

void SimpleRigidBody::SetSubscription( ICustomBody::EventAction_Move functionPointer )
{
	if( functionPointer )
	{
		this->onMovement = functionPointer;
	}
	else
	{
		this->onMovement = Default::EventAction_Move;
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
	this->rigid.gravityNormal = Float4( this->gravityNormal, 0 );
}

void SimpleRigidBody::SetCustomTag( void *ref )
{
	this->customTag = ref;
}

//void SimpleRigidBody::SetMomentOfInertiaTensor_KeepVelocity( const Float4x4 &localI )
//{
//	this->rigid.SetMomentOfInertia_KeepVelocity( localI );
//}
//
//void SimpleRigidBody::SetMomentOfInertiaTensor_KeepMomentum( const Float4x4 &localI )
//{
//	this->rigid.SetMomentOfInertia_KeepMomentum( localI );
//}
//
//void SimpleRigidBody::SetMass_KeepVelocity( Float m )
//{
//	this->rigid.SetMass_KeepVelocity( m );
//}
//
//void SimpleRigidBody::SetMass_KeepMomentum( Float m )
//{
//	this->rigid.SetMass_KeepMomentum( m );
//}
//
//void SimpleRigidBody::SetCenter( const Float3 &worldPos )
//{
//	this->rigid.SetCenter( worldPos );
//}
//
//void SimpleRigidBody::SetRotation( const Float4x4 &rotation )
//{
//	this->rigid.SetRotation( rotation );
//}
//
//void SimpleRigidBody::SetOrientation( const Float4x4 &orientation )
//{
//	this->rigid.SetOrientation( orientation );
//}
//
//void SimpleRigidBody::SetSize( const Float3 &size )
//{
//	this->rigid.SetSize( size );
//}
//
//void SimpleRigidBody::SetMomentum( const Float3 &worldG )
//{
//	this->rigid.SetLinearMomentum( worldG );
//}
