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
		::Oyster::Math::Quaternion  quaternion;
		::Oyster::Math::Float3		centerPos,				//!< Location of the body's center in the world.
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
		void Predict_LeapFrog( ::Oyster::Math::Float3 &outDeltaPos, ::Oyster::Math::Float3 &outDeltaAxis, const ::Oyster::Math::Float3 &actingLinearImpulse, const ::Oyster::Math::Float3 &actingAngularImpulse, ::Oyster::Math::Float deltaTime );

		void Move( const ::Oyster::Math::Float3 &deltaPos, const ::Oyster::Math::Float3 &deltaAxis );
		void ApplyImpulse( const ::Oyster::Math::Float3 &worldJ, const ::Oyster::Math::Float3 &atWorldPos );
		void ApplyImpulse_Linear( const ::Oyster::Math::Float3 &worldJ );
		void ApplyImpulse_Angular( const ::Oyster::Math::Float3 &worldJ );
		void ApplyForce( const ::Oyster::Math::Float3 &worldF, ::Oyster::Math::Float updateFrameLength );
		void ApplyForce( const ::Oyster::Math::Float3 &worldF, ::Oyster::Math::Float updateFrameLength, const ::Oyster::Math::Float3 &atWorldPos );

		// GET METHODS ////////////////////////////////

		const ::Oyster::Physics3D::MomentOfInertia & GetMomentOfInertia() const;
			  ::Oyster::Math::Float					 GetMass() const;
		const ::Oyster::Math::Quaternion &			 GetRotationQuaternion() const;
			  ::Oyster::Math::Float4x4				 GetRotationMatrix() const;
			  ::Oyster::Math::Float4x4				 GetOrientation() const;
			  ::Oyster::Math::Float4x4				 GetView() const;
 			  ::Oyster::Math::Float4x4				 GetToWorldMatrix() const;
			  ::Oyster::Math::Float4x4				 GetToLocalMatrix() const;
			  ::Oyster::Math::Float3				 GetSize() const;
			  ::Oyster::Math::Float3				 GetVelocity_Linear() const;
			  ::Oyster::Math::Float3				 GetVelocity_Angular() const;
			  ::Oyster::Math::Float3				 GetLinearMomentum( const ::Oyster::Math::Float3 &atWorldPos ) const;

		// SET METHODS ////////////////////////////////

		void SetMomentOfInertia_KeepVelocity( const ::Oyster::Physics3D::MomentOfInertia &localTensorI );
		void SetMomentOfInertia_KeepMomentum( const ::Oyster::Physics3D::MomentOfInertia &localTensorI );
		void SetMass_KeepVelocity( const ::Oyster::Math::Float &m );
		void SetMass_KeepMomentum( const ::Oyster::Math::Float &m );

		//void SetOrientation( const ::Oyster::Math::Float4x4 &o );
		void SetRotation( const ::Oyster::Math::Quaternion &quaternion );
		void SetSize( const ::Oyster::Math::Float3 &widthHeight );

		void SetMomentum_Linear( const ::Oyster::Math::Float3 &worldG, const ::Oyster::Math::Float3 &atWorldPos );

		void SetVelocity_Linear( const ::Oyster::Math::Float3 &worldV );
		void SetVelocity_Linear( const ::Oyster::Math::Float3 &worldV, const ::Oyster::Math::Float3 &atWorldPos );
		void SetVelocity_Angular( const ::Oyster::Math::Float3 &worldW );
		
		void SetImpulse_Linear( const ::Oyster::Math::Float3 &worldJ, const ::Oyster::Math::Float3 &atWorldPos );
		//void SetForce( const ::Oyster::Math::Float3 &worldF, ::Oyster::Math::Float updateFrameLength );
		//void SetForce( const ::Oyster::Math::Float3 &worldF, ::Oyster::Math::Float updateFrameLength, const ::Oyster::Math::Float3 &atWorldPos );

	private:
		::Oyster::Math::Float mass;						//!< m (kg)
		//::Oyster::Math::Float4x4 momentOfInertiaTensor;	//!< I (Nm*s) Tensor matrix ( only need to be 3x3 matrix, but is 4x4 for future hardware acceleration )	(localValue)
		::Oyster::Physics3D::MomentOfInertia momentOfInertiaTensor;
		//::Oyster::Math::Quaternion rotation;			//!< RotationAxis of the body.
	};
} }

#include "RigidBody_Inline.h"

#endif
