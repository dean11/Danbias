
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
	this->rigidBody = rigidBody;
	
	Oyster::Math::Float3 centerPosition = Oyster::Math::Float3(0,400,0);

	Oyster::Math::Float3 size = Oyster::Math::Float3(0.25f,1.0f,0.5f);
	Oyster::Math::Float mass = 60;
	Oyster::Math::Float restitutionCoeff = 0.5;
	Oyster::Math::Float frictionCoeff_Static = 0.4;
	Oyster::Math::Float frictionCoeff_Dynamic = 0.3;


	this->rigidBody = Oyster::Physics::API::Instance().AddCollisionBox(size, Oyster::Math::Float4(0, 0, 0, 1), centerPosition, mass, 0.5f, 0.8f, 0.6f );
	this->rigidBody->SetAngularFactor(0.0f);

	weapon = new Weapon(2,this);

	this->life = 100;
	this->teamID = teamID;
	this->playerState = PLAYER_STATE_IDLE;
	this->lookDir = Oyster::Math::Float3(0,0,-1);
	key_forward = 0;
	key_backward = 0;
	key_strafeRight = 0;
	key_strafeLeft = 0;

	this->moveDir = Oyster::Math::Float3(0,0,0);
	this->moveSpeed = 100;
	this->previousMoveSpeed = Oyster::Math::Float3(0,0,0);
}

Player::Player(Oyster::Physics::ICustomBody *rigidBody, Oyster::Physics::ICustomBody::SubscriptMessage (*EventOnCollision)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, int objectID, int teamID)
	:DynamicObject(rigidBody, EventOnCollision, type, objectID)
{
	this->rigidBody = rigidBody;
	
	Oyster::Math::Float3 centerPosition = Oyster::Math::Float3(0,400,0);

	Oyster::Math::Float3 size = Oyster::Math::Float3(0.25f,1.0f,0.5f);
	Oyster::Math::Float mass = 60;
	Oyster::Math::Float restitutionCoeff = 0.5;
	Oyster::Math::Float frictionCoeff_Static = 0.4;
	Oyster::Math::Float frictionCoeff_Dynamic = 0.3;


	this->rigidBody = Oyster::Physics::API::Instance().AddCollisionBox(size, Oyster::Math::Float4(0, 0, 0, 1), centerPosition, mass, 0.5f, 0.8f, 0.6f );
	this->rigidBody->SetAngularFactor(0.0f);

	weapon = new Weapon(2,this);

	this->life = 100;
	this->teamID = teamID;
	this->playerState = PLAYER_STATE_IDLE;
	this->lookDir = Oyster::Math::Float3(0,0,-1);
	key_forward = 0;
	key_backward = 0;
	key_strafeRight = 0;
	key_strafeLeft = 0;

	this->moveDir = Oyster::Math::Float3(0,0,0);
	this->moveSpeed = 100;
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
	
	if(this->moveDir != Oyster::Math::Float3::null)
	{
		Oyster::Math::Float3 velocity = this->rigidBody->GetLinearVelocity();
		Oyster::Math::Float3 lostVelocity = (this->previousMoveSpeed - velocity).GetMagnitude()*this->moveDir;
     	this->rigidBody->SetLinearVelocity(velocity + lostVelocity - this->moveDir*this->moveSpeed );
	}

	this->moveDir = Oyster::Math::Float3::null;

	if (key_forward > 0.001)
	{
		key_forward -= gameInstance->GetFrameTime(); // fixed timer 
		this->moveDir += this->rigidBody->GetState().GetOrientation().v[2].GetNormalized().xyz;
	}
	if (key_backward > 0.001)
	{
		key_backward -= gameInstance->GetFrameTime();
		this->moveDir -= this->rigidBody->GetState().GetOrientation().v[2].GetNormalized().xyz;
	}
	if (key_strafeRight > 0.001)
	{
		key_strafeRight -= gameInstance->GetFrameTime();
		Oyster::Math::Float3 forward = this->rigidBody->GetState().GetOrientation().v[2];
		Oyster::Math::Float3 up = this->rigidBody->GetState().centerPos;
		this->moveDir -= (up).Cross(forward).GetNormalized();
	}
	if (key_strafeLeft > 0.001)
	{
		key_strafeLeft -= gameInstance->GetFrameTime();
		Oyster::Math::Float3 forward = this->rigidBody->GetState().GetOrientation().v[2];
		Oyster::Math::Float3 up = this->rigidBody->GetState().centerPos;
		this->moveDir += (up).Cross(forward).GetNormalized();
	}

	if(this->moveDir != Oyster::Math::Float3::null)
	{
     	this->moveDir.Normalize();
		this->rigidBody->SetLinearVelocity(this->moveDir*this->moveSpeed + this->rigidBody->GetLinearVelocity());
		this->previousMoveSpeed = this->rigidBody->GetLinearVelocity();
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

	Oyster::Math::Float3 up = this->rigidBody->GetState().GetOrientation().v[1];
	this->rigidBody->SetUpAndRight(up, right);
}

void Player::Jump()
{
	Oyster::Math::Float3 up = this->rigidBody->GetState().GetOrientation().v[1].GetNormalized();
	this->rigidBody->ApplyImpulse(up *1500);
	this->playerState = PLAYER_STATE::PLAYER_STATE_JUMPING;
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
