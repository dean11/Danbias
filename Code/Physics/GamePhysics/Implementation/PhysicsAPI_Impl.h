#ifndef PHYSICS_API_IMPL_H
#define PHYSICS_API_IMPL_H

#include "../PhysicsAPI.h"
#include <btBulletDynamicsCommon.h>
#include <vld.h>
#include <BulletWorldImporter\btBulletWorldImporter.h>

namespace Oyster
{
	namespace Physics
	{
		class API_Impl : public API
		{
		public:
			struct ContactSensorCallback : public btCollisionWorld::ContactResultCallback
			{
				ContactSensorCallback(btRigidBody& contactBody, EventAction_ApplyEffect effect, void* args)
					: btCollisionWorld::ContactResultCallback(), body(contactBody), func(effect), args(args) {}
			
				btRigidBody& body;
				EventAction_ApplyEffect func;
				void* args;
			
				virtual bool needsCollision(btBroadphaseProxy* proxy) const
				{
					if(!btCollisionWorld::ContactResultCallback::needsCollision(proxy))
						return false;
			
					return body.checkCollideWithOverride(static_cast<btCollisionObject*>(proxy->m_clientObject));
				}
			
				virtual btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0, int partId0, int index0, const btCollisionObjectWrapper* colObj1, int partId1, int index1)
				{
					btVector3 pt;
					if(colObj0->m_collisionObject == &body)
					{
						pt = cp.m_localPointA;
						this->func((ICustomBody*)(colObj1->getCollisionObject()->getUserPointer()), this->args);
					}
					else
					{
						pt = cp.m_localPointB;
						this->func((ICustomBody*)(colObj0->getCollisionObject()->getUserPointer()), this->args);
					}
			
					return 0;
				}
			};

			struct ClosestNotMe : btCollisionWorld::ClosestRayResultCallback
			{
				public:
				ClosestNotMe (btRigidBody* me) : btCollisionWorld::ClosestRayResultCallback(btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0))
				{
					this->me = me;
				}

				virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult,bool normalInWorldSpace)
				{
					if (rayResult.m_collisionObject == this->me)
						return 1.0;
					return ClosestRayResultCallback::addSingleResult (rayResult, normalInWorldSpace);
				}
				protected:
					btRigidBody* me;
			};

			API_Impl();
			virtual ~API_Impl();

			void Release();

			void Init();

			bool IsInLimbo( const ICustomBody* objRef );
			void MoveToLimbo( const ICustomBody* objRef );
			void ReleaseFromLimbo( const ICustomBody* objRef );

			void SetGravityPoint(::Oyster::Math::Float3 gravityPoint);
			void SetGravity(float gravity);

			// Bullet physics
			ICustomBody* AddCollisionSphere(float radius, ::Oyster::Math::Float4 rotation, ::Oyster::Math::Float3 position, float mass, float restitution, float staticFriction, float dynamicFriction);
			ICustomBody* AddCollisionBox(::Oyster::Math::Float3 halfSize, ::Oyster::Math::Float4 rotation, ::Oyster::Math::Float3 position, float mass, float restitution, float staticFriction, float dynamicFriction);
			ICustomBody* AddCollisionCylinder(::Oyster::Math::Float3 halfSize, ::Oyster::Math::Float4 rotation, ::Oyster::Math::Float3 position, float mass, float restitution, float staticFriction, float dynamicFriction);

			ICustomBody* AddCharacter(::Oyster::Math::Float height, ::Oyster::Math::Float radius, ::Oyster::Math::Float4 rotation, ::Oyster::Math::Float3 position, float mass, float restitution, float staticFriction, float dynamicFriction);

			ICustomBody* AddTriangleMesh(const std::wstring fileName, ::Oyster::Math::Float4 rotation, ::Oyster::Math::Float3 position, ::Oyster::Math::Float3 scale, float mass, float restitution, float staticFriction, float dynamicFriction);

			void SetTimeStep(float timeStep);

			void UpdateWorld(float deltaTime);

			void ApplyEffect(Oyster::Collision3D::ICollideable* collideable, void* args, EventAction_ApplyEffect effect);

			ICustomBody* RayClosestObjectNotMe(ICustomBody* self, Oyster::Math::Float3 origin, Oyster::Math::Float3 target);

		private:
			btBroadphaseInterface* broadphase;
			btDefaultCollisionConfiguration* collisionConfiguration;
			btCollisionDispatcher* dispatcher;
			btSequentialImpulseConstraintSolver* solver;
			btDiscreteDynamicsWorld* dynamicsWorld;
			std::vector<ICustomBody*> customBodies;
			btBulletWorldImporter importer;
			unsigned int nrOfShapes;

			float timeStep;

			::Oyster::Math::Float3 gravityPoint;
			float gravity;

			float deltaTime;
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