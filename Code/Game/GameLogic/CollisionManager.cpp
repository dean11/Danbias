#include "PhysicsAPI.h"
#include "Object.h"
#include "DynamicObject.h"
#include "Player.h"
#include "Level.h"
#include "AttatchmentMassDriver.h"

using namespace Oyster;

using namespace GameLogic;

	void PlayerVBox(Player &player, DynamicObject &box, Oyster::Math::Float kineticEnergyLoss);
	void PlayerVObject(Player &player, Object &obj, Oyster::Math::Float kineticEnergyLoss);

	//Physics::ICustomBody::SubscriptMessage
	void Player::PlayerCollision(const Oyster::Physics::ICustomBody *rigidBodyPlayer, const Oyster::Physics::ICustomBody *obj, Oyster::Math::Float kineticEnergyLoss)
	{
		Player *player = ((Player*)(rigidBodyPlayer->GetCustomTag()));
		Object *realObj = (Object*)obj->GetCustomTag();

		switch (realObj->GetType())
		{
		case OBJECT_H::OBJECT_TYPE_GENERIC:
			PlayerVObject(*player,*realObj, kineticEnergyLoss);
			//return Physics::ICustomBody::SubscriptMessage_none;
			break;
		
		case OBJECT_TYPE::OBJECT_TYPE_BOX:
			PlayerVBox(*player,(*(DynamicObject*) realObj), kineticEnergyLoss);
			//return Physics::ICustomBody::SubscriptMessage_none;
			break;
		case OBJECT_TYPE::OBJECT_TYPE_PLAYER:
			//return Physics::ICustomBody::SubscriptMessage_none;
			break;
		case OBJECT_TYPE::OBJECT_TYPE_WORLD:
			int test = 5;
			break;
		}

		//return Physics::ICustomBody::SubscriptMessage_none;
	}
		
	void PlayerVBox(Player &player, DynamicObject &box, Oyster::Math::Float kineticEnergyLoss)
	{
		//use kinetic energyloss of the collision in order too determin how much damage to take
		//use as part of the damage algorithm
		player.DamageLife(20);
	}

	void PlayerVObject(Player &player, Object &obj, Oyster::Math::Float kineticEnergyLoss)
	{
		//Collision between a player and a general static or dynamic object
		//use kinetic energyloss of the collision in order too determin how much damage to take
		//use as part of the damage algorithm
		player.DamageLife(20);
	}
		
	//Oyster::Physics::ICustomBody::SubscriptMessage
	void Level::LevelCollision(const Oyster::Physics::ICustomBody *rigidBodyLevel, const Oyster::Physics::ICustomBody *obj, Oyster::Math::Float kineticEnergyLoss)
	{
		//return Physics::ICustomBody::SubscriptMessage_ignore_collision_response;
	}

	void AttatchmentMassDriver::ForcePushAction(Oyster::Physics::ICustomBody *obj)
	{
		Oyster::Math::Float4 pushForce = Oyster::Math::Float4(this->owner->GetLookDir()) * (500);
		((Object*)obj->GetCustomTag())->ApplyLinearImpulse(pushForce);
	}
