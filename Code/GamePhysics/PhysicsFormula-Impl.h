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
		inline ::Oyster::Math::Float Impulse( ::Oyster::Math::Float e, ::Oyster::Math::Float mA, ::Oyster::Math::Float gA, ::Oyster::Math::Float mB, ::Oyster::Math::Float gB )
		{
			return (e+1) * (mB*gA - mA*gB) / (mA + mB);
		}
	}
} } }

#endif