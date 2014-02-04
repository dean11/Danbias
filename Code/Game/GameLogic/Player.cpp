
#include "Player.h"
#include "Weapon.h"
#include "CollisionManager.h"
#include "Game.h"

using namespace GameLogic;
using namespace Oyster::Physics;

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
	lookDir = Oyster::Math::Float4(0,0,-1,0);
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
	weapon->Update(0.002f);
	//if(playerState == PLAYER_STATE_DEAD) Respawn(Oyster::Math::Float3(0,308,0));
	Object::BeginFrame();
}

void Player::EndFrame()
{
	
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
	Oyster::Math::Float3 forward = currPhysicsState.GetOrientation().v[2];
	//Oyster::Math::Float3 forward = lookDir;
	newPhysicsState.ApplyLinearImpulse(forward * (30000 * this->gameInstance->GetFrameTime()));
}
void Player::MoveBackwards()
{
	Oyster::Math::Float3 forward = currPhysicsState.GetOrientation().v[2];
	//Oyster::Math::Float3 forward = lookDir;
	newPhysicsState.ApplyLinearImpulse(-forward * 30000 * this->gameInstance->GetFrameTime());
}
void Player::MoveRight()
{
	//Do cross product with forward vector and negative gravity vector
	Oyster::Math::Float3 forward = currPhysicsState.GetOrientation().v[2];
	//Oyster::Math::Float3 forward = lookDir;
	Oyster::Math::Float3 r = (-currPhysicsState.GetGravityNormal()).Cross(forward);
	newPhysicsState.ApplyLinearImpulse(-r * 30000 * this->gameInstance->GetFrameTime());
	
}
void Player::MoveLeft()
{
	//Do cross product with forward vector and negative gravity vector
	Oyster::Math::Float3 forward = currPhysicsState.GetOrientation().v[2];
	//Oyster::Math::Float3 forward = lookDir;
	Oyster::Math::Float3 r = (-currPhysicsState.GetGravityNormal()).Cross(forward);	//Still get zero
	newPhysicsState.ApplyLinearImpulse(r * 30000 * this->gameInstance->GetFrameTime());
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
	this->newPhysicsState.SetCenterPosition(spawnPoint);
}

void Player::Rotate(const Oyster::Math3D::Float4 lookDir)
{
	Oyster::Math::Float dx = lookDir.w;
	if(dx > 0.0f)
	{
		int i =0 ;
	}
	Oyster::Math::Float3 up = currPhysicsState.GetOrientation().v[1];
	Oyster::Math::Float3 deltaAxis = up * (-dx * 0.02) ;
	Oyster::Math::Float3 oldOrt = currPhysicsState.GetRotation();

	newPhysicsState.SetRotation(oldOrt + deltaAxis);

	this->lookDir = lookDir.xyz;
}

void Player::Jump()
{
	Oyster::Math::Float3 up = currPhysicsState.GetOrientation().v[1];
	newPhysicsState.ApplyLinearImpulse(up * 30000 * this->gameInstance->GetFrameTime());
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
	return (Oyster::Math::Float3)currPhysicsState.GetCenterPosition();
}
Oyster::Math::Float4x4 Player::GetOrientation() const 
{
	return this->currPhysicsState.GetOrientation();
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
		//do stuff that makes you dead
	}
}

