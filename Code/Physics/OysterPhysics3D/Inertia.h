/********************************************************************
 * Created by Dan Andersson & Robin Engman 2014
 ********************************************************************/

#ifndef OYSTER_PHYSICS_3D_INERTIA_H
#define OYSTER_PHYSICS_3D_INERTIA_H

#include "OysterMath.h"

namespace Oyster { namespace Physics3D
{
	struct MomentOfInertia
	{
		::Oyster::Math::Quaternion rotation;
		::Oyster::Math::Float3 magnitude;

		MomentOfInertia();
		MomentOfInertia( const ::Oyster::Math::Quaternion &r, const ::Oyster::Math::Float3 &m );

		MomentOfInertia & operator = ( const MomentOfInertia &i );

		::Oyster::Math::Float3   CalculateAngularVelocity( const ::Oyster::Math::Quaternion &externR, const ::Oyster::Math::Float3 &angularMomentum ) const;
		::Oyster::Math::Float3 & CalculateAngularVelocity( const ::Oyster::Math::Quaternion &externR, const ::Oyster::Math::Float3 &angularMomentum, ::Oyster::Math::Float3 &targetMem ) const;

		::Oyster::Math::Float3   CalculateAngularMomentum( const ::Oyster::Math::Quaternion &externR, const ::Oyster::Math::Float3 &angularVelocity ) const;
		::Oyster::Math::Float3 & CalculateAngularMomentum( const ::Oyster::Math::Quaternion &externR, const ::Oyster::Math::Float3 &angularVelocity, ::Oyster::Math::Float3 &targetMem ) const;

		static ::Oyster::Math::Float CalculateSphere( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float radius );
		static ::Oyster::Math::Float CalculateHollowSphere( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float radius );
		static ::Oyster::Math::Float CalculateCuboidX( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float height, const ::Oyster::Math::Float depth );
		static ::Oyster::Math::Float CalculateCuboidY( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float width, const ::Oyster::Math::Float depth );
		static ::Oyster::Math::Float CalculateCuboidZ( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float width, const ::Oyster::Math::Float height );
		static ::Oyster::Math::Float CalculateRodCenter( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float length );
		static ::Oyster::Math::Float CalculateCylinderXY( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float height, const ::Oyster::Math::Float radius );
		static ::Oyster::Math::Float CalculateCylinderZ( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float radius );

		static MomentOfInertia Sphere( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float radius );
		static MomentOfInertia HollowSphere( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float radius );
		static MomentOfInertia Cuboid( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float height, const ::Oyster::Math::Float width, const ::Oyster::Math::Float depth );
		static MomentOfInertia RodCenter( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float length );
		static MomentOfInertia Cylinder( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float height, const ::Oyster::Math::Float radius );
	};

	inline ::Oyster::Math::Float MomentOfInertia::CalculateSphere( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float radius )
	{
		return (2.0f / 5.0f) * mass * radius * radius;
	}

	inline ::Oyster::Math::Float MomentOfInertia::CalculateHollowSphere( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float radius )
	{
		return (2.0f / 3.0f) * mass * radius * radius;
	}

	inline ::Oyster::Math::Float MomentOfInertia::CalculateCuboidX( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float height, const ::Oyster::Math::Float depth )
	{
		return (1.0f / 12.0f) * mass * (height * height + depth * depth);
	}

	inline ::Oyster::Math::Float MomentOfInertia::CalculateCuboidY( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float width, const ::Oyster::Math::Float depth )
	{
		return (1.0f / 12.0f) * mass * (width * width + depth * depth);
	}

	inline ::Oyster::Math::Float MomentOfInertia::CalculateCuboidZ( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float width, const ::Oyster::Math::Float height )
	{
		return (1.0f / 12.0f) * mass * (height * height + width * width);
	}

	inline ::Oyster::Math::Float MomentOfInertia::CalculateRodCenter( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float length )
	{
		return (1.0f / 12.0f) * mass * length * length;
	}

	inline ::Oyster::Math::Float MomentOfInertia::CalculateCylinderXY( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float height, const ::Oyster::Math::Float radius )
	{
		return (1.0f / 12.0f) * mass * (3.0f * radius * radius + height * height);
	}

	inline ::Oyster::Math::Float MomentOfInertia::CalculateCylinderZ( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float radius )
	{
		return 0.5f * mass * radius * radius;
	}

	inline MomentOfInertia MomentOfInertia::Sphere( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float radius )
	{
		return MomentOfInertia( ::Oyster::Math::Quaternion::identity,
								::Oyster::Math::Float3(MomentOfInertia::CalculateSphere(mass, radius)) );
	}

	inline MomentOfInertia MomentOfInertia::HollowSphere( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float radius )
	{
		return MomentOfInertia( ::Oyster::Math::Quaternion::identity,
								::Oyster::Math::Float3(MomentOfInertia::CalculateHollowSphere(mass, radius)) );
	}

	inline MomentOfInertia MomentOfInertia::Cuboid( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float height, const ::Oyster::Math::Float width, const ::Oyster::Math::Float depth )
	{
		return MomentOfInertia( ::Oyster::Math::Quaternion::identity,
								::Oyster::Math::Float3(MomentOfInertia::CalculateCuboidX(mass, height, depth),
													   MomentOfInertia::CalculateCuboidY(mass, width,  depth),
													   MomentOfInertia::CalculateCuboidZ(mass, height, width)) );
	}

	inline MomentOfInertia MomentOfInertia::RodCenter( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float length )
	{
		return MomentOfInertia( ::Oyster::Math::Quaternion::identity,
								::Oyster::Math::Float3(MomentOfInertia::CalculateRodCenter(mass , length)) );
	}

	inline MomentOfInertia MomentOfInertia::Cylinder( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float height, const ::Oyster::Math::Float radius )
	{
		::Oyster::Math::Float cylinderXY = MomentOfInertia::CalculateCylinderXY( mass , height, radius );
		return MomentOfInertia( ::Oyster::Math::Quaternion::identity,
								::Oyster::Math::Float3(cylinderXY, cylinderXY,
													   MomentOfInertia::CalculateCylinderZ(mass, radius)) );
	}
} }

#endif