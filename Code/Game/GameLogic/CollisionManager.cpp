#include "PhysicsAPI.h"
#include "Object.h"
#include "DynamicObject.h"
#include "Player.h"
#include "Level.h"
#include "AttatchmentMassDriver.h"
#include "Game.h"
#include "CollisionManager.h"
#include "JumpPad.h"
#include "Portal.h"

using namespace Oyster;

using namespace GameLogic;

	void PlayerVObject(Player &player, Object &obj, Oyster::Math::Float kineticEnergyLoss);
	void SendObjectFlying(Oyster::Physics::ICustomBody &obj, Oyster::Math::Float3 force);
	void Teleport(Oyster::Physics::ICustomBody &obj, Oyster::Math::Float3 target);

	//Physics::ICustomBody::SubscriptMessage
	void Player::PlayerCollision(Oyster::Physics::ICustomBody *rigidBodyPlayer, Oyster::Physics::ICustomBody *obj, Oyster::Math::Float kineticEnergyLoss)
	{
		
		Player *player = ((Game::PlayerData*)(rigidBodyPlayer->GetCustomTag()))->player;
		Object *realObj = (Object*)obj->GetCustomTag(); //needs to be changed?

		switch (realObj->GetObjectType())
		{
		case ObjectSpecialType_StandarsBox:
			PlayerVObject(*player,*realObj, kineticEnergyLoss);
			//return Physics::ICustomBody::SubscriptMessage_none;
			break;
		case ObjectSpecialType_Player:
			//return Physics::ICustomBody::SubscriptMessage_none;
			break;
		case ObjectSpecialType_World:
			PlayerVObject(*player,*realObj, kineticEnergyLoss);
			//player->playerState = PLAYER_STATE::PLAYER_STATE_WALKING;
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
		case ObjectSpecialType_Generic:
			break;
		case ObjectSpecialType_StandarsBox:
			break;
		case ObjectSpecialType_Player:
			SendObjectFlying(*obj, jumpPad->pushForce);
			break;
		case ObjectSpecialType_World:
			break;
		}
	}

	void SendObjectFlying(Oyster::Physics::ICustomBody &obj, Oyster::Math::Float3 force)
	{
		obj.ApplyImpulse(force);
	}

	void Portal::PortalActivated(Oyster::Physics::ICustomBody *rigidBodyPortal, Oyster::Physics::ICustomBody *obj, Oyster::Math::Float kineticEnergyLoss)
	{
		Portal *portal = (Portal*)(rigidBodyPortal->GetCustomTag());

		if(obj->GetState().mass == 0) return;

		Teleport(*obj,portal->portalExit);
	}

	void Teleport(Oyster::Physics::ICustomBody &obj, Oyster::Math::Float3 target)
	{
		obj.SetPosition(target);
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

		if(realObj->GetObjectType() == ObjectSpecialType_Player || realObj->GetObjectType() == ObjectSpecialType_World)
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
			case ObjectSpecialType_StandarsBox:
				//move obj to limbo in physics to make sure it wont collide with anything
//				Oyster::Physics::API::Instance().MoveToLimbo(obj);
				weapon->heldObject = obj; //weapon now holds the object
				weapon->hasObject = true;

				break;
			}
			
		}
		

	}