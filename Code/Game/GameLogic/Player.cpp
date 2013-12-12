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
		playerState = PLAYER_STATE_IDLE;

	}

	~PrivateData()
	{
		if (weapon)
		{
			delete weapon;
		}
	}
	
	int life;
	Weapon *weapon;
	PLAYER_STATE playerState;
	Oyster::Math::Float3 lookDir;
	
}myData;

Player::Player()
	:Object(CollisionManager::PlayerCollision, OBJECT_TYPE_PLAYER)
{
	myData = new PrivateData();
}

Player::~Player(void)
{
	delete myData;
}

/********************************************************
* Updates the player(is this function needed?)
********************************************************/

void Player::Update()
{
	
}

/********************************************************
* Moves the player based on client input
* Uses the physics to move the player by adding a force in the chosen direction
* Uses the Jump() function if the player is to jump, this is becuase jumping requires additional logic compared to normal movement
********************************************************/
void Player::Move(const PLAYER_MOVEMENT &movement)
{
	switch(movement)
	{
		case PLAYER_MOVEMENT_FORWARD:
			break;

		case PLAYER_MOVEMENT_BACKWARD:
			break;

		case PLAYER_MOVEMENT_LEFT:
			break;

		case PLAYER_MOVEMENT_RIGHT:
			break;

		case PLAYER_MOVEMENT_JUMP:
			Jump();
			break;
	}
}
/********************************************************
* Uses the players weapon based on user input
********************************************************/
void Player::UseWeapon(const WEAPON_FIRE &fireInput)
{
	myData->weapon->Use(fireInput);
}

/********************************************************
* Jumps if the player is currently not in a state of jumping
* Applies a force upwards(current upwards)
********************************************************/
void Player::Jump()
{

}

bool Player::IsWalking()
{
	return (myData->playerState == PLAYER_STATE_WALKING);
}
bool Player::IsJumping()
{
	return (myData->playerState == PLAYER_STATE_JUMPING);
}
bool Player::IsIdle()
{
	return (myData->playerState == PLAYER_STATE_IDLE);
}

Oyster::Math::Float3 Player::GetPos()
{
	return rigidBody->GetCenter();
}

/********************************************************
* Respawns the player on a new chosen position
* This resets a set of variables such as life, ammo etcetc
********************************************************/
void Player::Respawn()
{

}
