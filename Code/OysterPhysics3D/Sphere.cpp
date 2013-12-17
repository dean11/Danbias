#include "Sphere.h"
#include "OysterCollision3D.h"

using namespace ::Oyster::Collision3D;
using namespace ::Oyster::Math;

Sphere::Sphere( ) : ICollideable(Type_sphere)
{
	this->center = Float3::null;
	this->radius = 0.0f;
}

Sphere::Sphere( const Float3 &_position, const Float &_radius ) : ICollideable(Type_sphere)
{
	this->center = _position;
	this->radius = _radius;
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

bool Sphere::Intersects( const ICollideable &target, Float3 &worldPointOfContact ) const
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