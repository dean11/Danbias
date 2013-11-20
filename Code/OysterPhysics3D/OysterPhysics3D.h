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
		{
			return (0.5f * mass) * linearVelocity.Dot( linearVelocity );
		}

		/******************************************************************
		 * Returns the angular kinetic energy of a mass in rotation.
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float AngularKineticEnergy( const ::Oyster::Math::Float &momentOfInertia, const ::Oyster::Math::Float3 &angularVelocity )
		{
			return (0.5f * momentOfInertia) * angularVelocity.Dot( angularVelocity );
		}

		/******************************************************************
		 * Returns the linear momentum of a mass in motion.
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float3 LinearMomentum( const ::Oyster::Math::Float &mass, const ::Oyster::Math::Float3 &linearVelocity )
		{
			return mass * linearVelocity;
		}

		/******************************************************************
		 * Returns the linear velocity of a mass with momentum.
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float3 LinearVelocity( const ::Oyster::Math::Float &mass, const ::Oyster::Math::Float3 &linearMomentum )
		{
			return linearMomentum / mass;
		}

		/******************************************************************
		 * Returns the world angular momentum of a mass in rotation.
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float3 AngularMomentum( const ::Oyster::Math::Float4x4 &momentOfInertia, const ::Oyster::Math::Float3 &angularVelocity )
		{
			return ( momentOfInertia * ::Oyster::Math::Float4(angularVelocity, 0.0f) ).xyz;
		}

		/******************************************************************
		 * Returns the world angular momentum of a mass in rotation.
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float3 AngularMomentum( const ::Oyster::Math::Float3 linearMomentum, const ::Oyster::Math::Float3 &worldOffset )
		{
			return offset.Cross( linearMomentum );
		}

		/******************************************************************
		 * Returns the world tangential momentum at worldPos, of a mass in rotation.
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float3 TangentialLinearMomentum( const ::Oyster::Math::Float3 &angularMomentum, const ::Oyster::Math::Float3 &worldOffset )
		{
			return angularMomentum.Cross( worldOffset );
		}

		/******************************************************************
		 * Returns the world tangential momentum at worldPos, of a mass in rotation.
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float3 TangentialLinearMomentum( const ::Oyster::Math::Float4x4 &momentOfInertia, const ::Oyster::Math::Float3 &angularVelocity, const ::Oyster::Math::Float3 &worldOffset )
		{
			return TangentialLinearMomentum( AngularMomentum(momentOfInertia, angularVelocity), worldOffset );
		}

		/******************************************************************
		 * Returns the world impulse force at worldPos, of a mass in angular acceleration.
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float3 TangentialImpulseForce( const ::Oyster::Math::Float3 &impulseTorque, const ::Oyster::Math::Float3 &worldOffset )
		{
			return impulseTorque.Cross( worldOffset );
		}

		/******************************************************************
		 * 
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float3 AngularImpulseAcceleration( const ::Oyster::Math::Float4x4 &momentOfInertiaInversed, const ::Oyster::Math::Float3 &impulseTorque )
		{
			return ( momentOfInertiaInversed * ::Oyster::Math::Float4( impulseTorque, 0.0f ) ).xyz;
		}

		/******************************************************************
		 * 
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float3 AngularImpulseAcceleration( const ::Oyster::Math::Float3 &linearImpulseAcceleration, const ::Oyster::Math::Float3 &worldOffset )
		{
			return offset.Cross( linearImpulseAcceleration );
		}

		/******************************************************************
		 * Returns the world impulse acceleration at ( worldOffset = worldPos - body's center of gravity ), of a mass in angular acceleration.
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float3 TangentialImpulseAcceleration( const ::Oyster::Math::Float4x4 &worldMomentOfInertiaInversed, const ::Oyster::Math::Float3 &worldImpulseTorque, const ::Oyster::Math::Float3 &worldOffset )
		{
			return AngularImpulseAcceleration( worldMomentOfInertiaInversed, worldImpulseTorque ).Cross( worldOffset );
		}

		/******************************************************************
		 * Returns the world angular velocity of a mass in rotation.
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float3 AngularVelocity( const ::Oyster::Math::Float4x4 &momentOfInertiaInversed, const ::Oyster::Math::Float3 &angularMomentum )
		{
			return ( momentOfInertiaInversed * ::Oyster::Math::Float4( angularMomentum, 0.0f ) ).xyz;
		}

		/******************************************************************
		 * Returns the world tangential velocity at worldPos, of a mass in rotation.
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float3 TangentialLinearVelocity( const ::Oyster::Math::Float3 &angularVelocity, const ::Oyster::Math::Float3 &worldOffset )
		{
			return angularVelocity.Cross( offset );
		}

		/******************************************************************
		 * Returns the world tangential velocity at worldPos, of a mass in rotation.
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float3 TangentialLinearVelocity( const ::Oyster::Math::Float4x4 &momentOfInertiaInversed, const ::Oyster::Math::Float3 &angularMomentum, const ::Oyster::Math::Float3 &worldOffset )
		{
			return TangentialLinearVelocity( AngularVelocity(momentOfInertiaInversed, angularMomentum), offset );
		}

		/******************************************************************
		 * 
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float3 LinearImpulseAcceleration( ::Oyster::Math::Float mass, const ::Oyster::Math::Float3 &impulseForce )
		{
			return impulseForce / mass;
		}
		
		/******************************************************************
		 * 
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float3 ImpulseForce( ::Oyster::Math::Float mass, const ::Oyster::Math::Float3 &linearImpulseAcceleration )
		{
			return linearImpulseAcceleration * mass;
		}

		/******************************************************************
		 * 
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float3 ImpulseTorque( const ::Oyster::Math::Float3 & impulseForce, const ::Oyster::Math::Float3 &worldOffset )
		{
			return offset.Cross( impulseForce );
		}

		/******************************************************************
		 * T = I*a
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float3 ImpulseTorque( const ::Oyster::Math::Float4x4 & momentOfInertia, const ::Oyster::Math::Float3 &angularImpulseAcceleration )
		{
			return ( momentOfInertia * ::Oyster::Math::Float4(angularImpulseAcceleration, 0.0f) ).xyz;
		}

		namespace MomentOfInertia
		{ /// Library of Formulas to calculate moment of inerta for simple shapes
			/** @todo TODO: add MomentOfInertia tensor formulas */
			inline ::Oyster::Math::Float CalculateSphere( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float radius )
			{
				return (2.0f/5.0f)*mass*radius*radius;
			}

			inline ::Oyster::Math::Float4x4 Sphere( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float radius )
			{
				::Oyster::Math::Float4x4 inertia = ::Oyster::Math::Float4x4::identity;
				inertia.m[0][0] = ::Oyster::Physics3D::Formula::MomentOfInertia::CalculateSphere( mass , radius );
				inertia.m[1][1] = inertia.m[0][0];
				inertia.m[2][2] = inertia.m[0][0];

				return inertia;
			}

			inline ::Oyster::Math::Float CalculateHollowSphere( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float radius )
			{
				return (2.0f/3.0f)*mass*radius*radius;
			}

			inline ::Oyster::Math::Float4x4 HollowSphere( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float radius )
			{
				::Oyster::Math::Float4x4 inertia = ::Oyster::Math::Float4x4::identity;
				inertia.m[0][0] = ::Oyster::Physics3D::Formula::MomentOfInertia::CalculateHollowSphere( mass, radius );
				inertia.m[1][1] = inertia.m[0][0];
				inertia.m[2][2] = inertia.m[0][0];

				return inertia;
			}

			inline ::Oyster::Math::Float CalculateCuboidX( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float height, const ::Oyster::Math::Float depth )
			{
				return (1.0f/12.0f)*mass*(height*height + depth*depth);
			}

			inline ::Oyster::Math::Float CalculateCuboidY( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float width, const ::Oyster::Math::Float depth )
			{
				return (1.0f/12.0f)*mass*(width*width + depth*depth);
			}

			inline ::Oyster::Math::Float CalculateCuboidZ( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float width, const ::Oyster::Math::Float height )
			{
				return (1.0f/12.0f)*mass*(height*height + width*width);
			}

			inline ::Oyster::Math::Float4x4 Cuboid( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float height, const ::Oyster::Math::Float width, const ::Oyster::Math::Float depth )
			{
				::Oyster::Math::Float4x4 inertia = ::Oyster::Math::Float4x4::identity;
				inertia.m[0][0] = ::Oyster::Physics3D::Formula::MomentOfInertia::CalculateCuboidX( mass , height, depth );
				inertia.m[1][1] = ::Oyster::Physics3D::Formula::MomentOfInertia::CalculateCuboidY( mass , width, depth  );
				inertia.m[2][2] = ::Oyster::Physics3D::Formula::MomentOfInertia::CalculateCuboidZ( mass , height, width );

				return inertia;
			}
		
		}
	}
} }

#include "Particle.h"
#include "RigidBody.h"
#include "Spring.h"

#endif