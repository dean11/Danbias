/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#ifndef OYSTER_PHYSICS_3D_H
#define OYSTER_PHYSICS_3D_H

#include "OysterMath.h"

namespace Oyster { namespace Physics3D
{ /// Library of 3D physics related components, alghorithms and formulas
	namespace Formula
	{ /// Library of 3D physics related formulas

		/******************************************************************
		 * Returns the linear kinetic energy of a mass in motion.
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float LinearKineticEnergy( const ::Oyster::Math::Float &mass, const ::Oyster::Math::Float3 &linearVelocity )
		{ return (0.5f * mass) * linearVelocity.Dot( linearVelocity ); }

		/******************************************************************
		 * Returns the angular kinetic energy of a mass in rotation.
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float AngularKineticEnergy( const ::Oyster::Math::Float &momentOfInertia, const ::Oyster::Math::Float3 &angularVelocity )
		{ return (0.5f * momentOfInertia) * angularVelocity.Dot( angularVelocity ); }

		/******************************************************************
		 * Returns the linear momentum of a mass in motion.
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float3 LinearMomentum( const ::Oyster::Math::Float &mass, const ::Oyster::Math::Float3 &linearVelocity )
		{ return mass * linearVelocity; }

		/******************************************************************
		 * Returns the linear velocity of a mass with momentum.
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float3 LinearVelocity( const ::Oyster::Math::Float &mass, const ::Oyster::Math::Float3 &linearMomentum )
		{ return linearMomentum / mass; }

		/******************************************************************
		 * Returns the local angular momentum of a mass in rotation.
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float3 AngularMomentum( const ::Oyster::Math::Float &momentOfInertia, const ::Oyster::Math::Float3 &angularVelocity )
		{ return momentOfInertia * angularVelocity; }

		/******************************************************************
		 * Returns the local tangential momentum at localPos, of a mass in rotation.
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float3 & TangentialLinearMomentum( const ::Oyster::Math::Float &localAngularMomentum, const ::Oyster::Math::Float3 &localPosition, ::Oyster::Math::Float3 &targetMem = ::Oyster::Math::Float3() )
		{ return targetMem = localAngularMomentum * ::Oyster::Math::Float3( -localPosition.y, localPosition.x ); }

		/******************************************************************
		 * Returns the local tangential momentum at localPos, of a mass in rotation.
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float3 & TangentialLinearMomentum( const ::Oyster::Math::Float &momentOfInertia, const ::Oyster::Math::Float3 &angularVelocity, const ::Oyster::Math::Float3 &localPosition, ::Oyster::Math::Float3 &targetMem = ::Oyster::Math::Float3() )
		{ return targetMem = AngularMomentum( momentOfInertia, angularVelocity ).Cross( localPosition ); }

		/******************************************************************
		 * Returns the local angular velocity of a mass in rotation.
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float3 AngularVelocity( const ::Oyster::Math::Float &momentOfInertia, const ::Oyster::Math::Float3 &angularMomentum )
		{ return angularMomentum / momentOfInertia; }

		/******************************************************************
		 * Returns the local tangential velocity at localPos, of a mass in rotation.
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float3 & TangentialLinearVelocity( const ::Oyster::Math::Float3 &angularVelocity, const ::Oyster::Math::Float3 &localPosition, ::Oyster::Math::Float3 &targetMem = ::Oyster::Math::Float3() )
		{ return targetMem = angularVelocity.Cross( localPosition ); }

		/******************************************************************
		 * 
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float3 LinearImpulseAcceleration( ::Oyster::Math::Float mass, const ::Oyster::Math::Float3 &impulseForce )
		{ return impulseForce / mass; }
		
		/******************************************************************
		 * 
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float3 ImpulseForce( ::Oyster::Math::Float mass, const ::Oyster::Math::Float3 &linearImpulseAcceleration )
		{ return linearImpulseAcceleration * mass; }

		namespace MomentOfInertia
		{ /// Library of Formulas to calculate moment of inerta for simple shapes
			/** @todo TODO: add MomentOfInertia formulas */
		}
	}
} }

#include "Particle.h"
#include "RigidBody.h"
#include "Spring.h"

#endif