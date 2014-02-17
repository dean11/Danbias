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
		 * Returns the linear momentum of a mass in motion.
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float4 LinearMomentum( const ::Oyster::Math::Float &mass, const ::Oyster::Math::Float4 &linearVelocity )
		{
			return linearVelocity * mass;
		}

		/******************************************************************
		 * Returns the linear momentum of a mass in motion.
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float4 LinearVelocity( const ::Oyster::Math::Float &mass, const ::Oyster::Math::Float4 &linearMomentum )
		{
			return linearMomentum / mass;
		}

		/******************************************************************
		 * Returns the world angular momentum of a mass in rotation.
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float4 AngularMomentum( const ::Oyster::Math::Float4 linearMomentum, const ::Oyster::Math::Float4 &worldOffset )
		{
			return ::Oyster::Math::Float4( worldOffset.xyz.Cross(linearMomentum.xyz), 0.0f );
		}

		/******************************************************************
		 * Returns the world angular momentum of a mass in rotation.
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float4 AngularMomentum( const ::Oyster::Math::Float4x4 &worldMomentOfInertia, const ::Oyster::Math::Float4 &angularVelocity )
		{
			return worldMomentOfInertia * angularVelocity;
		}

		/******************************************************************
		 * Returns the world angular velocity of a mass in rotation.
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float4 AngularVelocity( const ::Oyster::Math::Float4x4 &worldMomentOfInertiaInversed, const ::Oyster::Math::Float4 &angularMomentum )
		{
			return worldMomentOfInertiaInversed * angularMomentum;
		}

		/******************************************************************
		 * Returns the world tangential momentum at worldPos, of a mass in rotation.
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float4 TangentialLinearMomentum( const ::Oyster::Math::Float4 &angularMomentum, const ::Oyster::Math::Float4 &worldOffset )
		{
			return ::Oyster::Math::Float4( angularMomentum.xyz.Cross(worldOffset.xyz), 0.0f ) /= worldOffset.Dot( worldOffset );
		}

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
		 * H = r x G
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float3 AngularMomentum( const ::Oyster::Math::Float3 linearMomentum, const ::Oyster::Math::Float3 &worldOffset )
		{
			return worldOffset.Cross( linearMomentum );
		}

		/******************************************************************
		 * Returns the world tangential momentum at worldPos, of a mass in rotation.
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float3 TangentialLinearMomentum( const ::Oyster::Math::Float3 &angularMomentum, const ::Oyster::Math::Float3 &worldOffset )
		{
			return angularMomentum.Cross( worldOffset ) /= worldOffset.Dot( worldOffset );
		}

		/******************************************************************
		 * Returns the world tangential momentum at worldPos, of a mass in rotation.
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float3 TangentialLinearMomentum( const ::Oyster::Math::Float4x4 &momentOfInertia, const ::Oyster::Math::Float3 &angularVelocity, const ::Oyster::Math::Float3 &worldOffset )
		{
			return TangentialLinearMomentum( AngularMomentum(momentOfInertia, angularVelocity), worldOffset ) /= worldOffset.Dot( worldOffset );
		}

		/******************************************************************
		 * Returns the world impulse force at worldPos, of a mass in angular acceleration.
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float3 TangentialImpulseForce( const ::Oyster::Math::Float3 &impulseTorque, const ::Oyster::Math::Float3 &worldOffset )
		{
			return impulseTorque.Cross( worldOffset ) /= worldOffset.Dot( worldOffset );
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
			return worldOffset.Cross( linearImpulseAcceleration );
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
		 * Returns the world angular velocity of a mass in rotation.
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float3 AngularVelocity( const ::Oyster::Math::Float3 &linearVelocity, const ::Oyster::Math::Float3 &worldOffset )
		{
			return worldOffset.Cross( linearVelocity );
		}

		/******************************************************************
		 * Returns the world angular velocity of a mass in rotation.
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float4 AngularVelocity( const ::Oyster::Math::Float4 &linearVelocity, const ::Oyster::Math::Float4 &worldOffset )
		{
			return ::Oyster::Math::Float4( worldOffset.xyz.Cross( linearVelocity.xyz ), 0.0f );
		}

		/******************************************************************
		 * Returns the world tangential velocity at worldPos, of a mass in rotation.
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float3 TangentialLinearVelocity( const ::Oyster::Math::Float3 &angularVelocity, const ::Oyster::Math::Float3 &worldOffset )
		{
			return angularVelocity.Cross( worldOffset ) /= worldOffset.Dot( worldOffset );
		}

		/******************************************************************
		 * Returns the world tangential velocity at worldPos, of a mass in rotation.
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float3 TangentialLinearVelocity( const ::Oyster::Math::Float4x4 &momentOfInertiaInversed, const ::Oyster::Math::Float3 &angularMomentum, const ::Oyster::Math::Float3 &worldOffset )
		{
			return TangentialLinearVelocity( AngularVelocity(momentOfInertiaInversed, angularMomentum), worldOffset );
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
			return worldOffset.Cross( impulseForce );
		}

		/******************************************************************
		 * 
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float4 ImpulseTorque( const ::Oyster::Math::Float4 & impulseForce, const ::Oyster::Math::Float4 &worldOffset )
		{
			return ::Oyster::Math::Float4( worldOffset.xyz.Cross(impulseForce.xyz), 0.0f );
		}

		/******************************************************************
		 * T = I*a
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float3 ImpulseTorque( const ::Oyster::Math::Float4x4 & momentOfInertia, const ::Oyster::Math::Float3 &angularImpulseAcceleration )
		{
			return ( momentOfInertia * ::Oyster::Math::Float4(angularImpulseAcceleration, 0.0f) ).xyz;
		}

		/******************************************************************
		 * T = I*a
		 * @todo TODO: improve doc
		 ******************************************************************/
		inline ::Oyster::Math::Float4 ImpulseTorque( const ::Oyster::Math::Float4x4 & momentOfInertia, const ::Oyster::Math::Float4 &angularImpulseAcceleration )
		{
			return momentOfInertia * angularImpulseAcceleration;
		}

		/******************************************************************
		 * @todo TODO: doc
		 ******************************************************************/
		inline ::Oyster::Math::Float ForceField( ::Oyster::Math::Float g, ::Oyster::Math::Float massA, ::Oyster::Math::Float massB, ::Oyster::Math::Float radiusSquared )
		{
			return g * massB / radiusSquared;
		}

		/******************************************************************
		 * @todo TODO: doc
		 ******************************************************************/
		inline ::Oyster::Math::Float ForceField( ::Oyster::Math::Float g, ::Oyster::Math::Float massA, ::Oyster::Math::Float massB, const ::Oyster::Math::Float4 &deltaPos )
		{
			return g * massA * massB / deltaPos.Dot( deltaPos );
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

			inline ::Oyster::Math::Float CalculateRodCenter( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float length )
			{
				return (1.0f/12.0f)*mass*(length*length);
			}

			inline ::Oyster::Math::Float4x4 RodCenter( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float length )
			{
				::Oyster::Math::Float4x4 inertia = ::Oyster::Math::Float4x4::identity;
				inertia.m[0][0] = ::Oyster::Physics3D::Formula::MomentOfInertia::CalculateRodCenter( mass , length );
				inertia.m[1][1] = inertia.m[0][0];
				inertia.m[2][2] = inertia.m[0][0];

				return inertia;
			}

			inline ::Oyster::Math::Float CalculateCylinderXY( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float height, const ::Oyster::Math::Float radius )
			{
				return (1.0f/12.0f)*mass*(3.0f*radius*radius + height*height);
			}

			inline ::Oyster::Math::Float CalculateCylinderZ( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float radius )
			{
				return 0.5f*mass*(radius*radius);
			}
			
			inline ::Oyster::Math::Float4x4 Cylinder( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float height, const ::Oyster::Math::Float radius )
			{
				::Oyster::Math::Float4x4 inertia = ::Oyster::Math::Float4x4::identity;
				inertia.m[0][0] = ::Oyster::Physics3D::Formula::MomentOfInertia::CalculateCylinderXY( mass , height, radius );
				inertia.m[1][1] = inertia.m[0][0];
				inertia.m[2][2] = ::Oyster::Physics3D::Formula::MomentOfInertia::CalculateCylinderZ( mass , radius );

				return inertia;
			}
		}
	}
} }

#include "Particle.h"
#include "RigidBody.h"
#include "Spring.h"

#endif