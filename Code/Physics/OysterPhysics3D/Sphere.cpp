#include "Sphere.h"
#include "OysterCollision3D.h"

using namespace ::Oyster::Collision3D;
using namespace ::Oyster::Math;

Sphere::Sphere( ) : ICollideable(Type_sphere)
{
	this->center = Float4::standard_unit_w;
	this->radius = 0.0f;
}

Sphere::Sphere( const Float3 &p, const Float &r ) : ICollideable(Type_sphere)
{
	this->center = Float4( p, 1.0f );
	this->radius = r;
}

Sphere::Sphere( const Float4 &p, const Float &r ) : ICollideable(Type_sphere)
{
	this->center = p;
	this->radius = r;
}

Sphere::~Sphere( ) {}

Sphere & Sphere::operator = ( const Sphere &sphere )
{
	this->center = sphere.center;
	this->radius = sphere.radius;
	return *this;
}

::Utility::DynamicMemory::UniquePointer<ICollideable> Sphere::Clone( ) const
{
	return ::Utility::DynamicMemory::UniquePointer<ICollideable>( new Sphere(*this) );
}

bool Sphere::Intersects( const ICollideable &target ) const
{
	switch( target.type )
	{
	case Type_universe:			return true;
	case Type_point:			return Utility::Intersect( *this, (const Point&)target );
	case Type_ray:				return Utility::Intersect( *this, (const Ray&)target, ((const Ray&)target).collisionDistance );
	case Type_sphere:			return Utility::Intersect( *this, (const Sphere&)target );
	case Type_plane:			return Utility::Intersect( (const Plane&)target, *this );
	//case Type_triangle:			return false; // TODO: 
	case Type_box_axis_aligned:	return Utility::Intersect( (const BoxAxisAligned&)target, *this );
	case Type_box:				return Utility::Intersect( (const Box&)target, *this );
	//case Type_frustrum:			return false; // TODO: 
	default:					return false;
	}
}

bool Sphere::Intersects( const ICollideable &target, Float4 &worldPointOfContact ) const
{
	switch( target.type )
	{
	case Type_universe:
		worldPointOfContact = this->center;
		return true;
	case Type_point:			return Utility::Intersect( *this, (const Point&)target, worldPointOfContact );
	case Type_ray:				return Utility::Intersect( *this, (const Ray&)target, ((const Ray&)target).collisionDistance, worldPointOfContact );
	case Type_sphere:			return Utility::Intersect( *this, (const Sphere&)target, worldPointOfContact );
	case Type_plane:			return Utility::Intersect( (const Plane&)target, *this, worldPointOfContact );
	//case Type_triangle:			return false; // TODO: 
	case Type_box_axis_aligned:	return Utility::Intersect( (const BoxAxisAligned&)target, *this, worldPointOfContact );
	case Type_box:				return Utility::Intersect( (const Box&)target, *this, worldPointOfContact );
	//case Type_frustrum:			return false; // TODO: 
	default:
		worldPointOfContact = Float3::null;
		return false;
	}
}

bool Sphere::Contains( const ICollideable &target ) const
{
	switch( target.type )
	{
	case Type_point:			return Utility::Intersect( *this, (const Point&)target );
	case Type_sphere:			return Utility::Contains( *this, (const Sphere&)target );
	//case Type_triangle:			return false; // TODO: 
	//case Type_box_axis_aligned:	return false; // TODO: 
	//case Type_box:				return false; // TODO: 
	//case Type_frustrum:			return false; // TODO: 
	default:					return false;
	}
}

Float Sphere::TimeOfContact( const ICollideable &deuterStart, const ICollideable &deuterEnd ) const
{
	if( deuterStart.type != deuterEnd.type )
		return -1.0f;

	switch( deuterStart.type )
	{
	//case Type_point:			// not implemented
	//case Type_sphere:			// not implemented
	//case Type_box:			// not implemented
	case Type_universe:			return 0.0f;
	default:					return 1.0f;
	}
}

namespace Oyster { namespace Math
{
	Sphere & Nlerp( const Sphere &start, const Sphere &end, Float t, Sphere &targetMem )
	{
		Float4 i = Lerp( Float4(start.center.xyz, start.radius), Float4(end.center.xyz, end.radius), t );
		targetMem.center.xyz = i.xyz;
		targetMem.radius = i.w;
		return targetMem;
	}
} }