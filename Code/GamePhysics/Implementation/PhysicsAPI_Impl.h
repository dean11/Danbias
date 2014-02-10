#ifndef PHYSICS_API_IMPL_H
#define PHYSICS_API_IMPL_H

#include "../PhysicsAPI.h"
#include "Octree.h"
#include <btBulletDynamicsCommon.h>

namespace Oyster
{
	namespace Physics
	{
		class API_Impl : public API
		{
		public:
			API_Impl();
			virtual ~API_Impl();

			void Init();

			bool IsInLimbo( const ICustomBody* objRef );
			void MoveToLimbo( const ICustomBody* objRef );
			void ReleaseFromLimbo( const ICustomBody* objRef );

			// Bullet physics
			ICustomBody* AddCollisionSphere(float radius, ::Oyster::Math::Float4 rotation, ::Oyster::Math::Float3 position, float mass, float restitution, float staticFriction, float dynamicFriction);
			ICustomBody* AddCollisionBox(::Oyster::Math::Float3 halfSize, ::Oyster::Math::Float4 rotation, ::Oyster::Math::Float3 position, float mass, float restitution, float staticFriction, float dynamicFriction);
			ICustomBody* AddCollisionCylinder(::Oyster::Math::Float3 halfSize, ::Oyster::Math::Float4 rotation, ::Oyster::Math::Float3 position, float mass, float restitution, float staticFriction, float dynamicFriction);

			void UpdateWorld();

			void ApplyEffect( const Oyster::Collision3D::ICollideable& collideable, void* args, void(hitAction)(ICustomBody*, void*) );

		private:
			btBroadphaseInterface* broadphase;
			btDefaultCollisionConfiguration* collisionConfiguration;
			btCollisionDispatcher* dispatcher;
			btSequentialImpulseConstraintSolver* solver;
			btDiscreteDynamicsWorld* dynamicsWorld;
			std::vector<ICustomBody*> customBodies;
		};

		namespace Default
		{
			void EventAction_Destruction( ::Utility::DynamicMemory::UniquePointer<::Oyster::Physics::ICustomBody> proto );
			::Oyster::Physics::ICustomBody::SubscriptMessage EventAction_BeforeCollisionResponse( const ::Oyster::Physics::ICustomBody *proto, const ::Oyster::Physics::ICustomBody *deuter );
			void EventAction_AfterCollisionResponse( const ::Oyster::Physics::ICustomBody *proto, const ::Oyster::Physics::ICustomBody *deuter, ::Oyster::Math::Float kineticEnergyLoss );
			void EventAction_Move( const ::Oyster::Physics::ICustomBody *object );
		}
	}
}

#endif