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

#include "PickupSystem/PickupHealth.h"

using namespace Oyster;

using namespace GameLogic;

	void PlayerVObject(Player &player, Object &obj, Oyster::Math::Float kineticEnergyLoss);
	void PlayerVLethalObject(Player &player, Object &obj, Oyster::Math::Float kineticEnergyLoss, Oyster::Math::Float ExtraDamage);
	void SendObjectFlying(Oyster::Physics::ICustomBody &obj, Oyster::Math::Float3 force);
	void Teleport(Oyster::Physics::ICustomBody &obj, Oyster::Math::Float3 target);

	//Physics::ICustomBody::SubscriptMessage
	void Player::PlayerCollision(Oyster::Physics::ICustomBody *objA, Oyster::Physics::ICustomBody *objB, Oyster::Math::Float kineticEnergyLoss)
	{
		Object *realObjA = ((Object*)(objA->GetCustomTag()));
		Player *player;
		Object *realObjB = (Object*)objB->GetCustomTag(); //needs to be changed?

		if(!realObjA)	
			return;
		if(!realObjB)		
			return;

		//check who is player and who is the object
		if(realObjA->GetObjectType() == ObjectSpecialType::ObjectSpecialType_Player)
		{
			player = (Player*)realObjA;
		}
		else
		{
			player = (Player*)realObjB;
			realObjB = realObjA;
		}


		switch (realObjB->GetObjectType())
		{
		case ObjectSpecialType::ObjectSpecialType_Generic:
			PlayerVObject(*player,*realObjB, kineticEnergyLoss);
			//return Physics::ICustomBody::SubscriptMessage_none;
			break;
		
		case ObjectSpecialType::ObjectSpecialType_StandardBox:
			PlayerVObject(*player,*realObjB, kineticEnergyLoss);
			//return Physics::ICustomBody::SubscriptMessage_none;
			break;
		case ObjectSpecialType::ObjectSpecialType_Player:
			//return Physics::ICustomBody::SubscriptMessage_none;
			break;
		case ObjectSpecialType::ObjectSpecialType_World:
			PlayerVObject(*player,*realObjB, kineticEnergyLoss);
			//player->playerState = PLAYER_STATE::PLAYER_STATE_WALKING;
			break;

		case ObjectSpecialType::ObjectSpecialType_CrystalFormation:
			PlayerVLethalObject(*player,*realObjB, kineticEnergyLoss,realObjB->GetExtraDamageOnCollision());
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

	void ExplosiveCrate::ExplosiveCrateCollision(Oyster::Physics::ICustomBody *objA, Oyster::Physics::ICustomBody *objB, Oyster::Math::Float kineticEnergyLoss)
	{
		

		Object *realObjA = ((Object*)(objA->GetCustomTag()));
		Object *realObjB = (Object*)objB->GetCustomTag(); //needs to be changed?
		ExplosiveCrate* crate;

		if(!realObjA)	
			return;
		if(!realObjB)		
			return;

		//check who is player and who is the object
		if(realObjA->GetObjectType() == ObjectSpecialType::ObjectSpecialType_RedExplosiveBox)
		{
			crate = (ExplosiveCrate*)realObjA;
		}
		else
		{
			crate = (ExplosiveCrate*)realObjB;
			realObjB = realObjA;
		}

		switch (realObjB->GetObjectType())
		{
		case ObjectSpecialType::ObjectSpecialType_Generic:
			break;
		case ObjectSpecialType::ObjectSpecialType_StandardBox:
			
			break;
		case ObjectSpecialType::ObjectSpecialType_Player:
			if(crate->hasExploaded) return;
			Oyster::Math::Float3 pos = crate->GetRigidBody()->GetState().centerPos;
			Oyster::Collision3D::Sphere *hitSphere = new Oyster::Collision3D::Sphere(pos,crate->ExplosionRadius);

			Oyster::Physics::API::Instance().ApplyEffect(hitSphere,crate,Explode);
			crate->hasExploaded = true;
			delete hitSphere;
			break;
		}
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
			hitPlayer->GetRigidBody()->ApplyImpulse(force);
			//do shredding damage
		}

		

	}

	void PlayerVObject(Player &player, Object &obj, Oyster::Math::Float kineticEnergyLoss)
	{
		//Collision between a player and a general static or dynamic object
		//use kinetic energyloss of the collision in order too determin how much damage to take
		//use as part of the damage algorithm
		Oyster::Math::Float3 objPrevVel = obj.GetRigidBody()->GetState().previousVelocity;
		Oyster::Math::Float3 playerPrevVel = player.GetRigidBody()->GetState().previousVelocity;

		Oyster::Math::Float3 deltaPos = (player.GetPosition() - obj.GetPosition());
		Oyster::Math::Float deltaSpeed = (objPrevVel - playerPrevVel).GetMagnitude();
		Oyster::Math::Float angularFactor = deltaPos.GetNormalized().Dot( (objPrevVel - playerPrevVel).GetNormalized());

		Oyster::Math::Float impactPower = deltaSpeed * angularFactor;
		Oyster::Math::Float damageFactor = 0.01f;


		int damageDone = 0;
		int forceThreashHold = 30; //FIX: balance this

		if(impactPower > forceThreashHold) //should only take damage if the force is high enough
		{
			if(obj.GetRigidBody()->GetState().mass == 0)
			{
				damageDone = impactPower * damageFactor;
			}
			else
			{
				damageDone = (impactPower * obj.GetRigidBody()->GetState().mass)* damageFactor;
			}
			
			player.DamageLife(damageDone);
		}
		
	}	

	
	void PlayerVLethalObject(Player &player, Object &obj, Oyster::Math::Float kineticEnergyLoss, Oyster::Math::Float ExtraDamage)
	{
		Oyster::Math::Float damageDone = 0;
		Oyster::Math::Float forceThreashHold = 200000;

		if(kineticEnergyLoss > forceThreashHold) //should only take damage if the force is high enough
		{
			damageDone = (kineticEnergyLoss * 0.10f);
			damageDone += ExtraDamage;
			//player.DamageLife(damageDone);
		}
	}

	Oyster::Physics::ICustomBody::SubscriptMessage Object::DefaultOnCollision(Oyster::Physics::ICustomBody *rigidBodyObject, Oyster::Physics::ICustomBody *obj, Oyster::Math::Float kineticEnergyLoss)
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

	//General collision collision for pickups
	//It calls the collision function defined in each pickup.
	void Pickup::PickupCollision(Oyster::Physics::ICustomBody* objA, Oyster::Physics::ICustomBody* objB, Oyster::Math::Float kineticEnergyLoss)
	{
		//Check if player is a player.
		Object* a = (Object*)objA->GetCustomTag();
		Object* b = (Object*)objB->GetCustomTag();

		if(!a)	
			return;
		if(!b)		
			return;

		if(b->GetObjectType() == ObjectSpecialType_Player)
		{
			((Pickup*)a)->OnCollision((Player*)(b));
		}
		else if(a->GetObjectType() != ObjectSpecialType_Player)
		{
			//One of the objects are not a player.
			//Do nothing.
			return;
		}

		((Pickup*)b)->OnCollision((Player*)a);
	}