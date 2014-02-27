
#include "Player.h"
#include "Weapon.h"
#include "CollisionManager.h"
#include "Game.h"

using namespace GameLogic;
using namespace Oyster::Physics;
using namespace Oyster::Math;

const float MOVE_FORCE = 30;
const float KEY_TIMER = 0.03f;
const float AFFECTED_TIMER = 1.0f;
Player::Player()
	:DynamicObject()
{
	Player::initPlayerData();
	this->weapon = NULL;
	this->teamID = -1; 
	this->playerScore.killScore = 0;
	this->playerScore.deathScore = 0;
}

Player::Player(Oyster::Physics::ICustomBody *rigidBody, void (*EventOnCollision)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, int objectID, int teamID)
	:DynamicObject(rigidBody, EventOnCollision, type, objectID)
{
	this->weapon = new Weapon(2,this);
	Player::initPlayerData();
	this->teamID = teamID;
	this->playerScore.killScore = 0;
	this->playerScore.deathScore = 0;
}

Player::Player(Oyster::Physics::ICustomBody *rigidBody, Oyster::Physics::ICustomBody::SubscriptMessage (*EventOnCollision)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, int objectID, int teamID)
	:DynamicObject(rigidBody, EventOnCollision, type, objectID)
{
	this->weapon = new Weapon(2,this);
	Player::initPlayerData();
	this->teamID = teamID;
	this->playerScore.killScore = 0;
	this->playerScore.deathScore = 0;
}

Player::~Player(void)
{
	if(this->weapon)
	{
		delete this->weapon;
		this->weapon = NULL;
	}	
}
void Player::initPlayerData()
{
	this->playerStats.hp = MAX_HP;
	this->playerStats.movementSpeed = BASIC_SPEED;
	this->playerState			= PLAYER_STATE_IDLE;
	this->lookDir				= Float3( 0.0f, 0.0f, -1.0f );

	this->key_forward			= 0;
	this->key_backward			= 0;
	this->key_strafeRight		= 0;
	this->key_strafeLeft		= 0;
	this->key_jump				= 0;
	this->RecentlyAffected		= 0;
	this->deathTimer			= 0;

	this->rotationUp = 0;

	ICustomBody::State state = this->rigidBody->GetState();
	state.staticFrictionCoeff = 0.0f;
	state.dynamicFrictionCoeff = 0.0f;
	this->rigidBody->SetState( state );
}

void Player::BeginFrame()
{
	if( this->playerState != PLAYER_STATE_DEAD && this->playerState != PLAYER_STATE_DIED ) 
	{
		static const Float maxSpeed = 30.0f;

		weapon->Update( 0.002f );

		// Rotate player accordingly
		this->rigidBody->AddRotationAroundY(this->rotationUp);
		this->rigidBody->SetUp(this->rigidBody->GetState().centerPos.GetNormalized());
		this->rotationUp = 0;

		// Direction data
		Oyster::Math::Float4x4 orientation;
		orientation = this->rigidBody->GetState().GetOrientation();

		Float3 &forwardDir = orientation.v[2].xyz;
		Float3 &upDir = orientation.v[1].xyz;
		Float3 &rightDir = orientation.v[0].xyz;

		// Pre-update velocities data
		Float3 linearVelocity = this->rigidBody->GetLinearVelocity();
		Float3 forwardVelocity = linearVelocity * Float3(fabs(forwardDir.x), fabs(forwardDir.y), fabs(forwardDir.z) );
		Float forwardSpeed = (linearVelocity * forwardDir).GetLength();
		Float3 rightVelocity = linearVelocity * Float3(fabs(rightDir.x), fabs(rightDir.y), fabs(rightDir.z) );
		Float rightSpeed = (linearVelocity * rightDir).GetLength();
		Float3 upVelocity = linearVelocity * Float3(fabs(upDir.x), fabs(upDir.y), fabs(upDir.z) );

		// Walking data
		Float3 walkDirection = Float3( 0.0f );
		Float &walkSpeed = this->playerStats.movementSpeed;

		Float frameTime = gameInstance->GetFrameTime();

		// Check for input
		if(key_forward > 0.001)
		{
			key_forward -= frameTime;
			walkDirection += forwardDir;
		}
		if(key_backward > 0.001)
		{
			key_backward -= frameTime;
			walkDirection -= forwardDir;
		}
		if(key_strafeRight > 0.001)
		{
			key_strafeRight -= frameTime;
			walkDirection += rightDir;
		}
		if(key_strafeLeft > 0.001)
		{
			key_strafeLeft -= frameTime;
			walkDirection -= rightDir;
		}
	
		// Dampen velocity if certain keys are not pressed
		if( key_jump <= 0.001 && IsWalking() )
		{
			if(key_forward <= 0.001 && key_backward <= 0.001)
			{
				forwardVelocity *= Float3(0.2f*fabs(forwardDir.x), 0.2f*fabs(forwardDir.y), 0.2f*fabs(forwardDir.z));
			}
			if(key_strafeRight <= 0.001 && key_strafeLeft <= 0.001)
			{
				rightVelocity *= Float3(0.2f*fabs(rightDir.x), 0.2f*fabs(rightDir.y), 0.2f*fabs(rightDir.z));
			}
		}

		if( walkDirection == Float3::null )
		{
			if(this->playerState != PLAYER_STATE::PLAYER_STATE_JUMPING)
			{
				if(this->playerState != PLAYER_STATE::PLAYER_STATE_IDLE)
					this->gameInstance->onActionEventFnc( this, PlayerAction::PlayerAction_Idle);
				this->playerState = PLAYER_STATE::PLAYER_STATE_IDLE;
			}
		}
		else
		{ // Walk if walkdirection is something
			walkDirection.Normalize();

			// If on the ground, accelerate normally
			if(IsWalking())
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
			if(IsJumping())
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

			if(this->playerState != PLAYER_STATE::PLAYER_STATE_JUMPING)
			{
				if(this->playerState != PLAYER_STATE::PLAYER_STATE_WALKING)
					this->gameInstance->onActionEventFnc( this, PlayerAction::PlayerAction_Walk);
				this->playerState = PLAYER_STATE::PLAYER_STATE_WALKING;
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
			if(IsWalking())
			{
				Oyster::Math::Float3 up = this->rigidBody->GetState().centerPos.GetNormalized();
				this->rigidBody->ApplyImpulse(up*this->rigidBody->GetState().mass * 20);
				
				if(this->playerState != PLAYER_STATE::PLAYER_STATE_JUMPING)
					this->gameInstance->onActionEventFnc( this, PlayerAction::PlayerAction_Jump);
				this->playerState = PLAYER_STATE::PLAYER_STATE_JUMPING;
			}
		}
		else
		{
			if(this->playerState == PLAYER_STATE::PLAYER_STATE_JUMPING)
			{
				this->gameInstance->onActionEventFnc( this, PlayerAction::PlayerAction_Idle);
				this->playerState = PLAYER_STATE::PLAYER_STATE_IDLE;
			}
		}
	}
}

void Player::EndFrame()
{
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
		Player::initPlayerData();
		this->rigidBody->SetPosition(spawnPoint);
		this->gameInstance->onRespawnFnc( this, spawnPoint);
		this->gameInstance->onDamageTakenFnc( this, this->playerStats.hp);
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
	return (this->rigidBody->GetLambdaUp() < 0.99f);
}
bool Player::IsJumping()
{
	return (this->rigidBody->GetLambdaUp() == 1.0f);
}
bool Player::IsIdle()
{
	return (this->rigidBody->GetLambdaUp() == 1.0f && this->rigidBody->GetLinearVelocity().GetMagnitude() < 0.0001f);
}

void Player::Inactivate()
{
	//this->
}
void Player::ResetPlayer( Oyster::Math::Float3 spawnPos)
{
	Player::initPlayerData();
	this->rigidBody->SetPosition(spawnPos);
	this->playerScore.killScore = 0;
	this->playerScore.deathScore = 0;
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
	if(damage != 0)
	{
		this->playerStats.hp -= damage;

		if(this->playerStats.hp > 100)
			this->playerStats.hp = 100;

		// send hp to client
		this->gameInstance->onDamageTakenFnc( this, this->playerStats.hp);

		if(this->playerStats.hp <= 0)
		{
			this->playerStats.hp = 0;
			this->playerState = PLAYER_STATE_DIED;
		}
	}
}

bool Player::deathTimerTick(float dt)
{
	this->deathTimer -= dt;
	if( this->deathTimer <= 0)
	{
		return true;
	}
	return false;
}
void Player::setDeathTimer(float deathTimer)
{
	this->deathTimer = deathTimer;
	this->playerState = PLAYER_STATE_DEAD;
}
