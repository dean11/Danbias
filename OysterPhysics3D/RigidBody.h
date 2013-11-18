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
		::Oyster::Collision3D::Box box;				/// Contains data representing physical presence.
		::Oyster::Math::Float3 angularMomentum,		/// The angular momentum H (Nm*s) around an parallell axis.
							   linearMomentum,		/// The linear momentum G (kg*m/s).
							   impulseTorqueSum,	/// The impulse torque T (Nm) that will be consumed each update.
							   impulseForceSum;		/// The impulse force F (N) that will be consumed each update.

		RigidBody( const ::Oyster::Collision3D::Box &box = ::Oyster::Collision3D::Box(), ::Oyster::Math::Float mass = 1.0f );

		RigidBody & operator = ( const RigidBody &body );

		void Update_LeapFrog( ::Oyster::Math::Float deltaTime );
		void ApplyImpulseForce( const ::Oyster::Math::Float3 &f );
		void ApplyImpulseForceAt_Local( const ::Oyster::Math::Float3 &f, const ::Oyster::Math::Float3 &pos );
		void ApplyImpulseForceAt_World( const ::Oyster::Math::Float3 &f, const ::Oyster::Math::Float3 &pos ); /// ApplyImpulseForce_LocalPos is preferred
		void ApplyLinearImpulseAcceleration( const ::Oyster::Math::Float3 &a );
		void ApplyLinearImpulseAccelerationAt_Local( const ::Oyster::Math::Float3 &a, const ::Oyster::Math::Float3 &pos );
		void ApplyLinearImpulseAccelerationAt_World( const ::Oyster::Math::Float3 &a, const ::Oyster::Math::Float3 &pos ); /// ApplyLinearImpulseAcceleration_LocalPos is preferred
		void ApplyImpulseTorque( const ::Oyster::Math::Float3 &t );
		void ApplyAngularImpulseAcceleration( const ::Oyster::Math::Float3 &a );

		// ACCESS METHODS /////////////////////////////

		::Oyster::Math::Float4x4 &		 AccessOrientation();
		const ::Oyster::Math::Float4x4 & AccessOrientation() const;
		::Oyster::Math::Float3 &		 AccessBoundingReach();
		const ::Oyster::Math::Float3 &	 AccessBoundingReach() const;
		::Oyster::Math::Float3 &		 AccessCenter();
		const ::Oyster::Math::Float3 &	 AccessCenter() const;

		// GET METHODS ////////////////////////////////

		const ::Oyster::Math::Float4x4 & GetMomentOfInertia() const;
		const ::Oyster::Math::Float &	 GetMass() const;

		const ::Oyster::Math::Float4x4 & GetOrientation() const;
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

		::Oyster::Math::Float3			 GetTangentialImpulseForceAt_Local( const ::Oyster::Math::Float3 &pos ) const;
		::Oyster::Math::Float3			 GetTangentialImpulseForceAt_World( const ::Oyster::Math::Float3 &pos ) const;
		::Oyster::Math::Float3			 GetTangentialLinearMomentumAt_Local( const ::Oyster::Math::Float3 &pos ) const;
		::Oyster::Math::Float3			 GetTangentialLinearMomentumAt_World( const ::Oyster::Math::Float3 &pos ) const;
		::Oyster::Math::Float3			 GetTangentialImpulseAccelerationAt_Local( const ::Oyster::Math::Float3 &pos ) const;
		::Oyster::Math::Float3			 GetTangentialImpulseAccelerationAt_World( const ::Oyster::Math::Float3 &pos ) const;
		::Oyster::Math::Float3			 GetTangentialLinearVelocityAt_Local( const ::Oyster::Math::Float3 &pos ) const;
		::Oyster::Math::Float3			 GetTangentialLinearVelocityAt_World( const ::Oyster::Math::Float3 &pos ) const;

		::Oyster::Math::Float3			 GetImpulseForceAt_Local( const ::Oyster::Math::Float3 &pos ) const;
		::Oyster::Math::Float3			 GetImpulseForceAt_World( const ::Oyster::Math::Float3 &pos ) const;
		::Oyster::Math::Float3			 GetLinearMomentumAt_Local( const ::Oyster::Math::Float3 &pos ) const;
		::Oyster::Math::Float3			 GetLinearMomentumAt_World( const ::Oyster::Math::Float3 &pos ) const;
		::Oyster::Math::Float3			 GetImpulseAccelerationAt_Local( const ::Oyster::Math::Float3 &pos ) const;
		::Oyster::Math::Float3			 GetImpulseAccelerationAt_World( const ::Oyster::Math::Float3 &pos ) const;
		::Oyster::Math::Float3			 GetLinearVelocityAt_Local( const ::Oyster::Math::Float3 &pos ) const;
		::Oyster::Math::Float3			 GetLinearVelocityAt_World( const ::Oyster::Math::Float3 &pos ) const;

		// SET METHODS ////////////////////////////////

		void SetMomentOfInertia( const ::Oyster::Math::Float4x4 &i );
		void SetMass_KeepVelocity( const ::Oyster::Math::Float &m );
		void SetMass_KeepMomentum( const ::Oyster::Math::Float &m );

		void SetOrientation( const ::Oyster::Math::Float4x4 &o );
		void SetSize( const ::Oyster::Math::Float3 &widthHeight );
		void SetCenter( const ::Oyster::Math::Float3 &p );
		
		void SetImpulseTorque( const ::Oyster::Math::Float3 &t );
		void SetAngularMomentum( const ::Oyster::Math::Float3 &h );
		void SetAngularImpulseAcceleration( const ::Oyster::Math::Float3 &a );
		void SetAngularVelocity( const ::Oyster::Math::Float3 &w );
		
		void SetImpulseForce( const ::Oyster::Math::Float3 &f );
		void SetLinearMomentum( const ::Oyster::Math::Float3 &g );
		void SetLinearImpulseAcceleration( const ::Oyster::Math::Float3 &a );
		void SetLinearVelocity( const ::Oyster::Math::Float3 &v );

		void SetImpulseForceAt_Local( const ::Oyster::Math::Float3 &f, const ::Oyster::Math::Float3 &pos );
		void SetImpulseForceAt_World( const ::Oyster::Math::Float3 &f, const ::Oyster::Math::Float3 &pos );
		void SetLinearMomentumAt_Local( const ::Oyster::Math::Float3 &g, const ::Oyster::Math::Float3 &pos );
		void SetLinearMomentumAt_World( const ::Oyster::Math::Float3 &g, const ::Oyster::Math::Float3 &pos );
		void SetImpulseAccelerationAt_Local( const ::Oyster::Math::Float3 &a, const ::Oyster::Math::Float3 &pos );
		void SetImpulseAccelerationAt_World( const ::Oyster::Math::Float3 &a, const ::Oyster::Math::Float3 &pos );
		void SetLinearVelocityAt_Local( const ::Oyster::Math::Float3 &v, const ::Oyster::Math::Float3 &pos );
		void SetLinearVelocityAt_World( const ::Oyster::Math::Float3 &v, const ::Oyster::Math::Float3 &pos );

	private:
		::Oyster::Math::Float mass; /// m (kg)
		::Oyster::Math::Float4x4 momentOfInertiaTensor; /// I (Nm*s) Tensor matrix ( only need to be 3x3 matrix, but is 4x4 for future hardware acceleration )
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
