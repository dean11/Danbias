
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

	this->life = 100;
	this->teamID = teamID;
	this->playerState = PLAYER_STATE_IDLE;
	this->lookDir = Oyster::Math::Float3(0,0,-1);
	key_forward = 0;
	key_backward = 0;
	key_strafeRight = 0;
	key_strafeLeft = 0;

	this->previousPosition = Oyster::Math::Float3(0,0,0);

	this->moveDir = Oyster::Math::Float3(0,0,0);
	this->moveSpeed = 100;
	this->previousMoveSpeed = Oyster::Math::Float3(0,0,0);
}

Player::Player(Oyster::Physics::ICustomBody *rigidBody, Oyster::Physics::ICustomBody::SubscriptMessage (*EventOnCollision)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, int objectID, int teamID)
	:DynamicObject(rigidBody, EventOnCollision, type, objectID)
{
	weapon = new Weapon(2,this);

	this->life = 100;
	this->teamID = teamID;
	this->playerState = PLAYER_STATE_IDLE;
	this->lookDir = Oyster::Math::Float3(0,0,-1);
	key_forward = 0;
	key_backward = 0;
	key_strafeRight = 0;
	key_strafeLeft = 0;

	this->previousPosition = Oyster::Math::Float3(0,0,0);
	this->moveDir = Oyster::Math::Float3(0,0,0);
	this->moveSpeed = 20;
	this->previousMoveSpeed = Oyster::Math::Float3(0,0,0);
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
	//weapon->Update(0.002f); 
	Object::BeginFrame();

	Oyster::Math::Float maxSpeed = 30;

	Oyster::Math::Float4x4 xform;
	xform = this->rigidBody->GetState().GetOrientation();

	/* Handle turning */
	/*if (left)
		m_turnAngle -= dt * m_turnVelocity;
	if (right)
		m_turnAngle += dt * m_turnVelocity;*/

	//xform.setRotation (btQuaternion (btVector3(0.0, 1.0, 0.0), m_turnAngle));

	Oyster::Math::Float3 linearVelocity = this->rigidBody->GetLinearVelocity();
	Oyster::Math::Float speed = this->rigidBody->GetLinearVelocity().GetLength();

	Oyster::Math::Float3 forwardDir = xform.v[2];
	Oyster::Math::Float3 rightDir = xform.v[0];
	forwardDir.Normalize();
	rightDir.Normalize();
	Oyster::Math::Float3 walkDirection = Oyster::Math::Float3(0.0, 0.0, 0.0);
	Oyster::Math::Float walkSpeed = this->moveSpeed*0.2f;

	if (key_forward > 0.001)
	{
		key_forward -= gameInstance->GetFrameTime();
		walkDirection += forwardDir;
		walkDirection.Normalize();
	}
	if (key_backward > 0.001)
	{
		key_backward -= gameInstance->GetFrameTime();
		walkDirection -= forwardDir;
		walkDirection.Normalize();
	}
	if (key_strafeRight > 0.001)
	{
		key_strafeRight -= gameInstance->GetFrameTime();
		walkDirection -= rightDir;
		walkDirection.Normalize();
	}
	if (key_strafeLeft > 0.001)
	{
		key_strafeLeft -= gameInstance->GetFrameTime();
		walkDirection += rightDir;
		walkDirection.Normalize();
		maxSpeed = 40;
	}
	

	if (key_forward <= 0.001 && key_backward <= 0.001 && key_strafeRight <= 0.001 && key_strafeLeft <= 0.001 && key_jump <= 0.001 && this->rigidBody->GetLambda() < 0.7f)
	{
		/* Dampen when on the ground and not being moved by the player */
		linearVelocity *= 0.2f;
		this->rigidBody->SetLinearVelocity (linearVelocity);
	} 
	else 
	{
		if (speed < maxSpeed && this->rigidBody->GetLambda() < 1.0f)
		{
			Oyster::Math::Float3 velocity = linearVelocity + walkDirection * walkSpeed;
			this->rigidBody->SetLinearVelocity(velocity);
		}
		else if(speed < maxSpeed)
		{
			Oyster::Math::Float3 velocity = linearVelocity + (walkDirection * walkSpeed)*0.2f;
			this->rigidBody->SetLinearVelocity(velocity);
		}
	}

	if (key_jump > 0.001)
	{
		this->key_jump -= this->gameInstance->GetFrameTime();
		if(this->rigidBody->GetLambda() < 1.0f)
		{
			Oyster::Math::Float3 up = this->rigidBody->GetState().GetOrientation().v[1].GetNormalized();
			this->rigidBody->ApplyImpulse(up*this->rigidBody->GetState().mass*20);
			this->playerState = PLAYER_STATE::PLAYER_STATE_JUMPING;
		}
	}

	this->weapon->Update(0.01f);
}

void Player::EndFrame()
{
	// snap to axis
	Oyster::Math::Float4 rotation;
	
	this->rigidBody->SetUp(this->rigidBody->GetState().centerPos.GetNormalized());
	
	
	Object::EndFrame();	
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
	this->life = 100;
	this->playerState = PLAYER_STATE::PLAYER_STATE_IDLE;
	this->lookDir = Oyster::Math::Float4(1,0,0);
	this->rigidBody->SetPosition(spawnPoint);
}

void Player::Rotate(const Oyster::Math3D::Float3 lookDir, const Oyster::Math3D::Float3 right)
{
	// this is the camera right vector
	this->lookDir = lookDir;

	//Oyster::Math::Float3 up = this->rigidBody->GetState().GetOrientation().v[1];
	//this->rigidBody->SetUpAndRight(up, right);
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
	this->life = 0;

	if(this->life <= 0)
	{
		this->life = 0;
		playerState = PLAYER_STATE_DEAD;
		this->gameInstance->onDisableFnc(this, 0.0f);
	}
}
