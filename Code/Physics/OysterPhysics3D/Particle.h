/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#ifndef OYSTER_PHYSICS_3D_PARTICLE_H
#define OYSTER_PHYSICS_3D_PARTICLE_H

#include "OysterMath.h"
#include "OysterCollision3D.h"
#include "OysterPhysics3D.h"

namespace Oyster { namespace Physics3D
{
	struct Particle
	{	/// A struct of a simple particle.
	public:
		::Oyster::Collision3D::Sphere sphere;
		::Oyster::Math::Float3 linearMomentum,	/// The linear momentum G (kg*m/s).
							   impulseForceSum;	/// The impulse force F (N) that will be consumed each update.

		Particle( const ::Oyster::Collision3D::Sphere &sphere = ::Oyster::Collision3D::Sphere(), ::Oyster::Math::Float mass = 1.0f );

		Particle & operator = ( const Particle &particle );

		void Update_LeapFrog( ::Oyster::Math::Float deltaTime );
		void ApplyImpulseForce( const ::Oyster::Math::Float3 &f );
		void ApplyLinearImpulseAcceleration( const ::Oyster::Math::Float3 &a );

		// ACCESS METHODS /////////////////////////////

		::Oyster::Math::Float3 &		AccessCenter();
		const ::Oyster::Math::Float3 &	AccessCenter() const;
		::Oyster::Math::Float &			AccessRadius();
		const ::Oyster::Math::Float &	AccessRadius() const;

		// GET METHODS ////////////////////////////////

		const ::Oyster::Math::Float &	GetMass() const;
		const ::Oyster::Math::Float3 &	GetCenter() const;
		const ::Oyster::Math::Float &	GetRadius() const;
		
		const ::Oyster::Math::Float3 &	GetImpulseForce() const;
		const ::Oyster::Math::Float3 &	GetLinearMomentum() const;
		::Oyster::Math::Float3			GetLinearImpulseAcceleration() const;
		::Oyster::Math::Float3			GetLinearVelocity() const;

		// SET METHODS ////////////////////////////////

		void SetMass_KeepVelocity( ::Oyster::Math::Float m );
		void SetMass_KeepMomentum( ::Oyster::Math::Float m );
		void SetCenter( const ::Oyster::Math::Float3 &p );
		void SetRadius( ::Oyster::Math::Float r );

		void SetImpulseForce( const ::Oyster::Math::Float3 &f );
		void SetLinearMomentum( const ::Oyster::Math::Float3 &g );
		void SetLinearImpulseAcceleration( const ::Oyster::Math::Float3 &a );
		void SetLinearVelocity( const ::Oyster::Math::Float3 &v );

	private:
		::Oyster::Math::Float mass; /// m (kg)
	};
} }

#endif