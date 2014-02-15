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
	
		Object(Oyster::Physics::ICustomBody *rigidBody, void (*collisionFuncAfter)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, int objectID);
		Object(Oyster::Physics::ICustomBody *rigidBody, Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncAfter)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, int objectID);
		~Object(void);

		ObjectSpecialType GetObjectType() const			override;
		int GetID() const								override;	
		void setID(int id);
		Oyster::Math::Float3 GetPosition()			override;
		Oyster::Math::Quaternion GetRotation()		override;
		Oyster::Math::Float3 GetScale()				override;
		Oyster::Math::Float4x4 GetOrientation()		override;

		Oyster::Math::Float getExtraDamageOnCollision();

		// API overrides
		


		Oyster::Physics::ICustomBody* GetRigidBody();

		virtual void BeginFrame();
		virtual void EndFrame();

		void setBeforeCollisonFunc(Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncBefore)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter));
		void setAfterCollisonFunc(Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncAfter)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss));

		static Oyster::Physics::ICustomBody::SubscriptMessage DefaultCollisionAfter(Oyster::Physics::ICustomBody *rigidBodyLevel, Oyster::Physics::ICustomBody *obj, Oyster::Math::Float kineticEnergyLoss);
	

	public: //TODO: Hax This should be private when level is dynamic


	protected:
		Oyster::Physics::ICustomBody *rigidBody;

		static const Game* gameInstance;
		Oyster::Math::Float3 currLook;
		Oyster::Math::Float3 newLook;

		ObjectSpecialType type;
		int objectID;

		Oyster::Math::Float extraDamageOnCollision;
	};

}

#endif 