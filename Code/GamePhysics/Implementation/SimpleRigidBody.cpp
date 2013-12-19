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
	this->scene = nullptr;
}

SimpleRigidBody::SimpleRigidBody( const API::SimpleBodyDescription &desc )
{
	this->rigid = RigidBody( Box( desc.rotation, desc.centerPosition.xyz, desc.size.xyz  ),
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
	this->scene = nullptr;
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
				  this->rigid.GetMomentOfInertia(), this->rigid.box.boundingOffset,
				  this->rigid.box.center, AngularAxis(this->rigid.box.rotation),
				  Float4(this->rigid.linearMomentum, 0.0f), Float4(this->rigid.angularMomentum, 0.0f) );
}

SimpleRigidBody::State & SimpleRigidBody::GetState( SimpleRigidBody::State &targetMem ) const
{
	return targetMem = State( this->rigid.GetMass(), this->rigid.restitutionCoeff,
							  this->rigid.frictionCoeff_Static, this->rigid.frictionCoeff_Kinetic,
							  this->rigid.GetMomentOfInertia(), this->rigid.box.boundingOffset,
							  this->rigid.box.center, AngularAxis(this->rigid.box.rotation),
							  Float4(this->rigid.linearMomentum, 0.0f), Float4(this->rigid.angularMomentum, 0.0f) );
}

void SimpleRigidBody::SetState( const SimpleRigidBody::State &state )
{
	this->rigid.box.boundingOffset = state.GetReach();
	this->rigid.box.center = state.GetCenterPosition();
	this->rigid.box.rotation = state.GetRotation();
	this->rigid.angularMomentum = state.GetAngularMomentum().xyz;
	this->rigid.linearMomentum = state.GetLinearMomentum().xyz;
	this->rigid.impulseTorqueSum += state.GetAngularImpulse().xyz;
	this->rigid.impulseForceSum += state.GetLinearImpulse().xyz;
	this->rigid.restitutionCoeff = state.GetRestitutionCoeff();
	this->rigid.frictionCoeff_Static = state.GetFrictionCoeff_Static();
	this->rigid.frictionCoeff_Kinetic = state.GetFrictionCoeff_Kinetic();

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
	Float4 offset = worldPos - this->rigid.box.center;
	Float distance = offset.Dot( offset );
	Float3 normal = Float3::standard_unit_z;

	if( distance != 0.0f )
	{
		Ray ray( Float4::standard_unit_w, offset / (Float)::std::sqrt(distance) );
		Float minDistance = numeric_limits<Float>::max();

		if( ray.Intersects(Plane(this->rigid.box.xAxis, this->rigid.box.boundingOffset.x)) )
		{ // check along x-axis
			if( ray.collisionDistance < 0.0f )
				normal = -this->rigid.box.xAxis.xyz;
			else
				normal = this->rigid.box.xAxis.xyz;

			minDistance = Abs( ray.collisionDistance );
		}

		if( ray.Intersects(Plane(this->rigid.box.yAxis, this->rigid.box.boundingOffset.y)) )
		{ // check along y-axis
			distance = Abs( ray.collisionDistance ); // recycling memory
			if( minDistance > distance )
			{
				if( ray.collisionDistance < 0.0f )
					normal = -this->rigid.box.yAxis.xyz;
				else
					normal = this->rigid.box.yAxis.xyz;

				minDistance = distance;
			}
		}

		if( ray.Intersects(Plane(this->rigid.box.zAxis, this->rigid.box.boundingOffset.z)) )
		{ // check along z-axis
			if( minDistance > Abs( ray.collisionDistance ) )
			{
				if( ray.collisionDistance < 0.0f )
					normal = -this->rigid.box.zAxis.xyz;
				else
					normal = this->rigid.box.zAxis.xyz;
			}
		}
	}
	targetMem.xyz = normal;
	return targetMem;
}

Float3 & SimpleRigidBody::GetGravityNormal( Float3 &targetMem ) const
{
	return targetMem = this->gravityNormal;	
}

//Float3 & SimpleRigidBody::GetCenter( Float3 &targetMem ) const
//{
//	return targetMem = this->rigid.box.center;
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

Float3 SimpleRigidBody::GetRigidLinearVelocity() const
{
	return this->rigid.GetLinearVelocity();
}


UpdateState SimpleRigidBody::Update( Float timeStepLength )
{
	this->rigid.Update_LeapFrog( timeStepLength );

	// compare previous and new state and return result
	//return this->current == this->previous ? UpdateState_resting : UpdateState_altered;
	return UpdateState_altered;
}

void SimpleRigidBody::SetScene( void *scene )
{
	this->scene = (Octree*)scene;
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
