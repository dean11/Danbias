#include "PhysicsAPI.h"
#include "Object.h"
#include "DynamicObject.h"
#include "Player.h"
#include "Level.h"
#include "AttatchmentMassDriver.h"
#include "AttatchmentGun.h"
#include "Game.h"
#include "CollisionManager.h"
#include "JumpPad.h"
#include "Portal.h"
#include "ExplosiveCrate.h"

#include "PickupSystem/PickupHealth.h"

using namespace Oyster;
using namespace GameLogic;

const float default_jump_pad_stun_duration = 1.0f;

	void PlayerVObject(Player &player, Object &obj, Oyster::Math::Float kineticEnergyLoss);
	void PlayerVLethalObject(Player &player, Object &obj, Oyster::Math::Float kineticEnergyLoss, Oyster::Math::Float ExtraDamage);
	void SendObjectFlying(Oyster::Physics::ICustomBody &obj, Oyster::Math::Float3 force);
	void Teleport(Oyster::Physics::ICustomBody &obj, Oyster::Math::Float3 target);

	void Player::PlayerCollision(Oyster::Physics::ICustomBody *objA, Oyster::Physics::ICustomBody *objB, Oyster::Math::Float kineticEnergyLoss)
	{
		Object *realObjA = (Object*)objA->GetCustomTag();
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
			PlayerVObject(*player,*realObjB, 50.0f);
			break;
		case ObjectSpecialType::ObjectSpecialType_StandardBox:
			PlayerVObject(*player,*realObjB, 50.0f);
			((Game*)&Game::Instance())->onCollisionEventFnc(player, CollisionEvent::CollisionEvent_BoxVsPlayer);
			break;
		case ObjectSpecialType::ObjectSpecialType_Player:
			PlayerVObject(*player,*realObjB, 150.0f);
			break;
		case ObjectSpecialType::ObjectSpecialType_World:
			PlayerVObject(*player,*realObjB, NoEdgeConstants::Values::Limits::FallDamageThreshold);
			break;
		case ObjectSpecialType::ObjectSpecialType_CrystalFormation:
			PlayerVLethalObject(*player,*realObjB, kineticEnergyLoss,realObjB->GetExtraDamageOnCollision());
			((Game*)&Game::Instance())->onCollisionEventFnc(player, CollisionEvent::CollisionEvent_CrystalVsPlayer);
			break;
		}
		// send collision event message
		
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
			{
				Player *player = (Player*)realObj;
				player->Stun( default_jump_pad_stun_duration );
				SendObjectFlying(*obj, jumpPad->pushForce);
				((Game*)&Game::Instance())->onCollisionEventFnc(player, CollisionEvent::CollisionEvent_JumpPad);
			}
			break;
		}
	}

	void SendObjectFlying(Oyster::Physics::ICustomBody &obj, Oyster::Math::Float3 force)
	{
		obj.ApplyImpulse(force);
	}

	void Portal::PortalActivated(Oyster::Physics::ICustomBody *objA, Oyster::Physics::ICustomBody *objB, Oyster::Math::Float kineticEnergyLoss)
	{
		Object *realObjA = (Object*)objA->GetCustomTag();
		Portal *portal;
		Object *realObjB = (Object*)objB->GetCustomTag();

		if(!realObjA)	
			return;
		if(!realObjB)		
			return;

		//check who is player and who is the object
		if(realObjA->GetObjectType() == ObjectSpecialType::ObjectSpecialType_Portal)
		{
			portal = (Portal*)realObjA;
		}
		else
		{
			portal = (Portal*)realObjB;
			realObjB = realObjA;
		}
		if(realObjB->GetRigidBody()->GetState().mass == 0) return;



		Teleport(*realObjB->GetRigidBody(),portal->portalExit);
		((Game*)&Game::Instance())->onCollisionEventFnc(realObjB, CollisionEvent::CollisionEvent_Portal);
	}

	void Teleport(Oyster::Physics::ICustomBody &obj, Oyster::Math::Float3 target)
	{
		obj.SetPosition(target);
		
	}

	void ExplosiveCrate::ExplosiveCrateCollision(Oyster::Physics::ICustomBody *objA, Oyster::Physics::ICustomBody *objB, Oyster::Math::Float kineticEnergyLoss)
	{
		Object *realObjA = ((Object*)(objA->GetCustomTag()));
		Object *realObjB = (Object*)objB->GetCustomTag();
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
			((Game*)&Game::Instance())->onCollisionEventFnc(crate, CollisionEvent::CollisionEvent_Explosion);
			break;
		}
	}
	
	void ExplosiveCrate::Explode(Oyster::Physics::ICustomBody *obj, void* args)
	{
		Object *realObj = (Object*)obj->GetCustomTag();
		ExplosiveCrate* ExplosionSource = ((ExplosiveCrate*)args);

		Oyster::Math::Float3 explosionCenterPos = ExplosionSource->GetPosition();
		Oyster::Math::Float3 hitObjectPos = obj->GetState().centerPos;
		Oyster::Math::Float3 force = (((hitObjectPos - explosionCenterPos).GetNormalized()) * ExplosionSource->pushForceMagnitude);


		if(realObj->GetObjectType() == ObjectSpecialType::ObjectSpecialType_Player)
		{
			Player *hitPlayer = (Player*)realObj;
			hitPlayer->DamageLife(ExplosionSource->extraDamageOnCollision);
			obj->SetLinearVelocity(Oyster::Math::Float3::null);
			obj->ApplyImpulse(force * ExplosionSource->pushForceMagnitude);
			hitPlayer->Stun(0.2f);
			//do shredding damage
		}
		else if(realObj->GetObjectType() != ObjectSpecialType::ObjectSpecialType_World)
		{
			Object *hitObject = (Object*)realObj;
			obj->ApplyImpulse(force * ExplosionSource->pushForceMagnitude);
			//do shredding damage
		}
		//send message that box has exploded?
		ExplosionSource->RemoveAffectedBy();
		ExplosionSource->RemoveManipulation();

		Oyster::Physics::API::Instance().MoveToLimbo(ExplosionSource->GetRigidBody());
		((Game*)&Game::Instance())->onDisableFnc(ExplosionSource);
	}

	void PlayerVObject(Player &player, Object &obj, Oyster::Math::Float a)
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
		Oyster::Math::Float damageFactor = 0.1f;


		Oyster::Math::Float damageDone = 0.0f;

		if(impactPower > a) //should only take damage if the force is high enough
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
		PlayerVObject(player, obj, kineticEnergyLoss);
		// always take extra DMG when in contact with leathal object
		player.DamageLife(ExtraDamage);


		Oyster::Math::Float3 objectPos = obj.GetPosition();
		Oyster::Math::Float3 playerPos = player.GetRigidBody()->GetState().centerPos - player.GetRigidBody()->GetOrientation()[1];
		Oyster::Math::Float3 force = (((playerPos - objectPos).GetNormalized()));

		player.GetRigidBody()->SetLinearVelocity(Oyster::Math::Float3::null);
		player.GetRigidBody()->ApplyImpulse(force * 3000);
		player.Stun(0.2f);

	}

	Oyster::Physics::ICustomBody::SubscriptMessage Object::DefaultOnCollision(Oyster::Physics::ICustomBody *rigidBodyObject, Oyster::Physics::ICustomBody *obj, Oyster::Math::Float kineticEnergyLoss)
	{
		return Physics::ICustomBody::SubscriptMessage_none;
	}

	void DynamicObject::DynamicDefaultOnCollision(Oyster::Physics::ICustomBody *objA, Oyster::Physics::ICustomBody *objB, Oyster::Math::Float kineticEnergyLoss)
	{
		
		DynamicObject *realObjA = dynamic_cast<DynamicObject*>((Object*)objA->GetCustomTag());
		
		DynamicObject *realObjB = dynamic_cast<DynamicObject*>((Object*)objB->GetCustomTag());

		if(!realObjA || !realObjB) // one of the objects cannot be cast into a dynamicObject and so we leave the function
		{
			return;
		}

		Math::Float3 deltaVelocity = realObjA->GetRigidBody()->GetLinearVelocity() - realObjB->GetRigidBody()->GetLinearVelocity();
		Math::Float velocityNorm = deltaVelocity.Dot( deltaVelocity );

		static const Math::Float velocity_norm_threshold_interrupt_weapon = 3600.0f; // 60 m/s deltaVelocity
		if( velocityNorm >= velocity_norm_threshold_interrupt_weapon )
		{
			Player *player = realObjA->getManipulatingPlayer();
			if( player != nullptr )
			{
				player->UseWeapon( WEAPON_INTERRUPT );
			}

			player = realObjB->getManipulatingPlayer();
			if( player != nullptr )
			{
				player->UseWeapon( WEAPON_INTERRUPT );
			}
		}


		//check which obj is the one that is already affected, if both are then use the special case of changing ownership.
		if(realObjA->getAffectingPlayer() == NULL && realObjB->getAffectingPlayer() == NULL) //None of the objects have a player affecting them
		{
			return;//leave function as the are not to transfer any ownership
		}

		if(realObjA->getAffectingPlayer() != NULL && realObjB->getAffectingPlayer() == NULL)
		{
			//realobjA is the affectedObject, transfer this to realobjB
			realObjB->SetAffectedBy(*realObjA->getAffectingPlayer());
			return;

		}
		if(realObjB->getAffectingPlayer() != NULL && realObjA->getAffectingPlayer() == NULL)
		{
			//realobjB is the affectedObject, transfer this to realobjA
			realObjA->SetAffectedBy(*realObjB->getAffectingPlayer());
			return;

		}

		if(realObjA->getAffectingPlayer() != NULL && realObjB->getAffectingPlayer() != NULL && ( realObjA->getAffectingPlayer()->GetID() != realObjB->getAffectingPlayer()->GetID()))
		{
			//Both objects have a player affecting them, now use the special case
			if(realObjA->GetRigidBody()->GetState().previousVelocity.GetMagnitude() > realObjB->GetRigidBody()->GetState().previousVelocity.GetMagnitude() )
			{
				//realObjA is the winner and will change Bs ownership to A
				realObjB->SetAffectedBy(*realObjA->getAffectingPlayer());
			}
			else
			{
				realObjA->SetAffectedBy(*realObjB->getAffectingPlayer());
				//realObjB is the winner and will change As ownership to B
			}
		}
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
		AttatchmentMassDriver *weapon = ((forcePushData*)(args))->weapon;
		Player *player = weapon->owner;

		if(realObj->GetObjectType() == ObjectSpecialType::ObjectSpecialType_Player || realObj->GetObjectType() == ObjectSpecialType::ObjectSpecialType_World)
			return;

		Oyster::Math::Float3 playerPos = player->GetPosition();
		Oyster::Math::Float3 targetPos = realObj->GetPosition();
		Oyster::Math::Float3 forceDir = (targetPos - playerPos).GetNormalized();
		Oyster::Math::Float pushForce = ((forcePushData*)(args))->pushForce;

		if(weapon->hasObject)
		{
			obj->SetLinearVelocity(player->GetRigidBody()->GetLinearVelocity());
		}

		obj->ApplyImpulse(forceDir * pushForce);
		

		DynamicObject *dynamicObj = dynamic_cast<DynamicObject*>(realObj);
		
		if(dynamicObj)
		{
			dynamicObj->SetAffectedBy(*player);
		}
	}

	void AttatchmentMassDriver::AttemptPickUp(Oyster::Physics::ICustomBody *obj, void* args)
	{
		if(obj->GetState().mass == 0) return;
		if(!args) return;

		AttatchmentMassDriver *weapon = ((AttatchmentMassDriver*)args);

		if(weapon->hasObject)
		{
			//do nothing
		}
		else
		{
			Object* realObj = (Object*)(obj->GetCustomTag());
			//check so that it is an object that you can pickup

			DynamicObject *dynamicObj = dynamic_cast<DynamicObject*>(realObj);
			if(!dynamicObj) return;
		
			if(dynamicObj->getManipulatingPlayer() != NULL)
			{
				return;
			}

			switch(dynamicObj->GetObjectType())
			{
			case ObjectSpecialType::ObjectSpecialType_StandardBox:
				weapon->heldObject = obj; //weapon now holds the object
				weapon->hasObject = true;
				dynamicObj->SetManipulatingPlayer(*weapon->owner); //TODO: add if this is to be a struggle of who has the most power in its weapon, the player that is already manipulating the object or you. if you then you take the object from the other player, if not then you do not take the object

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
			//Only update if it is active. And if the player is alive
			if(((Pickup*)a)->IsActive() && ((Player*)b)->GetState() != PLAYER_STATE_DEAD && ((Player*)b)->GetState() != PLAYER_STATE_DIED)
			{
				((Pickup*)a)->OnCollision((Player*)(b));
			}
			return;
		}
		else if(a->GetObjectType() != ObjectSpecialType_Player)
		{
			//One of the objects are not a player.
			//Do nothing.
			return;
		}
		
		//Only update if it is active. And if the player is alive
		if(((Pickup*)b)->IsActive() && ((Player*)a)->GetState() != PLAYER_STATE_DEAD && ((Player*)a)->GetState() != PLAYER_STATE_DIED)
		{
			((Pickup*)b)->OnCollision((Player*)a);
		}
	}