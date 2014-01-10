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

		lookDir = Oyster::Math::Float4(1,0,0,0);
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
	Oyster::Math::Float4 lookDir;
	
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
	state.ApplyLinearImpulse(myData->lookDir * 100);
}
void Player::MoveBackwards()
{
	state.ApplyLinearImpulse(-myData->lookDir * 100);
}
void Player::MoveRight()
{
	//Do cross product with forward vector and negative gravity vector
	Oyster::Math::Float4 r = (-rigidBody->GetGravityNormal()).Cross((Oyster::Math::Float3)myData->lookDir);
	state.ApplyLinearImpulse(r * 100);
}
void Player::MoveLeft()
{
	//Do cross product with forward vector and negative gravity vector
	Oyster::Math::Float4 r = -(-rigidBody->GetGravityNormal()).Cross((Oyster::Math::Float3)myData->lookDir);
	state.ApplyLinearImpulse(-r * 100);
}

void Player::UseWeapon(const WEAPON_FIRE &fireInput)
{
	myData->weapon->Use(fireInput);
}

void Player::Respawn(Oyster::Math::Float3 spawnPoint)
{

	myData->life = 100;
	myData->playerState = PLAYER_STATE::PLAYER_STATE_IDLE;
	myData->lookDir = Oyster::Math::Float4(1,0,0);
}

void Player::Jump()
{
	
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
	return (Oyster::Math::Float3)state.GetCenterPosition();
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