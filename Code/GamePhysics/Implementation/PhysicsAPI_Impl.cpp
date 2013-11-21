#include "PhysicsAPI_Impl.h"
#include "SimpleRigidBody.h"

using namespace ::Oyster::Physics;
using namespace ::Oyster::Math;
using namespace ::Oyster::Collision3D;
using namespace ::Utility::DynamicMemory;

API_Impl instance;  

API & Instance()
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

bool API_Impl::IsInLimbo( unsigned int objRef )
{
	//! @todo TODO: implement stub
	return true;
}

void API_Impl::MoveToLimbo( unsigned int objRef )
{
	/** @todo TODO: Fix this function.*/
}
void API_Impl::ReleaseFromLimbo( unsigned int objRef )
{
	/** @todo TODO: Fix this function.*/
}

unsigned int API_Impl::AddObject( ::Utility::DynamicMemory::UniquePointer<ICustomBody> handle )
{
	/** @todo TODO: Fix this function.*/

	return 0;
}

::Utility::DynamicMemory::UniquePointer<ICustomBody> ExtractObject( unsigned int objRef )
{
	//! @todo TODO: implement stub
	return NULL;
}

void API_Impl::DestroyObject( unsigned int objRef )
{
	/** @todo TODO: Fix this function.*/
}

void API_Impl::ApplyForceAt( unsigned int objRef, const Float3 &worldPos, const Float3 &worldF )
{
	//! @todo TODO: implement stub
}

void API_Impl::ApplyCollisionResponse( unsigned int objRefA, unsigned int objRefB, Float &deltaWhen, Float3 &worldPointOfContact )
{
	//! @todo TODO: implement stub
}

void API_Impl::SetMomentOfInertiaTensor_KeepVelocity( unsigned int objRef, const Float4x4 &localI )
{
	//! @todo TODO: implement stub
}

void API_Impl::SetMomentOfInertiaTensor_KeepMomentum( unsigned int objRef, const Float4x4 &localI )
{
	//! @todo TODO: implement stub
}

void API_Impl::SetMass_KeepVelocity( unsigned int objRef, Float m )
{
	//! @todo TODO: implement stub
}

void API_Impl::SetMass_KeepMomentum( unsigned int objRef, Float m )
{
	//! @todo TODO: implement stub
}

void API_Impl::SetCenter( unsigned int objRef, const Float3 &worldPos )
{
	//! @todo TODO: implement stub
}

void API_Impl::SetRotation( unsigned int objRef, const Float4x4 &rotation )
{
	//! @todo TODO: implement stub
}

void API_Impl::SetOrientation( unsigned int objRef, const Float4x4 &orientation )
{
	//! @todo TODO: implement stub
}

UniquePointer<ICustomBody> API_Impl::CreateSimpleRigidBody() const
{
	return new SimpleRigidBody();
}