
#include "Player.h"
#include "Weapon.h"

using namespace GameLogic;
using namespace Oyster::Physics;

Player::Player()
	:DynamicObject(CollisionManager::PlayerCollision, OBJECT_TYPE::OBJECT_TYPE_PLAYER)
{
	weapon = new Weapon();
		
	life = 100;
	teamID = -1;
	playerState = PLAYER_STATE::PLAYER_STATE_IDLE;
	lookDir = Oyster::Math::Float4(0,0,-1,0);
	Oyster::Physics::ICustomBody::State state;
	this->rigidBody->GetState( this->setState );
	setState.SetLinearMomentum( Oyster::Math::Float4(20, 0, 0, 0) );
	this->rigidBody->SetState( setState );

}

Player::~Player(void)
{
	delete weapon;

	weapon = NULL;
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
	setState.ApplyLinearImpulse(this->lookDir * 100);
}
void Player::MoveBackwards()
{
	setState.ApplyLinearImpulse(-this->lookDir * 100);
}
void Player::MoveRight()
{
	//Do cross product with forward vector and negative gravity vector
	Oyster::Math::Float4 r = Oyster::Math::Float4(1, 0, 0, 0 );
	//Oyster::Math::Float4 r = (-rigidBody->GetGravityNormal()).Cross((Oyster::Math::Float3)this->lookDir);
	setState.ApplyLinearImpulse(r * 100);
	
}
void Player::MoveLeft()
{
	//Do cross product with forward vector and negative gravity vector
	Oyster::Math::Float4 r = Oyster::Math::Float4(1, 0, 0, 0 );
	//Oyster::Math::Float4 r = -(-rigidBody->GetGravityNormal()).Cross((Oyster::Math::Float3)this->lookDir);
	setState.ApplyLinearImpulse(-r * 100);
}

void Player::UseWeapon(const WEAPON_FIRE &usage)
{
	this->weapon->Use(usage);
}

void Player::Respawn(Oyster::Math::Float3 spawnPoint)
{

	this->life = 100;
	this->playerState = PLAYER_STATE::PLAYER_STATE_IDLE;
	this->lookDir = Oyster::Math::Float4(1,0,0);
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
	return this->lookDir.xyz;
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

void Player::BeginFrame()
{
	this->rigidBody->SetState(this->setState);
}
void Player::EndFrame()
{
	this->rigidBody->GetState(this->getState);
	this->setState = this->getState;
}