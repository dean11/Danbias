#include "Universe.h"
#include "OysterCollision3D.h"

using namespace ::Utility::Value;
using namespace ::Oyster::Math;
using namespace ::Oyster::Collision3D;
using namespace ::Utility::DynamicMemory;

Universe::Universe() : ICollideable(Type_universe) {}
Universe::~Universe() {}

Universe & Universe::operator = ( const Universe &universe )
{
	return *this;
}

UniquePointer<ICollideable> Universe::Clone( ) const
{
	return UniquePointer<ICollideable>( new Universe(*this) );
}

bool Universe::Intersects( const ICollideable &target ) const
{ // universe touches everything
	switch( target.type )
	{
	case Type_ray:
		((const Ray&)target).collisionDistance = 0.0f;
		break;
	case Type_line:
		((const Line&)target).ray.collisionDistance = 0.0f;
		break;
	default: break;
	}

	return true;
}

bool Universe::Intersects( const ICollideable &target, Float4 &worldPointOfContact ) const
{ // universe touches everything
	switch( target.type )
	{
	case Type_point:
		worldPointOfContact = ((const Point&)target).center;
		break;
	case Type_sphere:
		worldPointOfContact = ((const Sphere&)target).center;
		break;
	case Type_plane:
		worldPointOfContact = ((const Plane&)target).normal * ((const Plane&)target).phasing;
		break;
	case Type_box_axis_aligned:
		worldPointOfContact = Average( ((const BoxAxisAligned&)target).minVertex, ((const BoxAxisAligned&)target).maxVertex );
		break;
	case Type_box:
		worldPointOfContact = ((const Box&)target).center;
		break;
	case Type_frustrum:
		worldPointOfContact = Average( ((const Frustrum&)target).leftPlane.normal * ((const Frustrum&)target).leftPlane.phasing, ((const Frustrum&)target).rightPlane.normal * ((const Frustrum&)target).rightPlane.phasing );
		worldPointOfContact = Average( worldPointOfContact, Average( ((const Frustrum&)target).bottomPlane.normal * ((const Frustrum&)target).bottomPlane.phasing, ((const Frustrum&)target).topPlane.normal * ((const Frustrum&)target).topPlane.phasing ) );
		worldPointOfContact = Average( worldPointOfContact, Average( ((const Frustrum&)target).nearPlane.normal * ((const Frustrum&)target).nearPlane.phasing, ((const Frustrum&)target).farPlane.normal * ((const Frustrum&)target).farPlane.phasing ) );
		break;
	case Type_ray:
		((const Ray&)target).collisionDistance = 0.0f;
		worldPointOfContact = ((const Ray&)target).origin;
		break;
	case Type_line:
		((const Line&)target).ray.collisionDistance = 0.0f;
		worldPointOfContact = ((const Line&)target).ray.origin;
		break;
	default:
		worldPointOfContact = Float3::null;
		break;
	}

	return true;
}

bool Universe::Contains( const ICollideable &target ) const
{ // universe contains everything
	return true;
}

Float Universe::TimeOfContact( const ICollideable &deuterStart, const ICollideable &deuterEnd ) const
{
	return 0.0f;
}