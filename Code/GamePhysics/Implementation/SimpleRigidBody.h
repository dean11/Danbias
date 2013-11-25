#ifndef OYSTER_PHYSICS_SIMPLE_RIGIDBODY_H
#define OYSTER_PHYSICS_SIMPLE_RIGIDBODY_H

#include "..\PhysicsAPI.h"

namespace Oyster { namespace Physics
{
	class SimpleRigidBody : public ICustomBody
	{
	public:
		SimpleRigidBody();
		virtual ~SimpleRigidBody();

		::Utility::DynamicMemory::UniquePointer<ICustomBody> Clone() const;
		
		bool IsSubscribingCollisions() const;
		bool Intersects( const ICustomBody &object, ::Oyster::Math::Float &deltaWhen, ::Oyster::Math::Float3 &worldPointOfContact ) const;
		bool Intersects( const ::Oyster::Collision3D::ICollideable &shape ) const;

		::Oyster::Collision3D::Sphere &	GetBoundingSphere( ::Oyster::Collision3D::Sphere &targetMem = ::Oyster::Collision3D::Sphere() ) const;
		::Oyster::Math::Float3 &		GetNormalAt( const ::Oyster::Math::Float3 &worldPos, ::Oyster::Math::Float3 &targetMem = ::Oyster::Math::Float3() ) const;
		::Oyster::Math::Float3 &		GetCenter( ::Oyster::Math::Float3 &targetMem = ::Oyster::Math::Float3() ) const;
		::Oyster::Math::Float4x4 &		GetRotation( ::Oyster::Math::Float4x4 &targetMem = ::Oyster::Math::Float4x4() ) const;
		::Oyster::Math::Float4x4 &		GetOrientation( ::Oyster::Math::Float4x4 &targetMem = ::Oyster::Math::Float4x4() ) const;
		::Oyster::Math::Float4x4 &		GetView( ::Oyster::Math::Float4x4 &targetMem = ::Oyster::Math::Float4x4() ) const;

		UpdateState Update( ::Oyster::Math::Float timeStepLength );

		void SetMomentOfInertiaTensor_KeepVelocity( const ::Oyster::Math::Float4x4 &localI );
		void SetMomentOfInertiaTensor_KeepMomentum( const ::Oyster::Math::Float4x4 &localI );
		void SetMass_KeepVelocity( ::Oyster::Math::Float m );
		void SetMass_KeepMomentum( ::Oyster::Math::Float m );
		void SetCenter( const ::Oyster::Math::Float3 &worldPos );
		void SetRotation( const ::Oyster::Math::Float4x4 &rotation );
		void SetOrientation( const ::Oyster::Math::Float4x4 &orientation );

	private:
	};
} }

#endif