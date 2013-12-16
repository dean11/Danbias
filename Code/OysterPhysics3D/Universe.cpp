#include "Universe.h"
#include "OysterCollision3D.h"

using namespace ::Utility::Value;
using namespace ::Oyster::Math;
using namespace ::Oyster::Collision3D;
using namespace ::Utility::DynamicMemory;

Universe::Universe() : ICollideable(Type_universe) {}
Universe::~Universe() {}

Universe & Universe::operator = ( const Universe &universe )
{ return *this; }

UniquePointer<ICollideable> Universe::Clone( ) const
{ return UniquePointer<ICollideable>( new Universe(*this) ); }

bool Universe::Intersects( const ICollideable &target ) const
{ // universe touches everything
	switch( target.type )
	{
	case Type_ray:
		((Ray*)&target)->collisionDistance = 0.0f;
		break;
	case Type_line:
		((Line*)&target)->ray.collisionDistance = 0.0f;
		break;
	default: break;
	}

	return true;
}

bool Universe::Intersects( const ICollideable &target, Float3 &worldPointOfContact ) const
{ // universe touches everything
	switch( target.type )
	{
	case Type_point:
		worldPointOfContact = ((Point*)&target)->center;
		break;
	case Type_sphere:
		worldPointOfContact = ((Sphere*)&target)->center;
		break;
	case Type_plane:
		worldPointOfContact = ((Plane*)&target)->normal * ((Plane*)&target)->phasing;
		break;
	case Type_box_axis_aligned:
		worldPointOfContact = Average( ((BoxAxisAligned*)&target)->minVertex, ((BoxAxisAligned*)&target)->maxVertex );
		break;
	case Type_box:
		worldPointOfContact = ((Box*)&target)->center;
		break;
	case Type_frustrum:
		worldPointOfContact = Average( ((Frustrum*)&target)->leftPlane.normal * ((Frustrum*)&target)->leftPlane.phasing, ((Frustrum*)&target)->rightPlane.normal * ((Frustrum*)&target)->rightPlane.phasing );
		worldPointOfContact = Average( worldPointOfContact, Average( ((Frustrum*)&target)->bottomPlane.normal * ((Frustrum*)&target)->bottomPlane.phasing, ((Frustrum*)&target)->topPlane.normal * ((Frustrum*)&target)->topPlane.phasing ) );
		worldPointOfContact = Average( worldPointOfContact, Average( ((Frustrum*)&target)->nearPlane.normal * ((Frustrum*)&target)->nearPlane.phasing, ((Frustrum*)&target)->farPlane.normal * ((Frustrum*)&target)->farPlane.phasing ) );
		break;
	case Type_ray:
		((Ray*)&target)->collisionDistance = 0.0f;
		worldPointOfContact = ((Ray*)&target)->origin;
		break;
	case Type_line:
		((Line*)&target)->ray.collisionDistance = 0.0f;
		worldPointOfContact = ((Line*)&target)->ray.origin;
		break;
	default:
		worldPointOfContact = Float3::null;
		break;
	}

	return true;
}

bool Universe::Contains( const ICollideable &target ) const
{ return true; } // universe contains everything

