#ifndef PHYSICS_FORMULA_IMPL_H
#define PHYSICS_FORMULA_IMPL_H

#include "PhysicsFormula.h"
#include "OysterPhysics3D.h"

namespace Oyster { namespace Physics { namespace Formula
{
	namespace MomentOfInertia
	{
		inline ::Oyster::Math::Float4x4 CreateSphereMatrix( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float radius )
		{
			return ::Oyster::Physics3D::Formula::MomentOfInertia::Sphere(mass, radius);
		}

		inline ::Oyster::Math::Float4x4 CreateHollowSphereMatrix( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float radius )
		{
			return ::Oyster::Physics3D::Formula::MomentOfInertia::HollowSphere(mass, radius);
		}

		inline ::Oyster::Math::Float4x4 CreateCuboidMatrix( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float height, const ::Oyster::Math::Float width, const ::Oyster::Math::Float depth )
		{
			return ::Oyster::Physics3D::Formula::MomentOfInertia::Cuboid(mass, height, width, depth);
		}

		inline ::Oyster::Math::Float4x4 CreateCylinderMatrix( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float height, const ::Oyster::Math::Float radius )
		{
			return ::Oyster::Physics3D::Formula::MomentOfInertia::Cylinder(mass, height, radius);
		}

		inline ::Oyster::Math::Float4x4 CreateRodMatrix( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float length )
		{
			return ::Oyster::Physics3D::Formula::MomentOfInertia::RodCenter(mass, length);
		}
	}

	namespace CollisionResponse
	{
		inline ::Oyster::Math::Float Bounce( ::Oyster::Math::Float e, ::Oyster::Math::Float mA, ::Oyster::Math::Float gA, ::Oyster::Math::Float mB, ::Oyster::Math::Float gB )
		{
			//return (e + 1) * (mB*gA - mA*gB) / (mA + mB);
			return (e + 1) * (mA*gB - mB*gA) / (mA + mB);
		}

		inline ::Oyster::Math::Float4 Friction( ::Oyster::Math::Float i, ::Oyster::Math::Float4 iN, ::Oyster::Math::Float4 momA, ::Oyster::Math::Float sFA, ::Oyster::Math::Float dFA, ::Oyster::Math::Float mA, ::Oyster::Math::Float4 momB, ::Oyster::Math::Float sFB, ::Oyster::Math::Float dFB, ::Oyster::Math::Float mB )
		{
			// FRICTION
			// Relative momentum after normal impulse
			::Oyster::Math::Float4 relativeMomentum = momB - momA;

			::Oyster::Math::Float4 tanFriction = relativeMomentum - relativeMomentum.Dot( iN ) * iN;

			if( tanFriction.Dot(tanFriction) > 0.0f )
			{ // no friction if moving directly into surface, or not at all.
				tanFriction.Normalize();

				::Oyster::Math::Float magnitudeFriction = -relativeMomentum.Dot( tanFriction );
				magnitudeFriction = magnitudeFriction * mA * mB / ( mA + mB );

				::Oyster::Math::Float mu = 0.5f * ( sFA + sFB );
 
				::Oyster::Math::Float4 frictionImpulse;
				if( abs(magnitudeFriction) < i * mu )
				{
					frictionImpulse = magnitudeFriction * tanFriction;
				}
				else
				{
					::Oyster::Math::Float dynamicFriction = 0.5f * ( dFA + dFB );
					frictionImpulse = ( -i * dynamicFriction ) * tanFriction;
				}
 
				return ( 1 / mA ) * frictionImpulse;
			}
			else
				return ::Oyster::Math::Float4::null;
		}
	}

} } }

#endif