#include "PhysicsAPI_Impl.h"
#include "SimpleRigidBody.h"
#include "OysterPhysics3D.h"

using namespace ::Oyster::Physics;
using namespace ::Oyster::Physics3D;
using namespace ::Oyster::Math;
using namespace ::Oyster::Collision3D;
using namespace ::Utility::DynamicMemory;

API_Impl API_instance;

// default API::EventAction_Collision
void defaultCollisionAction( const ICustomBody *proto, const ICustomBody *deuter )
{ /* do nothing */ }

// default API::EventAction_Destruction
void defaultDestructionAction( UniquePointer<ICustomBody> proto )
{ /* do nothing besides proto auto deleting itself. */ }

Float4x4 & MomentOfInertia::CreateSphereMatrix( const Float mass, const Float radius)
{
	return Formula::MomentOfInertia::Sphere(mass, radius);
}

Float4x4 & MomentOfInertia::CreateHollowSphereMatrix( const Float mass, const Float radius)
{
	return Formula::MomentOfInertia::HollowSphere(mass, radius);
}

Float4x4 & MomentOfInertia::CreateCuboidMatrix( const Float mass, const Float height, const Float width, const Float depth )
{
	return Formula::MomentOfInertia::Cuboid(mass, height, width, depth);
}

Float4x4 & MomentOfInertia::CreateCylinderMatrix( const Float mass, const Float height, const Float radius )
{
	return Formula::MomentOfInertia::Cylinder(mass, height, radius);
}

Float4x4 & MomentOfInertia::CreateRodMatrix( const Float mass, const Float length )
{
	return Formula::MomentOfInertia::RodCenter(mass, length);
}

API & API::Instance()
{
	return API_instance;
}

API_Impl::API_Impl()
	: gravityConstant( Constant::gravity_constant ),
	  updateFrameLength( 1.0f / 120.0f ),
	  collisionAction( defaultCollisionAction ),
	  destructionAction( defaultDestructionAction )
{}

API_Impl::~API_Impl() {}

void API_Impl::Init( unsigned int numObjects, unsigned int numGravityWells , const Float3 &worldSize )
{
	//! @todo TODO: implement stub
}

void API_Impl::SetDeltaTime( float deltaTime )
{
	updateFrameLength = deltaTime;
}
void API_Impl::SetGravityConstant( float g )
{
	this->gravityConstant = g;
}
void API_Impl::SetAction( API::EventAction_Collision functionPointer )
{
	this->collisionAction = functionPointer;
}
void API_Impl::SetAction( API::EventAction_Destruction functionPointer )
{
	this->destructionAction = functionPointer;
}

void API_Impl::Update()
{
	/** @todo TODO: Fix this function.*/
}

bool API_Impl::IsInLimbo( const ICustomBody* objRef )
{
	//! @todo TODO: implement stub
	return true;
}

void API_Impl::MoveToLimbo( const ICustomBody* objRef )
{
	/** @todo TODO: Fix this function.*/
}
void API_Impl::ReleaseFromLimbo( const ICustomBody* objRef )
{
	/** @todo TODO: Fix this function.*/
}

void API_Impl::AddObject( ::Utility::DynamicMemory::UniquePointer<ICustomBody> handle )
{
	/** @todo TODO: Fix this function.*/
}

::Utility::DynamicMemory::UniquePointer<ICustomBody> API_Impl::ExtractObject( const ICustomBody* objRef )
{
	//! @todo TODO: implement stub
	return NULL;
}

void API_Impl::DestroyObject( const ICustomBody* objRef )
{
	/** @todo TODO: Fix this function.*/
}

void API_Impl::ApplyForceAt( const ICustomBody* objRef, const Float3 &worldPos, const Float3 &worldF )
{
	//! @todo TODO: implement stub
}

void API_Impl::ApplyCollisionResponse( const ICustomBody* objRefA, const ICustomBody* objRefB, Float &deltaWhen, Float3 &worldPointOfContact )
{
	//! @todo TODO: implement stub
}

void API_Impl::SetMomentOfInertiaTensor_KeepVelocity( const ICustomBody* objRef, const Float4x4 &localI )
{
	//! @todo TODO: implement stub
}

void API_Impl::SetMomentOfInertiaTensor_KeepMomentum( const ICustomBody* objRef, const Float4x4 &localI )
{
	//! @todo TODO: implement stub
}

void API_Impl::SetMass_KeepVelocity( const ICustomBody* objRef, Float m )
{
	//! @todo TODO: implement stub
}

void API_Impl::SetMass_KeepMomentum( const ICustomBody* objRef, Float m )
{
	//! @todo TODO: implement stub
}

void API_Impl::SetCenter( const ICustomBody* objRef, const Float3 &worldPos )
{
	//! @todo TODO: implement stub
}

void API_Impl::SetRotation( const ICustomBody* objRef, const Float4x4 &rotation )
{
	//! @todo TODO: implement stub
}

void API_Impl::SetOrientation( const ICustomBody* objRef, const Float4x4 &orientation )
{
	//! @todo TODO: implement stub
}

void API_Impl::SetSize( const ICustomBody* objRef, const Float3 &size )
{
	//! @todo TODO: implement stub
}

UniquePointer<ICustomBody> API_Impl::CreateSimpleRigidBody() const
{
	return new SimpleRigidBody();
}