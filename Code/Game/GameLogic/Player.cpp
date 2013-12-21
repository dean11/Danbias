#include "Player.h"
#include "OysterMath.h"
#include "CollisionManager.h"
#include "Weapon.h"

using namespace GameLogic;
using namespace Oyster::Physics;

struct Player::PrivateData
{
	PrivateData()
	{
		weapon = new Weapon();
		
		life = 100;
		teamID = -1;
		playerState = PLAYER_STATE::PLAYER_STATE_IDLE;

		lookDir = Oyster::Math::Float3(1,0,0);
	}

	~PrivateData()
	{
		if (weapon)
		{
			delete weapon;
		}
	}
	
	int life;
	int teamID;
	Weapon *weapon;
	PLAYER_STATE playerState;
	Oyster::Math::Float3 lookDir;
	
}myData;

Player::Player()
	:Object(CollisionManager::PlayerCollision, OBJECT_TYPE::OBJECT_TYPE_PLAYER)
{
	myData = new PrivateData();
}

Player::~Player(void)
{
	delete myData;
}


void Player::Move(const PLAYER_MOVEMENT &movement)
{
	//Oyster::Math::Float3 currentVelocity = rigidBody->GetRigidLinearVelocity();

	switch(movement)
	{
	case PLAYER_MOVEMENT::PLAYER_MOVEMENT_FORWARD:
			//API::Instance().ApplyForceAt(rigidBody,rigidBody->GetCenter(),myData->lookDir * 100);
			break;

	case PLAYER_MOVEMENT::PLAYER_MOVEMENT_BACKWARD:
			//API::Instance().ApplyForceAt(rigidBody,rigidBody->GetCenter(),-myData->lookDir * 100);
			break;

	case PLAYER_MOVEMENT::PLAYER_MOVEMENT_LEFT:
			break;

	case PLAYER_MOVEMENT::PLAYER_MOVEMENT_RIGHT:
			break;

	case PLAYER_MOVEMENT::PLAYER_MOVEMENT_JUMP:
			Jump();
			break;
	}
}

void Player::MoveForward()
{
	//API::Instance().ApplyForceAt(rigidBody,rigidBody->GetCenter(),myData->lookDir * 100);
}
void Player::MoveBackwards()
{
	//API::Instance().ApplyForceAt(rigidBody,rigidBody->GetCenter(),-myData->lookDir * 100);
}
void Player::MoveRight()
{
	//Do cross product with forward vector and negative gravity vector
	Oyster::Math::Float3 r = (-rigidBody->GetGravityNormal()).Cross(myData->lookDir);
	//API::Instance().ApplyForceAt(rigidBody, rigidBody->GetCenter(), r * 100);
}
void Player::MoveLeft()
{
	//Do cross product with forward vector and negative gravity vector
	Oyster::Math::Float3 r = -(-rigidBody->GetGravityNormal()).Cross(myData->lookDir);
	//API::Instance().ApplyForceAt(rigidBody, rigidBody->GetCenter(), r * 100);
}

void Player::UseWeapon(const WEAPON_FIRE &fireInput)
{
	myData->weapon->Use(fireInput);
}

void Player::Respawn(Oyster::Math::Float3 spawnPoint)
{
	//API::Instance().SetCenter(rigidBody,spawnPoint);
	myData->life = 100;
	myData->playerState = PLAYER_STATE::PLAYER_STATE_IDLE;
	myData->lookDir = Oyster::Math::Float3(1,0,0);
}

void Player::Jump()
{
	//API::Instance().ApplyForceAt(rigidBody,rigidBody->GetCenter(),-Oyster::Math::Float3(0,1,0) * 100);
}

bool Player::IsWalking()
{
	return (myData->playerState == PLAYER_STATE::PLAYER_STATE_WALKING);
}
bool Player::IsJumping()
{
	return (myData->playerState == PLAYER_STATE::PLAYER_STATE_JUMPING);
}
bool Player::IsIdle()
{
	return (myData->playerState == PLAYER_STATE::PLAYER_STATE_IDLE);
}

Oyster::Math::Float3 Player::GetPos()
{
	//return rigidBody->GetCenter();
	return Oyster::Math::Float3(0,0,0);
}

Oyster::Math::Float3 Player::GetLookDir()
{
	return myData->lookDir;
}

int Player::GetTeamID()
{
	return myData->teamID;
}

void Player::DamageLife(int damage)
{
	myData->life -= damage;
}