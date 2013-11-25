#include "PhysicsAPI_Impl.h"
#include "SimpleRigidBody.h"
#include "OysterPhysics3D.h"

using namespace ::Oyster::Physics;
using namespace ::Oyster::Math;
using namespace ::Oyster::Collision3D;
using namespace ::Utility::DynamicMemory;

API_Impl instance;  

::Oyster::Math::Float4x4 & MomentOfInertia::CreateSphereMatrix( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float radius)
{
	return ::Oyster::Physics3D::Formula::MomentOfInertia::Sphere(mass, radius);
}

::Oyster::Math::Float4x4 & MomentOfInertia::CreateHollowSphereMatrix( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float radius)
{
	return ::Oyster::Physics3D::Formula::MomentOfInertia::HollowSphere(mass, radius);
}

::Oyster::Math::Float4x4 & MomentOfInertia::CreateCuboidMatrix( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float height, const ::Oyster::Math::Float width, const ::Oyster::Math::Float depth )
{
	return ::Oyster::Physics3D::Formula::MomentOfInertia::Cuboid(mass, height, width, depth);
}

::Oyster::Math::Float4x4 & MomentOfInertia::CreateCylinderMatrix( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float height, const ::Oyster::Math::Float radius )
{
	return ::Oyster::Physics3D::Formula::MomentOfInertia::Cylinder(mass, height, radius);
}

::Oyster::Math::Float4x4 & MomentOfInertia::CreateRodMatrix( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float length )
{
	return ::Oyster::Physics3D::Formula::MomentOfInertia::RodCenter(mass, length);
}

API & API::Instance()
{
	return instance;
}

API_Impl::API_Impl()
{
	/** @todo TODO: Fix this constructor.*/
}

API_Impl::~API_Impl()
{
	/** @todo TODO: Fix this destructor.*/
}

void API_Impl::SetDeltaTime( float deltaTime )
{
	/** @todo TODO: Fix this function.*/
}
void API_Impl::SetGravityConstant( float g )
{
	/** @todo TODO: Fix this function.*/
}
void API_Impl::SetAction( EventAction_Collision functionPointer )
{
	/** @todo TODO: Fix this function.*/
}
void API_Impl::SetAction( EventAction_Destruction functionPointer )
{
	/** @todo TODO: Fix this function.*/
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

UniquePointer<ICustomBody> API_Impl::CreateSimpleRigidBody() const
{
	return new SimpleRigidBody();
}