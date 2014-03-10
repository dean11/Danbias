//////////////////////////////////////////////////
//Created by Erik and Linda of the GameLogic team
//////////////////////////////////////////////////
#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H
#include "GameLogicDef.h"
#include "Object.h"


namespace GameLogic
{
	class Player;
	class DynamicObject : public Object
	{

	public:
		DynamicObject();
		DynamicObject(Oyster::Physics::ICustomBody *rigidBody , void (*EventOnCollision)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, int objectID);
		DynamicObject(Oyster::Physics::ICustomBody *rigidBody , Oyster::Physics::ICustomBody::SubscriptMessage (*EventOnCollision)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, int objectID);

		DynamicObject(Oyster::Physics::ICustomBody *rigidBody , void (*EventOnCollision)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, int objectID, Oyster::Math::Float extraDamageOnCollision);
		DynamicObject(Oyster::Physics::ICustomBody *rigidBody , Oyster::Physics::ICustomBody::SubscriptMessage (*EventOnCollision)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, int objectID, Oyster::Math::Float extraDamageOnCollision);

		~DynamicObject(void);

		void ReleaseDynamicObject();
		bool IsReleased();
		bool IsActive();
		void Inactivate();
		void Activate();

		void SetAffectedBy(GameLogic::Player &player);
		void SetManipulatingPlayer(GameLogic::Player &player);
		void RemoveAffectedBy();
		void RemoveManipulation();
		GameLogic::Player* getAffectingPlayer();
		GameLogic::Player* getManipulatingPlayer();

		static void DynamicObject::DynamicDefaultOnCollision(Oyster::Physics::ICustomBody *rigidBodyObject, Oyster::Physics::ICustomBody *obj, Oyster::Math::Float kineticEnergyLoss);

		void AttemptResetToInitalPos(Oyster::Math::Float dt);

	private:
		bool isActive;
		bool isReleased;
	protected:
		GameLogic::Player *affectedBy;
		GameLogic::Player *manipulatedBy;


	};

}

#endif