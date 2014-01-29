
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
	setState.ApplyLinearImpulse(this->lookDir * (20 * this->gameInstance->GetFrameTime()));
}
void Player::MoveBackwards()
{
	setState.ApplyLinearImpulse(-this->lookDir * 20 * this->gameInstance->GetFrameTime());
}
void Player::MoveRight()
{
	//Do cross product with forward vector and negative gravity vector
	Oyster::Math::Float3 r = Oyster::Math::Float4(1, 0, 0);
	//Oyster::Math::Float4 r = (-rigidBody->GetGravityNormal()).Cross((Oyster::Math::Float3)this->lookDir);
	setState.ApplyLinearImpulse(r * 20 * this->gameInstance->GetFrameTime());
	
}
void Player::MoveLeft()
{
	//Do cross product with forward vector and negative gravity vector
	Oyster::Math::Float3 r = Oyster::Math::Float4(1, 0, 0 );
	//Oyster::Math::Float4 r1 = -(-rigidBody->GetGravityNormal()).Cross((Oyster::Math::Float3)this->lookDir);	//Still get zero
	setState.ApplyLinearImpulse(-r * 20 * this->gameInstance->GetFrameTime());
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
}

void Player::Rotate(const Oyster::Math3D::Float3 lookDir)
{
	this->lookDir = lookDir;
		
	Oyster::Math::Float4 up(0,1,0,0);//-setState.GetGravityNormal(); 
	Oyster::Math::Float4 pos = setState.GetCenterPosition();
	Oyster::Math::Float4x4 world = Oyster::Math3D::OrientationMatrix_LookAtDirection(lookDir, up.xyz, pos.xyz);
	// cant set rotation
	//setState.SetOrientation(world);
	//this->lookDir = lookDir - up.xyz;
	//this->lookDir = lookDir;

	//this->setState.AddRotation(Oyster::Math::Float4(x, y));
	//this->setState.SetRotation();
}

void Player::Jump()
{
	
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
	return (Oyster::Math::Float3)getState.GetCenterPosition();
}
Oyster::Math::Float4x4 Player::GetOrientation() const 
{
	return this->getState.GetOrientation();
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
}

