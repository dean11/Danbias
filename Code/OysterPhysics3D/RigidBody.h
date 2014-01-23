/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#ifndef OYSTER_PHYSICS_3D_RIGIDBODY_H
#define OYSTER_PHYSICS_3D_RIGIDBODY_H

#include "OysterMath.h"
#include "OysterCollision3D.h"
#include "OysterPhysics3D.h"
#include "Inertia.h"

namespace Oyster { namespace Physics3D
{
	struct RigidBody
	{ //! A struct of a simple rigid body.
	public:
		::Oyster::Math::Float4		centerPos,				//!< Location of the body's center in the world.
									axis,					//!< Euler rotationAxis of the body.
									boundingReach,			//!< 
									momentum_Linear,		//!< The linear momentum G (kg*m/s).
									momentum_Angular,		//!< The angular momentum H (Nm*s) around an parallell axis.
									impulse_Linear,			//!< The linear impulse sum Jl (kg*m/s) that will be consumed each update.
									impulse_Angular,		//!< The angular impulse sum Ja (kg*m^2/s) that will be consumed each update.		
									gravityNormal;
		::Oyster::Math::Float		restitutionCoeff,		//!< 
									frictionCoeff_Static,	//!< 
									frictionCoeff_Kinetic;	//!< 

		RigidBody();

		RigidBody & operator = ( const RigidBody &body );

		void Update_LeapFrog( ::Oyster::Math::Float updateFrameLength );
		void Predict_LeapFrog( ::Oyster::Math::Float4 &outDeltaPos, ::Oyster::Math::Float4 &outDeltaAxis, const ::Oyster::Math::Float4 &actingLinearImpulse, const ::Oyster::Math::Float4 &actingAngularImpulse, ::Oyster::Math::Float deltaTime );

		void Move( const ::Oyster::Math::Float4 &deltaPos, const ::Oyster::Math::Float4 &deltaAxis );
		void ApplyImpulse( const ::Oyster::Math::Float4 &worldJ, const ::Oyster::Math::Float4 &atWorldPos );
		void ApplyImpulse_Linear( const ::Oyster::Math::Float4 &worldJ );
		void ApplyImpulse_Angular( const ::Oyster::Math::Float4 &worldJ );
		void ApplyForce( const ::Oyster::Math::Float4 &worldF, ::Oyster::Math::Float updateFrameLength );
		void ApplyForce( const ::Oyster::Math::Float4 &worldF, ::Oyster::Math::Float updateFrameLength, const ::Oyster::Math::Float4 &atWorldPos );

		// GET METHODS ////////////////////////////////

		const ::Oyster::Physics3D::MomentOfInertia & GetMomentOfInertia() const;
			  ::Oyster::Math::Float					 GetMass() const;
		const ::Oyster::Math::Quaternion &			 GetRotation() const;
			  ::Oyster::Math::Float4x4				 GetRotationMatrix() const;
			  ::Oyster::Math::Float4x4				 GetOrientation() const;
			  ::Oyster::Math::Float4x4				 GetView() const;
 			  ::Oyster::Math::Float4x4				 GetToWorldMatrix() const;
			  ::Oyster::Math::Float4x4				 GetToLocalMatrix() const;
			  ::Oyster::Math::Float4				 GetSize() const;
			  ::Oyster::Math::Float4				 GetVelocity_Linear() const;
			  ::Oyster::Math::Float4				 GetVelocity_Angular() const;
			  ::Oyster::Math::Float4				 GetLinearMomentum( const ::Oyster::Math::Float4 &atWorldPos ) const;

		// SET METHODS ////////////////////////////////

		void SetMomentOfInertia_KeepVelocity( const ::Oyster::Physics3D::MomentOfInertia &localTensorI );
		void SetMomentOfInertia_KeepMomentum( const ::Oyster::Physics3D::MomentOfInertia &localTensorI );
		void SetMass_KeepVelocity( const ::Oyster::Math::Float &m );
		void SetMass_KeepMomentum( const ::Oyster::Math::Float &m );

		void SetOrientation( const ::Oyster::Math::Float4x4 &o );
		void SetRotation( const ::Oyster::Math::Float4x4 &r );
		void SetSize( const ::Oyster::Math::Float4 &widthHeight );

		void SetMomentum_Linear( const ::Oyster::Math::Float4 &worldG, const ::Oyster::Math::Float4 &atWorldPos );

		void SetVelocity_Linear( const ::Oyster::Math::Float4 &worldV );
		void SetVelocity_Linear( const ::Oyster::Math::Float4 &worldV, const ::Oyster::Math::Float4 &atWorldPos );
		void SetVelocity_Angular( const ::Oyster::Math::Float4 &worldW );
		
		void SetImpulse_Linear( const ::Oyster::Math::Float4 &worldJ, const ::Oyster::Math::Float4 &atWorldPos );
		void SetForce( const ::Oyster::Math::Float4 &worldF, ::Oyster::Math::Float updateFrameLength );
		void SetForce( const ::Oyster::Math::Float4 &worldF, ::Oyster::Math::Float updateFrameLength, const ::Oyster::Math::Float4 &atWorldPos );

	private:
		::Oyster::Math::Float mass;						//!< m (kg)
		//::Oyster::Math::Float4x4 momentOfInertiaTensor;	//!< I (Nm*s) Tensor matrix ( only need to be 3x3 matrix, but is 4x4 for future hardware acceleration )	(localValue)
		::Oyster::Physics3D::MomentOfInertia momentOfInertiaTensor;
		::Oyster::Math::Quaternion rotation;			//!< RotationAxis of the body.
	};
} }

#include "RigidBody_Inline.h"

#endif
