#include "PhysicsAPI.h"
#include "Object.h"
#include "DynamicObject.h"
#include "Player.h"
#include "Level.h"
#include "AttatchmentMassDriver.h"
#include "Game.h"
#include "CollisionManager.h"
#include "JumpPad.h"

using namespace Oyster;

using namespace GameLogic;

	void PlayerVObject(Player &player, Object &obj, Oyster::Math::Float kineticEnergyLoss);
	void SendObjectFlying(Oyster::Physics::ICustomBody &obj, Oyster::Math::Float3 force);

	//Physics::ICustomBody::SubscriptMessage
	void Player::PlayerCollision(Oyster::Physics::ICustomBody *rigidBodyPlayer, Oyster::Physics::ICustomBody *obj, Oyster::Math::Float kineticEnergyLoss)
	{
		
		Player *player = ((Game::PlayerData*)(rigidBodyPlayer->GetCustomTag()))->player;
		Object *realObj = (Object*)obj->GetCustomTag(); //needs to be changed?

		switch (realObj->GetObjectType())
		{
		case OBJECT_TYPE::OBJECT_TYPE_GENERIC:
			PlayerVObject(*player,*realObj, kineticEnergyLoss);
			//return Physics::ICustomBody::SubscriptMessage_none;
			break;
		
		case OBJECT_TYPE::OBJECT_TYPE_BOX:
			PlayerVObject(*player,*realObj, kineticEnergyLoss);
			//return Physics::ICustomBody::SubscriptMessage_none;
			break;
		case OBJECT_TYPE::OBJECT_TYPE_PLAYER:
			//return Physics::ICustomBody::SubscriptMessage_none;
			break;
		case OBJECT_TYPE::OBJECT_TYPE_WORLD:
			PlayerVObject(*player,*realObj, kineticEnergyLoss);
			//player->playerState = PLAYER_STATE::PLAYER_STATE_WALKING;
			break;
		case OBJECT_TYPE::OBJECT_TYPE_JUMPPAD:
			int i = 0;
		// JUMP
			break;
		}

		//return Physics::ICustomBody::SubscriptMessage_none;
	}

	void JumpPad::JumpPadActivated(Oyster::Physics::ICustomBody *rigidBodyJumpPad, Oyster::Physics::ICustomBody *obj, Oyster::Math::Float kineticEnergyLoss)
	{
		JumpPad *jumpPad = (JumpPad*)(rigidBodyJumpPad->GetCustomTag());
		Object *realObj = (Object*)obj->GetCustomTag(); //needs to be changed?

		switch (realObj->GetObjectType())
		{
		case OBJECT_TYPE::OBJECT_TYPE_GENERIC:
			break;
		case OBJECT_TYPE::OBJECT_TYPE_BOX:
			break;
		case OBJECT_TYPE::OBJECT_TYPE_PLAYER:
			SendObjectFlying(*obj, jumpPad->pushForce);
			break;
		case OBJECT_TYPE::OBJECT_TYPE_WORLD:
			break;
		}
	}

	void SendObjectFlying(Oyster::Physics::ICustomBody &obj, Oyster::Math::Float3 force)
	{
		Oyster::Physics::ICustomBody::State state;

		state = obj.GetState();
		//state.ApplyLinearImpulse(force);
		obj.SetState(state);
	}
	

	void PlayerVObject(Player &player, Object &obj, Oyster::Math::Float kineticEnergyLoss)
	{
		//Collision between a player and a general static or dynamic object
		//use kinetic energyloss of the collision in order too determin how much damage to take
		//use as part of the damage algorithm
		int damageDone = 0;
		int forceThreashHold = 200000;

		if(kineticEnergyLoss > forceThreashHold) //should only take damage if the force is high enough
		{
			damageDone = (int)(kineticEnergyLoss * 0.10f);
			//player.DamageLife(damageDone);
		}
		
	}	

	Oyster::Physics::ICustomBody::SubscriptMessage Object::DefaultCollisionAfter(Oyster::Physics::ICustomBody *rigidBodyLevel, Oyster::Physics::ICustomBody *obj, Oyster::Math::Float kineticEnergyLoss)
	{
		return Physics::ICustomBody::SubscriptMessage_none;
	}
	Oyster::Physics::ICustomBody::SubscriptMessage Player::PlayerCollisionAfter(Oyster::Physics::ICustomBody *rigidBodyLevel, Oyster::Physics::ICustomBody *obj, Oyster::Math::Float kineticEnergyLoss)
	{
		return Physics::ICustomBody::SubscriptMessage_none;
	}
	Oyster::Physics::ICustomBody::SubscriptMessage CollisionManager::IgnoreCollision(Oyster::Physics::ICustomBody *rigidBody, Oyster::Physics::ICustomBody *obj)
	{
		return Physics::ICustomBody::SubscriptMessage_ignore_collision_response;
	}


	Oyster::Physics::ICustomBody::SubscriptMessage Level::LevelCollisionAfter(Oyster::Physics::ICustomBody *rigidBodyLevel, Oyster::Physics::ICustomBody *obj, Oyster::Math::Float kineticEnergyLoss)
	{
		return Physics::ICustomBody::SubscriptMessage_ignore_collision_response;
	}

	void AttatchmentMassDriver::ForcePushAction(Oyster::Physics::ICustomBody *obj, void *args)
	{
		if(obj->GetState().mass == 0) return;

		Object *realObj = (Object*)obj->GetCustomTag();

		if(realObj->GetObjectType() == OBJECT_TYPE_PLAYER || realObj->GetObjectType() == OBJECT_TYPE_WORLD)
			return;

		obj->ApplyImpulse(((forcePushData*)(args))->pushForce);
	}

	void AttatchmentMassDriver::AttemptPickUp(Oyster::Physics::ICustomBody *obj, void* args)
	{
		if(obj->GetState().mass == 0) return;

		AttatchmentMassDriver *weapon = ((AttatchmentMassDriver*)args);

		if(weapon->hasObject)
		{
			//do nothing
		}
		else
		{
			Object* realObj = (Object*)(obj->GetCustomTag());
			//check so that it is an object that you can pickup

			switch(realObj->GetObjectType())
			{
			case OBJECT_TYPE::OBJECT_TYPE_BOX:
				//move obj to limbo in physics to make sure it wont collide with anything
//				Oyster::Physics::API::Instance().MoveToLimbo(obj);
				weapon->heldObject = obj; //weapon now holds the object
				weapon->hasObject = true;

				break;
			}
			
		}
		

	}