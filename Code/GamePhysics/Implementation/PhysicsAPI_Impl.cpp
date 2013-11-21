#include "PhysicsAPI_Impl.h"

using namespace Oyster;
using namespace Physics;

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

void API_Impl::MoveToLimbo( unsigned int objRef )
{
	/** @todo TODO: Fix this function.*/
}
void API_Impl::ReleaseFromLimbo( unsigned int objRef )
{
	/** @todo TODO: Fix this function.*/
}

unsigned int API_Impl::AddObject( ::Utility::DynamicMemory::UniquePointer<IRigidBody> handle )
{
	/** @todo TODO: Fix this function.*/

	return 0;
}
void API_Impl::DestroyObject( unsigned int objRef )
{
	/** @todo TODO: Fix this function.*/
}