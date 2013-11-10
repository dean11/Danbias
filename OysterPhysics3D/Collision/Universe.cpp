#include "Universe.h"
#include "OysterCollision.h"

using namespace ::Oyster::Collision3D;
using namespace ::Utility::Memory;

Universe::Universe() : ICollideable(Type_universe) {}
Universe::~Universe() {}

Universe & Universe::operator = ( const Universe &universe )
{ return *this; }

UniquePointer<ICollideable> Universe::Clone( ) const
{ return UniquePointer<ICollideable>( new Universe(*this) ); }

bool Universe::Intersects( const ICollideable *target ) const
{ // universe touches everything
	switch( target->type )
	{
	case Type_ray:
		((Ray*)target)->collisionDistance = 0.0f;
		break;
	case Type_line:
		((Line*)target)->ray.collisionDistance = 0.0f;
		break;
	default: break;
	}

	return true;
}

bool Sphere::Contains( const ICollideable *target ) const
{ return true; } // universe contains everything

