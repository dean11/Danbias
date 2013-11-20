#ifndef PHYSICS_API_IMPL_H
#define PHYSICS_API_IMPL_H

#include "../PhysicsAPI.h"

namespace Oyster
{
	namespace Physics
	{
		class API_Impl : public API
		{
		public:
			API_Impl();
			virtual ~API_Impl();

			void SetDeltaTime( float deltaTime );
			void SetGravityConstant( float g );
			void SetAction( EventAction_Collision functionPointer );
			void SetAction( EventAction_Destruction functionPointer );

			void Update();

			void MoveToLimbo( unsigned int objRef );
			void ReleaseFromLimbo( unsigned int objRef );

			unsigned int AddObject( ::Utility::DynamicMemory::UniquePointer<IRigidBody> handle );
			void DestroyObject( unsigned int objRef );
		};
	
	}

}

#endif