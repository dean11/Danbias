#include "PhysicsAPI.h"
#include "Object.h"
#include "DynamicObject.h"
#include "Player.h"
#include "Level.h"
#include "AttatchmentMassDriver.h"
#include "Game.h"

using namespace Oyster;

using namespace GameLogic;

	void PlayerVBox(Player &player, DynamicObject &box, Oyster::Math::Float kineticEnergyLoss);
	void PlayerVObject(Player &player, Object &obj, Oyster::Math::Float kineticEnergyLoss);

	//Physics::ICustomBody::SubscriptMessage
	void Player::PlayerCollision(Oyster::Physics::ICustomBody *rigidBodyPlayer, Oyster::Physics::ICustomBody *obj, Oyster::Math::Float kineticEnergyLoss)
	{
		
		Player *player = ((Game::PlayerData*)(rigidBodyPlayer->GetCustomTag()))->player;
		Object *realObj = (Object*)obj->GetCustomTag(); //needs to be changed?

		return;
		switch (realObj->GetObjectType())
		{
		case OBJECT_TYPE::OBJECT_TYPE_GENERIC:
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
		int damageDone = 0;
		int forceThreashHold = 200;

		if(kineticEnergyLoss > forceThreashHold) //should only take damage if the force is high enough
		{
			damageDone = kineticEnergyLoss * 0.10f;
			player.DamageLife(damageDone);
		}
		
	}	
	Oyster::Physics::ICustomBody::SubscriptMessage Object::DefaultCollisionBefore(Oyster::Physics::ICustomBody *rigidBodyLevel, Oyster::Physics::ICustomBody *obj)
	{
		return Physics::ICustomBody::SubscriptMessage_none;
	}
	//Oyster::Physics::ICustomBody::SubscriptMessage
	Oyster::Physics::ICustomBody::SubscriptMessage Level::LevelCollisionBefore(Oyster::Physics::ICustomBody *rigidBodyLevel, Oyster::Physics::ICustomBody *obj)
	{
		return Physics::ICustomBody::SubscriptMessage_ignore_collision_response;
	}
	Oyster::Physics::ICustomBody::SubscriptMessage Level::LevelCollisionAfter(Oyster::Physics::ICustomBody *rigidBodyLevel, Oyster::Physics::ICustomBody *obj, Oyster::Math::Float kineticEnergyLoss)
	{
		return Physics::ICustomBody::SubscriptMessage_ignore_collision_response;
	}

	void AttatchmentMassDriver::ForcePushAction(Oyster::Physics::ICustomBody *obj, void *args)
	{
		Oyster::Math::Float3 pushForce = Oyster::Math::Float4(1,0,0) * (500);
		Oyster::Physics::ICustomBody::State state;
		Object *realObj = (Object*)obj->GetCustomTag();
		if(realObj->GetType() == OBJECT_TYPE_BOX)
		{
			state = obj->GetState();
			state.SetOrientation(Oyster::Math::Float3(1,0.5,1),Oyster::Math::Float3(1,0.5,1));
			obj->SetState(state);
		}

		if(realObj->GetType() == OBJECT_TYPE_PLAYER || realObj->GetType() == OBJECT_TYPE_WORLD)
			return;
		state = obj->GetState();
		state.ApplyLinearImpulse(pushForce);
		obj->SetState(state);
		//((Object*)obj->GetCustomTag())->ApplyLinearImpulse(pushForce);
	}