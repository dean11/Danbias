//////////////////////////////////////////////////
//Created by Erik and Linda of the GameLogic team
//////////////////////////////////////////////////


#ifndef OBJECT_H
#define OBJECT_H

#include "GameLogicStates.h"
#include "GameAPI.h"
#include <PhysicsAPI.h>

namespace GameLogic
{
	class Game;

	class Object	:public IObjectData
	{
	public:		
		Object();
		Object(OBJECT_TYPE type);
		Object(Oyster::Physics::ICustomBody *rigidBody, OBJECT_TYPE type);
		Object(void* collisionFuncBefore, void* collisionFuncAfter, OBJECT_TYPE type);
		Object(Oyster::Physics::ICustomBody *rigidBody ,void* collisionFuncBefore, void* collisionFuncAfter, OBJECT_TYPE type);
		Object(Oyster::Physics::ICustomBody *rigidBody ,Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncBefore)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter), Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncAfter)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), OBJECT_TYPE type);
		~Object(void);

		OBJECT_TYPE GetObjectType() const			override;
		int GetID() const							override;
		Oyster::Math::Float3 GetPosition()			override;
		Oyster::Math::Quaternion GetRotation()		override;
		Oyster::Math::Float3 GetScale()				override;
		Oyster::Math::Float4x4 GetOrientation()		override;

		void setID(int id);

		Oyster::Physics::ICustomBody* GetRigidBody();
		void ApplyLinearImpulse(Oyster::Math::Float3 force);

		virtual void BeginFrame();
		virtual void EndFrame();

		void setBeforeCollisonFunc(Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncBefore)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter));
		void setAfterCollisonFunc(Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncAfter)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss));

		static Oyster::Physics::ICustomBody::SubscriptMessage DefaultCollisionBefore(Oyster::Physics::ICustomBody *rigidBodyLevel, Oyster::Physics::ICustomBody *obj);
		static Oyster::Physics::ICustomBody::SubscriptMessage DefaultCollisionAfter(Oyster::Physics::ICustomBody *rigidBodyLevel, Oyster::Physics::ICustomBody *obj, Oyster::Math::Float kineticEnergyLoss);
	
	public: //HACK: This should be private when level is dynamic
		OBJECT_TYPE type;
		int objectID;

	protected:
		Oyster::Physics::ICustomBody *rigidBody;

		static const Game* gameInstance;
		Oyster::Math::Float3 currLook;
		Oyster::Math::Float3 newLook;
	};

}

#endif 