/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#ifndef OYSTER_PHYSICS_3D_RIGIDBODY_H
#define OYSTER_PHYSICS_3D_RIGIDBODY_H

#include "OysterMath.h"
#include "OysterCollision3D.h"
#include "OysterPhysics3D.h"

namespace Oyster { namespace Physics3D
{
	struct RigidBody
	{	/// A struct of a simple rigid body.
	public:
		::Oyster::Collision3D::Box box;				/** Contains data representing physical presence.					(worldValue) */
		::Oyster::Math::Float3 angularMomentum,		/** The angular momentum H (Nm*s) around an parallell axis.			(worldValue) */
							   linearMomentum,		/** The linear momentum G (kg*m/s).									(worldValue) */
							   impulseTorqueSum,	/** The impulse torque T (Nm) that will be consumed each update.	(worldValue) */
							   impulseForceSum;		/** The impulse force F (N) that will be consumed each update.		(worldValue) */

		RigidBody( const ::Oyster::Collision3D::Box &box = ::Oyster::Collision3D::Box(), ::Oyster::Math::Float mass = 1.0f );

		RigidBody & operator = ( const RigidBody &body );

		void Update_LeapFrog( ::Oyster::Math::Float deltaTime );
		void ApplyImpulseForce( const ::Oyster::Math::Float3 &worldF );
		void ApplyImpulseForceAt( const ::Oyster::Math::Float3 &worldF, const ::Oyster::Math::Float3 &worldPos );
		void ApplyLinearImpulseAcceleration( const ::Oyster::Math::Float3 &worldA );
		void ApplyLinearImpulseAccelerationAt( const ::Oyster::Math::Float3 &worldA, const ::Oyster::Math::Float3 &worldPos );
		void ApplyImpulseTorque( const ::Oyster::Math::Float3 &worldT );
		void ApplyAngularImpulseAcceleration( const ::Oyster::Math::Float3 &worldA );

		// ACCESS METHODS /////////////////////////////

		::Oyster::Math::Float3 &		 AccessBoundingReach();
		const ::Oyster::Math::Float3 &	 AccessBoundingReach() const;
		::Oyster::Math::Float3 &		 AccessCenter();
		const ::Oyster::Math::Float3 &	 AccessCenter() const;

		// GET METHODS ////////////////////////////////

		const ::Oyster::Math::Float4x4 & GetMomentOfInertia() const;
		const ::Oyster::Math::Float &	 GetMass() const;

		const ::Oyster::Math::Float4x4   GetOrientation() const;
		::Oyster::Math::Float4x4		 GetView() const;
 		const ::Oyster::Math::Float4x4 & GetToWorldMatrix() const;
		::Oyster::Math::Float4x4		 GetToLocalMatrix() const;

		const ::Oyster::Math::Float3 &	 GetBoundingReach() const;
		::Oyster::Math::Float3			 GetSize() const;

		const ::Oyster::Math::Float3 &	 GetCenter() const;

		const ::Oyster::Math::Float3 &	 GetImpulsTorque() const;
		const ::Oyster::Math::Float3 &	 GetAngularMomentum() const;
		::Oyster::Math::Float3			 GetAngularImpulseAcceleration() const;
		::Oyster::Math::Float3			 GetAngularVelocity() const;
		
		const ::Oyster::Math::Float3 &	 GetImpulseForce() const;
		const ::Oyster::Math::Float3 &	 GetLinearMomentum() const;
		::Oyster::Math::Float3			 GetLinearImpulseAcceleration() const;
		::Oyster::Math::Float3			 GetLinearVelocity() const;

		void GetMomentumAt( const ::Oyster::Math::Float3 &worldPos, const ::Oyster::Math::Float3 &surfaceNormal, ::Oyster::Math::Float3 &normalMomentum, ::Oyster::Math::Float3 &tangentialMomentum ) const;

		// SET METHODS ////////////////////////////////

		void SetMomentOfInertia_KeepVelocity( const ::Oyster::Math::Float4x4 &localI );
		void SetMomentOfInertia_KeepMomentum( const ::Oyster::Math::Float4x4 &localI );
		void SetMass_KeepVelocity( const ::Oyster::Math::Float &m );
		void SetMass_KeepMomentum( const ::Oyster::Math::Float &m );

		void SetOrientation( const ::Oyster::Math::Float4x4 &o );
		void SetSize( const ::Oyster::Math::Float3 &widthHeight );
		void SetCenter( const ::Oyster::Math::Float3 &worldPos );
		
		void SetImpulseTorque( const ::Oyster::Math::Float3 &worldT );
		void SetAngularMomentum( const ::Oyster::Math::Float3 &worldH );
		void SetAngularImpulseAcceleration( const ::Oyster::Math::Float3 &worldA );
		void SetAngularVelocity( const ::Oyster::Math::Float3 &worldW );
		
		void SetImpulseForce( const ::Oyster::Math::Float3 &worldF );
		void SetLinearMomentum( const ::Oyster::Math::Float3 &worldG );
		void SetLinearImpulseAcceleration( const ::Oyster::Math::Float3 &worldA );
		void SetLinearVelocity( const ::Oyster::Math::Float3 &worldV );

		void SetImpulseForceAt( const ::Oyster::Math::Float3 &worldF, const ::Oyster::Math::Float3 &worldPos );
		void SetLinearMomentumAt( const ::Oyster::Math::Float3 &worldG, const ::Oyster::Math::Float3 &worldPos );
		void SetImpulseAccelerationAt( const ::Oyster::Math::Float3 &worldA, const ::Oyster::Math::Float3 &worldPos );
		void SetLinearVelocityAt( const ::Oyster::Math::Float3 &worldV, const ::Oyster::Math::Float3 &worldPos );

	private:
		::Oyster::Math::Float mass; /** m (kg) */
		::Oyster::Math::Float4x4 momentOfInertiaTensor; /** I (Nm*s) Tensor matrix ( only need to be 3x3 matrix, but is 4x4 for future hardware acceleration )	(localValue) */
	};

	// INLINE IMPLEMENTATIONS ///////////////////////////////////////

	inline const ::Oyster::Math::Float4x4 & RigidBody::GetToWorldMatrix() const
	{
		return this->GetOrientation();
	}

	inline ::Oyster::Math::Float4x4 RigidBody::GetToLocalMatrix() const
	{
		return this->GetView();
	}

} }

#endif
