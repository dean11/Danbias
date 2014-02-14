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
#include "ExplosiveCrate.h"

using namespace Oyster;

using namespace GameLogic;

	void PlayerVObject(Player &player, Object &obj, Oyster::Math::Float kineticEnergyLoss);
	void PlayerVLethalObject(Player &player, Object &obj, Oyster::Math::Float kineticEnergyLoss, Oyster::Math::Float ExtraDamage);
	void SendObjectFlying(Oyster::Physics::ICustomBody &obj, Oyster::Math::Float3 force);
	void Teleport(Oyster::Physics::ICustomBody &obj, Oyster::Math::Float3 target);

	//Physics::ICustomBody::SubscriptMessage
	void Player::PlayerCollision(Oyster::Physics::ICustomBody *rigidBodyPlayer, Oyster::Physics::ICustomBody *obj, Oyster::Math::Float kineticEnergyLoss)
	{
		
		Player *player = ((Game::PlayerData*)(rigidBodyPlayer->GetCustomTag()))->player;
		Object *realObj = (Object*)obj->GetCustomTag(); //needs to be changed?

		switch (realObj->GetObjectType())
		{
		case ObjectSpecialType::ObjectSpecialType_Generic:
			PlayerVObject(*player,*realObj, kineticEnergyLoss);
			//return Physics::ICustomBody::SubscriptMessage_none;
			break;
		
		case ObjectSpecialType::ObjectSpecialType_StandardBox:
			PlayerVObject(*player,*realObj, kineticEnergyLoss);
			//return Physics::ICustomBody::SubscriptMessage_none;
			break;
		case ObjectSpecialType::ObjectSpecialType_Player:
			//return Physics::ICustomBody::SubscriptMessage_none;
			break;
		case ObjectSpecialType::ObjectSpecialType_World:
			PlayerVObject(*player,*realObj, kineticEnergyLoss);
			//player->playerState = PLAYER_STATE::PLAYER_STATE_WALKING;
			break;

		case ObjectSpecialType::ObjectSpecialType_CrystalFormation:
			PlayerVLethalObject(*player,*realObj, kineticEnergyLoss,realObj->getExtraDamageOnCollision());
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
		case ObjectSpecialType::ObjectSpecialType_Generic:
			break;
		case ObjectSpecialType::ObjectSpecialType_StandardBox:
			SendObjectFlying(*obj, jumpPad->pushForce);
			break;
		case ObjectSpecialType::ObjectSpecialType_Player:
			SendObjectFlying(*obj, jumpPad->pushForce);
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

	void ExplosiveCrate::ExplosiveCrateCollision(Oyster::Physics::ICustomBody *rigidBodyCrate, Oyster::Physics::ICustomBody *obj, Oyster::Math::Float kineticEnergyLoss)
	{
		int forceThreashHold = 200000; //how much force for the box to explode of the impact

		Object *realObj = (Object*)obj->GetCustomTag(); //needs to be changed?

		switch (realObj->GetObjectType())
		{
		case ObjectSpecialType::ObjectSpecialType_Generic:
			break;
		case ObjectSpecialType::ObjectSpecialType_StandardBox:
			
			break;
		case ObjectSpecialType::ObjectSpecialType_Player:
			ExplosiveCrate* crate = ((ExplosiveCrate*)rigidBodyCrate->GetCustomTag());


			Oyster::Math::Float3 pos = rigidBodyCrate->GetState().centerPos;
			Oyster::Collision3D::Sphere *hitSphere = new Oyster::Collision3D::Sphere(pos,crate->ExplosionRadius);

			Oyster::Physics::API::Instance().ApplyEffect(hitSphere,crate,Explode);

			delete hitSphere;
			break;
		}
		/*if(kineticEnergyLoss > forceThreashHold)
		{
		ExplosiveCrate* crate = ((ExplosiveCrate*)rigidBodyCrate->GetCustomTag());


		Oyster::Math::Float3 pos = rigidBodyCrate->GetState().centerPos;
		Oyster::Collision3D::Sphere *hitSphere = new Oyster::Collision3D::Sphere(pos,crate->ExplosionRadius);

		Oyster::Physics::API::Instance().ApplyEffect(hitSphere,crate,Explode);

		delete hitSphere;
		}*/
	}
	
	void ExplosiveCrate::Explode(Oyster::Physics::ICustomBody *obj, void* args)
	{
		Object *realObj = (Object*)obj->GetCustomTag();
		ExplosiveCrate* ExplosionSource = ((ExplosiveCrate*)args);

		Oyster::Math::Float3 explosionCenterPos = ExplosionSource->GetPosition();
		Oyster::Math::Float3 hitObjectPos = obj->GetState().centerPos;
		Oyster::Math::Float3 force = (((hitObjectPos- explosionCenterPos).GetNormalized()) * ExplosionSource->pushForceMagnitude);
		
		if(realObj->GetObjectType() == ObjectSpecialType::ObjectSpecialType_Player)
		{
			Player *hitPlayer = (Player*)realObj;
			
			//hitPlayer->DamageLife(ExplosionSource->getExtraDamageOnCollision());
			//do shredding damage
		}

		realObj->GetRigidBody()->ApplyImpulse(force);

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

	
	void PlayerVLethalObject(Player &player, Object &obj, Oyster::Math::Float kineticEnergyLoss, Oyster::Math::Float ExtraDamage)
	{
		int damageDone = 0;
		int forceThreashHold = 200000;

		if(kineticEnergyLoss > forceThreashHold) //should only take damage if the force is high enough
		{
			damageDone = (int)(kineticEnergyLoss * 0.10f);
			damageDone += ExtraDamage;
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

		if(realObj->GetObjectType() == ObjectSpecialType::ObjectSpecialType_Player || realObj->GetObjectType() == ObjectSpecialType::ObjectSpecialType_World)
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
			case ObjectSpecialType::ObjectSpecialType_StandardBox:
				weapon->heldObject = obj; //weapon now holds the object
				weapon->hasObject = true;

				break;
			}
			
		}
		

	}