#ifndef OYSTER_PHYSICS_SPHERICAL_RIGIDBODY_H
#define OYSTER_PHYSICS_SPHERICAL_RIGIDBODY_H

#include "..\PhysicsAPI.h"
#include "RigidBody.h"
#include "Sphere.h"
#include "Octree.h"

namespace Oyster { namespace Physics
{
	class SphericalRigidBody : public ICustomBody
	{
	public:
		SphericalRigidBody();
		SphericalRigidBody( const API::SphericalBodyDescription &desc );
		virtual ~SphericalRigidBody();

		::Utility::DynamicMemory::UniquePointer<ICustomBody> Clone() const;
		
		State GetState() const;
		State & GetState( State &targetMem = State() ) const;
		void SetState( const State &state );
		//::Oyster::Math::Float3 GetRigidLinearVelocity() const;

		SubscriptMessage CallSubscription_Collision( const ICustomBody *deuter );
		void CallSubscription_CollisionResponse( const ICustomBody *deuter, ::Oyster::Math::Float kineticEnergyLoss );
		void CallSubscription_Move();

		bool IsAffectedByGravity() const;
		bool Intersects( const ::Oyster::Collision3D::ICollideable &shape ) const;
		bool Intersects( const ::Oyster::Collision3D::ICollideable &shape, ::Oyster::Math::Float4 &worldPointOfContact ) const;
		bool Intersects( const ICustomBody &object, ::Oyster::Math::Float4 &worldPointOfContact ) const;

		::Oyster::Collision3D::Sphere &	GetBoundingSphere( ::Oyster::Collision3D::Sphere &targetMem = ::Oyster::Collision3D::Sphere() ) const;
		::Oyster::Math::Float4 &		GetNormalAt( const ::Oyster::Math::Float4 &worldPos, ::Oyster::Math::Float4 &targetMem = ::Oyster::Math::Float4() ) const;
		::Oyster::Math::Float3 &		GetGravityNormal( ::Oyster::Math::Float3 &targetMem = ::Oyster::Math::Float3() ) const;
		void *							GetCustomTag() const;
		//::Oyster::Math::Float3 &		GetCenter( ::Oyster::Math::Float3 &targetMem = ::Oyster::Math::Float3() ) const;
		//::Oyster::Math::Float4x4 &		GetRotation( ::Oyster::Math::Float4x4 &targetMem = ::Oyster::Math::Float4x4() ) const;
		//::Oyster::Math::Float4x4 &		GetOrientation( ::Oyster::Math::Float4x4 &targetMem = ::Oyster::Math::Float4x4() ) const;
		//::Oyster::Math::Float4x4 &		GetView( ::Oyster::Math::Float4x4 &targetMem = ::Oyster::Math::Float4x4() ) const;

		UpdateState Update( ::Oyster::Math::Float timeStepLength );
		void Predict( ::Oyster::Math::Float4 &outDeltaPos, ::Oyster::Math::Float4 &outDeltaAxis, const ::Oyster::Math::Float4 &actingLinearImpulse, const ::Oyster::Math::Float4 &actingAngularImpulse, ::Oyster::Math::Float deltaTime );

		void SetScene( void *scene );

		void SetSubscription( EventAction_Collision functionPointer );
		void SetSubscription( EventAction_CollisionResponse functionPointer );
		void SetSubscription( EventAction_Move functionPointer );

		void SetGravity( bool ignore);
		void SetGravityNormal( const ::Oyster::Math::Float3 &normalizedVector );
		void SetCustomTag( void *ref );
		//void SetMomentOfInertiaTensor_KeepVelocity( const ::Oyster::Math::Float4x4 &localI );
		//void SetMomentOfInertiaTensor_KeepMomentum( const ::Oyster::Math::Float4x4 &localI );
		//void SetMass_KeepVelocity( ::Oyster::Math::Float m );
		//void SetMass_KeepMomentum( ::Oyster::Math::Float m );
		//void SetCenter( const ::Oyster::Math::Float3 &worldPos );
		//void SetRotation( const ::Oyster::Math::Float4x4 &rotation );
		//void SetOrientation( const ::Oyster::Math::Float4x4 &orientation );
		//void SetSize( const ::Oyster::Math::Float3 &size );
		//void SetMomentum( const ::Oyster::Math::Float3 &worldG );

	private:
		::Oyster::Physics3D::RigidBody rigid;
		::Oyster::Math::Float4 deltaPos, deltaAxis;
		::Oyster::Math::Float3 gravityNormal;
		EventAction_Collision onCollision;
		EventAction_CollisionResponse onCollisionResponse;
		EventAction_Move onMovement;
		Octree *scene;
		void *customTag;
		bool ignoreGravity, isForwarded;
	};
} }

#endif