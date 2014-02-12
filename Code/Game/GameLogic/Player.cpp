
#include "Player.h"
#include "Weapon.h"
#include "CollisionManager.h"
#include "Game.h"

using namespace GameLogic;
using namespace Oyster::Physics;
const int MOVE_FORCE = 30;
const float KEY_TIMER = 0.04f;
Player::Player()
	:DynamicObject()
{

}
Player::Player(OBJECT_TYPE type)
	:DynamicObject(type)
{
	InitPlayer();
}
Player::Player(Oyster::Physics::ICustomBody *rigidBody, OBJECT_TYPE type)
	:DynamicObject(rigidBody,type)
{
	InitPlayer();
}
Player::Player(void* collisionFuncBefore, void* collisionFuncAfter, OBJECT_TYPE type)
	:DynamicObject(collisionFuncBefore,collisionFuncAfter,type)
{
	InitPlayer();
}
Player::Player(Oyster::Physics::ICustomBody *rigidBody ,void* collisionFuncBefore, void* collisionFuncAfter, OBJECT_TYPE type)
	:DynamicObject(rigidBody, collisionFuncBefore, collisionFuncAfter, type)
{
	InitPlayer();
}
Player::Player(Oyster::Physics::ICustomBody *rigidBody ,Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncBefore)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter), Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncAfter)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), OBJECT_TYPE type)
	:DynamicObject(rigidBody, collisionFuncBefore, collisionFuncAfter, type)
{
	InitPlayer();
}

void Player::InitPlayer()
{
	weapon = new Weapon(2,this);

	this->life = 100;
	this->teamID = -1;
	this->playerState = PLAYER_STATE_IDLE;
	this->lookDir = Oyster::Math::Float3(0,0,-1);
	this->moveDir = Oyster::Math::Float3(0,0,0);
	key_forward = 0;
	key_backward = 0;
	key_strafeRight = 0;
	key_strafeLeft = 0;
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
	Oyster::Math::Float3 forward(0,0,0); 
	Oyster::Math::Float3 back(0,0,0); 
	Oyster::Math::Float3 right(0,0,0); 
	Oyster::Math::Float3 left(0,0,0); 
	Oyster::Math::Float3 moveDirection(0,0,0);
	if (key_forward > 0.001)
	{
		key_forward -= gameInstance->GetFrameTime();
		forward = this->rigidBody->GetState().GetOrientation().v[2].GetNormalized();
	}
	if (key_backward > 0.001)
	{
		key_backward -= gameInstance->GetFrameTime();
		back = -this->rigidBody->GetState().GetOrientation().v[2].GetNormalized();
	}
	if (key_strafeRight > 0.001)
	{

		key_strafeRight -= gameInstance->GetFrameTime();
		Oyster::Math::Float3 forward = this->rigidBody->GetState().GetOrientation().v[2].GetNormalized();
		Oyster::Math::Float3 up = this->rigidBody->GetState().centerPos.Normalize();
		right = -((up).Cross(forward).Normalize());
		right.Normalize();
	}
	if (key_strafeLeft > 0.001)
	{
		key_strafeLeft -= gameInstance->GetFrameTime();
		Oyster::Math::Float3 forward = this->rigidBody->GetState().GetOrientation().v[2].GetNormalized();
		Oyster::Math::Float3 up = this->rigidBody->GetState().centerPos.Normalize();
		left = (up).Cross(forward).Normalize();
		left.Normalize();
	}
	moveDirection = forward + back + left + right;
	//moveDirection.Normalize();
	rigidBody->SetLinearVelocity( MOVE_FORCE * moveDirection );

	weapon->Update(0.01f);
}

void Player::EndFrame()
{
	// snap to axis
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

void Player::Rotate(const Oyster::Math3D::Float4 lookDir)
{
	// this is the camera right vector
	this->lookDir = lookDir.xyz;

	Oyster::Math::Float3 up = this->rigidBody->GetState().GetOrientation().v[1];
	this->rigidBody->SetUpAndRight(up, lookDir.xyz);
	this->rigidBody->SetUpAndRight(this->rigidBody->GetState().centerPos.GetNormalized(), this->rigidBody->GetState().GetOrientation().v[0].xyz.GetNormalized());
}

void Player::Jump()
{
	Oyster::Math::Float3 up = this->rigidBody->GetState().GetOrientation().v[1].GetNormalized();
	this->rigidBody->ApplyImpulse(up *1500);
	this->playerState == PLAYER_STATE::PLAYER_STATE_JUMPING;
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

