
#include "Player.h"
#include "Weapon.h"
#include "CollisionManager.h"
#include "Game.h"

using namespace GameLogic;
using namespace Oyster::Physics;
const float MOVE_FORCE = 30;
const float KEY_TIMER = 0.03f;
Player::Player()
	:DynamicObject()
{

}

Player::Player(Oyster::Physics::ICustomBody *rigidBody, void (*EventOnCollision)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, int objectID, int teamID)
	:DynamicObject(rigidBody, EventOnCollision, type, objectID)
{
	weapon = new Weapon(2,this);
	AffectedObjects.Reserve(15);
	

	this->life = 100;
	this->teamID = teamID;
	this->playerState = PLAYER_STATE_IDLE;
	this->lookDir = Oyster::Math::Float3(0,0,-1);
	key_forward = 0;
	key_backward = 0;
	key_strafeRight = 0;
	key_strafeLeft = 0;
	key_jump = 0;
	invincibleCooldown = 0;
	this->deathTimeLeft = 0;
	this->deathTime		= 5;

	this->previousPosition = Oyster::Math::Float3(0,0,0);

	this->moveDir = Oyster::Math::Float3(0,0,0);
	this->moveSpeed = 100;
	this->previousMoveSpeed = Oyster::Math::Float3(0,0,0);

	this->rotationUp = 0;
}

Player::Player(Oyster::Physics::ICustomBody *rigidBody, Oyster::Physics::ICustomBody::SubscriptMessage (*EventOnCollision)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, int objectID, int teamID)
	:DynamicObject(rigidBody, EventOnCollision, type, objectID)
{
	weapon = new Weapon(2,this);

	AffectedObjects.Reserve(15);
	

	this->life = 100;
	this->teamID = teamID;
	this->playerState = PLAYER_STATE_IDLE;
	this->lookDir = Oyster::Math::Float3(0,0,-1);
	key_forward = 0;
	key_backward = 0;
	key_strafeRight = 0;
	key_strafeLeft = 0;
	key_jump = 0;
	invincibleCooldown = 0;
	this->deathTimeLeft = 0;
	this->deathTime		= 5;
	this->previousPosition = Oyster::Math::Float3(0,0,0);
	this->moveDir = Oyster::Math::Float3(0,0,0);
	this->moveSpeed = 20;
	this->previousMoveSpeed = Oyster::Math::Float3(0,0,0);

	this->rotationUp = 0;
}

Player::~Player(void)
{
	if(weapon)
	{
		delete weapon;
		weapon = NULL;
	}	
}

void Player::BeginFrame()
{
	if( this->playerState != PLAYER_STATE_DEAD) 
	{
		weapon->Update(0.002f); 

		Oyster::Math::Float maxSpeed = 30;

		// Rotate player accordingly
		this->rigidBody->SetUp(this->rigidBody->GetState().centerPos.GetNormalized());
		Oyster::Math::Quaternion firstUp = this->rigidBody->GetState().quaternion;
		this->rigidBody->SetRotationAsAngularAxis(Oyster::Math3D::Float4(this->rigidBody->GetState().centerPos.GetNormalized(), this->rotationUp));
		Oyster::Math::Quaternion secondTurn = this->rigidBody->GetState().quaternion;

		this->rigidBody->SetRotation(secondTurn*firstUp);
	
		// Direction data
		Oyster::Math::Float4x4 xform;
		xform = this->rigidBody->GetState().GetOrientation();

		Oyster::Math::Float3 forwardDir = xform.v[2];
		Oyster::Math::Float3 upDir = xform.v[1];
		Oyster::Math::Float3 rightDir = xform.v[0];
		forwardDir.Normalize();
		upDir.Normalize();
		rightDir.Normalize();

		// Previous velocities data
		Oyster::Math::Float3 linearVelocity = this->rigidBody->GetLinearVelocity();
		Oyster::Math::Float3 forwardVelocity = linearVelocity*Oyster::Math::Float3(fabs(forwardDir.x), fabs(forwardDir.y), fabs(forwardDir.z));
		Oyster::Math::Float forwardSpeed = (linearVelocity*forwardDir).GetLength();
		Oyster::Math::Float3 rightVelocity = linearVelocity*Oyster::Math::Float3(fabs(rightDir.x), fabs(rightDir.y), fabs(rightDir.z));
		Oyster::Math::Float rightSpeed = (linearVelocity*rightDir).GetLength();
		Oyster::Math::Float3 upVelocity = linearVelocity*Oyster::Math::Float3(fabs(upDir.x), fabs(upDir.y), fabs(upDir.z));

		// Walking data
		Oyster::Math::Float3 walkDirection = Oyster::Math::Float3(0.0, 0.0, 0.0);
		Oyster::Math::Float walkSpeed = this->moveSpeed*0.2f;

		// Check for input
		if(key_forward > 0.001)
		{
			key_forward -= gameInstance->GetFrameTime();
			walkDirection += forwardDir;
		}
		if(key_backward > 0.001)
		{
			key_backward -= gameInstance->GetFrameTime();
			walkDirection -= forwardDir;
		}
		if(key_strafeRight > 0.001)
		{
			key_strafeRight -= gameInstance->GetFrameTime();
			walkDirection += rightDir;
		}
		if(key_strafeLeft > 0.001)
		{
			key_strafeLeft -= gameInstance->GetFrameTime();
			walkDirection -= rightDir;
		}
	
		// Dampen velocity if certain keys are not pressed
		if(key_jump <= 0.001 && this->rigidBody->GetLambda() < 0.9f)
		{
			if(key_forward <= 0.001 && key_backward <= 0.001)
			{
				forwardVelocity *= Oyster::Math::Float3(0.2f*fabs(forwardDir.x), 0.2f*fabs(forwardDir.y), 0.2f*fabs(forwardDir.z));
			}
			if(key_strafeRight <= 0.001 && key_strafeLeft <= 0.001)
			{
				rightVelocity *= Oyster::Math::Float3(0.2f*fabs(rightDir.x), 0.2f*fabs(rightDir.y), 0.2f*fabs(rightDir.z));
			}
		}
	
		// Walk if walkdirection is something
		if(walkDirection != Oyster::Math::Float3::null)
		{
			walkDirection.Normalize();

			// If on the ground, accelerate normally
			if(this->rigidBody->GetLambda() < 0.9f)
			{
				if(forwardSpeed < maxSpeed)
				{
					forwardVelocity += walkDirection*Oyster::Math::Float3(fabs(forwardDir.x), fabs(forwardDir.y), fabs(forwardDir.z)) * walkSpeed;
				}
				if(rightSpeed < maxSpeed)
				{
					rightVelocity += walkDirection*Oyster::Math::Float3(fabs(rightDir.x), abs(rightDir.y), fabs(rightDir.z)) * walkSpeed;
				}
			}
			// If in the air, accelerate slower
			if(this->rigidBody->GetLambda() >= 0.9f)
			{
				if(forwardSpeed < maxSpeed)
				{
					forwardVelocity += walkDirection*Oyster::Math::Float3(fabs(forwardDir.x), fabs(forwardDir.y), fabs(forwardDir.z)) * walkSpeed*0.2f;
				}
				if(rightSpeed < maxSpeed)
				{
					rightVelocity += walkDirection*Oyster::Math::Float3(fabs(rightDir.x), fabs(rightDir.y), fabs(rightDir.z)) * walkSpeed*0.2f;
				}
			}
		}

		// Adjust velocities so no squaring occurs
		forwardVelocity *= Oyster::Math::Float3(fabs(forwardDir.x), fabs(forwardDir.y), fabs(forwardDir.z));
		rightVelocity *= Oyster::Math::Float3(fabs(rightDir.x), fabs(rightDir.y), fabs(rightDir.z));
		upVelocity *= Oyster::Math::Float3(fabs(upDir.x), fabs(upDir.y), fabs(upDir.z)); 

		this->rigidBody->SetLinearVelocity(forwardVelocity+rightVelocity+upVelocity);

		//Jump
		if(key_jump > 0.001)
		{
			this->key_jump -= this->gameInstance->GetFrameTime();
			if(this->rigidBody->GetLambda() < 0.9f)
			{
				Oyster::Math::Float3 up = this->rigidBody->GetState().centerPos.GetNormalized();
				this->rigidBody->ApplyImpulse(up*this->rigidBody->GetState().mass*20);
				this->playerState = PLAYER_STATE::PLAYER_STATE_JUMPING;
			}
		}
	}
	else
	{
		// player is dead 
		// TODO move this logic to lvl
		this->deathTimeLeft -= gameInstance->GetFrameTime();
		if( this->deathTimeLeft <= 0)
		{
			Respawn( Oyster::Math::Float3( -50, 180, 0)); 
		}
	}
}

void Player::EndFrame()
{
	//check if there are any objects that can be removed from the AffectedObjects list
	for(int i = 0; i < this->AffectedObjects.Size(); i++)
	{
		if(this->AffectedObjects[i] && (this->AffectedObjects[i]->GetRigidBody()->GetState().previousVelocity).GetMagnitude() <= 0.1f)
		{
			this->AffectedObjects[i]->RemoveAffectedBy();
			this->AffectedObjects.Remove(i);
		}

	}

}

void Player::Move(const PLAYER_MOVEMENT &movement)
{
	switch(movement)
	{
	case PLAYER_MOVEMENT::PLAYER_MOVEMENT_FORWARD:
			MoveForward();
			break;

	case PLAYER_MOVEMENT::PLAYER_MOVEMENT_BACKWARD:
			MoveBackwards();
			break;

	case PLAYER_MOVEMENT::PLAYER_MOVEMENT_LEFT:
			MoveLeft();
			break;

	case PLAYER_MOVEMENT::PLAYER_MOVEMENT_RIGHT:
			MoveRight();
			break;

	case PLAYER_MOVEMENT::PLAYER_MOVEMENT_JUMP:
			Jump();
			break;
	}
}

void Player::MoveForward()
{
	key_forward = KEY_TIMER;
}
void Player::MoveBackwards()
{
	key_backward = KEY_TIMER;
}
void Player::MoveRight()
{
	key_strafeRight = KEY_TIMER;
}
void Player::MoveLeft()
{
	key_strafeLeft = KEY_TIMER;
}

void Player::UseWeapon(const WEAPON_FIRE &usage)
{
	this->weapon->Use(usage,gameInstance->GetFrameTime());
}

void Player::Respawn(Oyster::Math::Float3 spawnPoint)
{ 
	if( this->playerState == PLAYER_STATE_DEAD) 
	{
		this->life = 100;
		this->playerState = PLAYER_STATE::PLAYER_STATE_IDLE;
		//this->lookDir = Oyster::Math::Float4(1,0,0);	
		this->rigidBody->SetPosition(spawnPoint);
		this->gameInstance->onRespawnFnc( this, spawnPoint);
		this->gameInstance->onDamageTakenFnc( this, this->life);
	}
}

void Player::SetLookDir(const Oyster::Math3D::Float3& lookDir)
{
	// this is the camera right vector
	this->lookDir = -lookDir;

}
void Player::TurnLeft(Oyster::Math3D::Float deltaRadians)
{
	this->rotationUp += deltaRadians;
}

void Player::Jump()
{
	this->key_jump = KEY_TIMER;
}

bool Player::IsWalking()
{
	return (this->playerState == PLAYER_STATE::PLAYER_STATE_WALKING);
}
bool Player::IsJumping()
{
	return (this->playerState == PLAYER_STATE::PLAYER_STATE_JUMPING);
}
bool Player::IsIdle()
{
	return (this->playerState == PLAYER_STATE::PLAYER_STATE_IDLE);
}

void Player::Inactivate()
{
	//this->
}

Oyster::Math::Float3 Player::GetPosition() const
{
	return (Oyster::Math::Float3) this->rigidBody->GetState().centerPos;
}
Oyster::Math::Float4x4 Player::GetOrientation() const 
{
	return this->rigidBody->GetState().GetOrientation();
}
Oyster::Math::Float3 Player::GetLookDir() const
{
	return this->lookDir;
}
int Player::GetTeamID() const
{
	return this->teamID;
}
PLAYER_STATE Player::GetState() const
{
	return this->playerState;
}

void Player::DamageLife(int damage)
{

	this->life -= damage;
	this->gameInstance->onDamageTakenFnc( this, this->life);

	if(this->life <= 0)
	{
		this->life = 0;
		playerState = PLAYER_STATE_DEAD;
		this->deathTimeLeft = this->deathTime;
		this->gameInstance->onDeadFnc(this, this->deathTimeLeft);
	}

}

void Player::AddAffectedObject(DynamicObject &AffectedObject)
{
	//check if object already exists in the list, if so then do not add
	for(int i = 0; i < AffectedObjects.Size(); i++)
	{
		if(AffectedObjects[i]->GetID() == AffectedObject.GetID())
		{
			//object already exists, exit function
			return;
		}
	}
	//else you add the object to the stack
	AffectedObjects.Push(&AffectedObject);
}
