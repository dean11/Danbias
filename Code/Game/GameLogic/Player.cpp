
#include "Player.h"
#include "Weapon.h"
#include "CollisionManager.h"
#include "Game.h"

using namespace GameLogic;
using namespace Oyster::Physics;
using namespace Oyster::Math3D;
using namespace Utility::Value;

const float MOVE_FORCE = 30.0f;
const float KEY_TIMER = 0.03f;
const float AFFECTED_TIMER = 1.0f;

// movement properties
const Float dampening_factor = 0.2f,
			forward_velocity = 50.0f,
			backward_velocity = 25.0f,
			strafe_velocity = 40.0f,
			jump_velocity = 25.0f;

Player::Player()
	:DynamicObject()
{
	Player::initPlayerData();
	this->weapon = NULL;
	this->teamID = -1; 
	this->playerScore.killScore = 0;
	this->playerScore.deathScore = 0;
	this->lookDir = Float3( 0.0f, 0.0f, -1.0f );
}

Player::Player(Oyster::Physics::ICustomBody *rigidBody, void (*EventOnCollision)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Float kineticEnergyLoss), ObjectSpecialType type, int objectID, int teamID)
	:DynamicObject(rigidBody, EventOnCollision, type, objectID)
{
	this->weapon = new Weapon(2,this);
	Player::initPlayerData();
	this->teamID = teamID;
	this->playerScore.killScore = 0;
	this->playerScore.deathScore = 0;
	this->lookDir = Float3( 0.0f, 0.0f, -1.0f );
}

Player::Player(Oyster::Physics::ICustomBody *rigidBody, Oyster::Physics::ICustomBody::SubscriptMessage (*EventOnCollision)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Float kineticEnergyLoss), ObjectSpecialType type, int objectID, int teamID)
	:DynamicObject(rigidBody, EventOnCollision, type, objectID)
{
	this->weapon = new Weapon(2,this);
	Player::initPlayerData();
	this->teamID = teamID;
	this->playerScore.killScore = 0;
	this->playerScore.deathScore = 0;
	this->lookDir = Float3( 0.0f, 0.0f, -1.0f );
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
	this->playerState = PLAYER_STATE_IDLE;

	this->key_forward			= 0.0f;
	this->key_backward			= 0.0f;
	this->key_strafeRight		= 0.0f;
	this->key_strafeLeft		= 0.0f;
	this->key_jump				= 0.0f;
	this->RecentlyAffected		= 0.0f;
	this->deathTimer			= 0.0f;

	this->rotationUp = 0.0f;

	ICustomBody::State state;
	this->rigidBody->GetState( state );
	state.staticFrictionCoeff = 0.0f;
	state.dynamicFrictionCoeff = 0.0f;
	this->rigidBody->SetState( state );
}

void Player::BeginFrame()
{
	if( this->playerState != PLAYER_STATE_DEAD && this->playerState != PLAYER_STATE_DIED) 
	{
		this->weapon->Update( 0.002f );

		// Rotate player accordingly
		this->rigidBody->AddRotationAroundY(this->rotationUp);
		this->rigidBody->SetUp(this->rigidBody->GetState().centerPos.GetNormalized());
		this->rotationUp = 0;

		ICustomBody::State state;
		this->rigidBody->GetState( state );

		// Direction data
		Float4x4 rotation = state.GetRotation();
		Float3 &forwardDir = rotation.v[2].xyz;
		Float3 &upDir = rotation.v[1].xyz;
		Float3 &rightDir = rotation.v[0].xyz;

		if( this->IsWalking() )
		{
			this->UpdateMovement( rotation, state );

			if( this->key_jump > 0.001f )
			{ // process jumping
				this->rigidBody->ApplyImpulse( upDir * jump_velocity * state.mass );

				if( this->playerState != PLAYER_STATE::PLAYER_STATE_JUMPING )
				{
					this->gameInstance->onActionEventFnc( this, PlayerAction::PlayerAction_Jump );
					this->playerState = PLAYER_STATE::PLAYER_STATE_JUMPING;
				}
			}
			else if( this->playerState != PLAYER_STATE::PLAYER_STATE_WALKING )
			{ // if is walking and not attempting to jump. Play walking animation
				this->gameInstance->onActionEventFnc( this, PlayerAction::PlayerAction_Walk );
				this->playerState = PLAYER_STATE::PLAYER_STATE_WALKING;
			}
		}
		else if( this->IsJumping() )
		{
			this->UpdateMovement( rotation, state );
		}
		else if( this->IsIdle() && this->playerState != PLAYER_STATE_IDLE )
		{
			this->gameInstance->onActionEventFnc( this, PlayerAction::PlayerAction_Idle );
			this->playerState = PLAYER_STATE::PLAYER_STATE_IDLE;
		}

		Float frameTime = gameInstance->GetFrameTime();
		this->key_forward = Max( this->key_forward - frameTime, 0.0f );
		this->key_backward = Max( this->key_backward - frameTime, 0.0f );
		this->key_strafeRight = Max( this->key_strafeRight - frameTime, 0.0f );
		this->key_strafeLeft = Max( this->key_strafeLeft - frameTime, 0.0f );
		this->key_jump = Max( this->key_jump - frameTime, 0.0f );
	}
}

void Player::EndFrame() { }

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

void Player::Respawn( Float3 spawnPoint )
{ 
	if( this->playerState == PLAYER_STATE_DEAD) 
	{
		Player::initPlayerData();
		this->rigidBody->SetPosition(spawnPoint);
		this->gameInstance->onRespawnFnc( this, spawnPoint);
		this->gameInstance->onDamageTakenFnc( this, this->playerStats.hp);
	}
}

void Player::SetLookDir( const Float3& lookDir )
{
	// this is the camera right vector
	this->lookDir = -lookDir;

}

void Player::TurnLeft( Float deltaRadians )
{
	this->rotationUp += deltaRadians;
}

void Player::Jump()
{
	this->key_jump = KEY_TIMER;
}

bool Player::IsWalking()
{
	return ( this->rigidBody->GetLambdaUp() < 0.6f );
}

bool Player::IsJumping()
{
	return ( this->rigidBody->GetLambdaUp() >= 0.6f );
}

bool Player::IsIdle()
{
	return ( this->rigidBody->GetLambdaUp() < 0.6f && this->rigidBody->GetLinearVelocity().GetMagnitude() < 0.1f );
}

void Player::Inactivate()
{
	//this->
}

void Player::ResetPlayer( Float3 spawnPos )
{
	Player::initPlayerData();
	this->rigidBody->SetPosition( spawnPos );
	this->playerScore.killScore = 0;
	this->playerScore.deathScore = 0;
}

Float3 Player::GetPosition() const
{
	return this->rigidBody->GetState().centerPos;
}

Float4x4 Player::GetOrientation() const 
{
	return this->rigidBody->GetState().GetOrientation();
}

Float3 Player::GetLookDir() const
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

void Player::AddKill()
{
	this->playerScore.killScore++;
}

void Player::AddDeath()
{
	this->playerScore.deathScore++;
}

int Player::GetKills() const
{
	return this->playerScore.killScore;
}

int Player::GetDeath() const
{
	return this->playerScore.deathScore;
}

void Player::DamageLife( int damage )
{
	if( damage != 0 )
	{
		this->playerStats.hp -= damage;

		if( this->playerStats.hp > 100.0f )
			this->playerStats.hp = 100.0f;

		// send hp to client
		this->gameInstance->onDamageTakenFnc( this, this->playerStats.hp);

		if( this->playerStats.hp <= 0.0f )
		{
			this->playerStats.hp = 0.0f;
			this->playerState = PLAYER_STATE_DIED;
			this->rigidBody->SetLinearVelocity( 0.0f );
		}
	}
}

bool Player::deathTimerTick( float dt )
{
	this->deathTimer -= dt;
	if( this->deathTimer <= 0.0f )
	{
		return true;
	}
	return false;
}

void Player::setDeathTimer( float deathTimer )
{
	this->deathTimer = deathTimer;
	this->playerState = PLAYER_STATE_DEAD;
}

void Player::UpdateMovement( const Float4x4 &orientationMatrix, const ICustomBody::State &state )
{
	const Float3 &rightDir = orientationMatrix.v[0].xyz,
				 &upDir = orientationMatrix.v[1].xyz,
				 &forwardDir = orientationMatrix.v[2].xyz;

	// preserve up/down movement
	Float3 movementAccumulator = NormalProjection( this->rigidBody->GetLinearVelocity(), upDir );

	{ // process forward/backward
		Float3 forwardVelocity = 0.0f;
		bool isNotMovingForwardOrBackward = true;
		if( this->key_forward > 0.001f )
		{
			forwardVelocity += forwardDir * forward_velocity;
			isNotMovingForwardOrBackward = false;
		}
		if( this->key_backward > 0.001f )
		{
			forwardVelocity -= forwardDir * backward_velocity;
			isNotMovingForwardOrBackward = false;
		}
		if( isNotMovingForwardOrBackward )
		{ // dampen forward/backward velocity if not running forward/backward
			forwardVelocity = NormalProjection( state.previousVelocity, forwardDir ) * dampening_factor;
		}
		movementAccumulator += forwardVelocity;
	}
	{ // process strafe right/left
		Float3 strafeVelocity = 0.0f;
		bool isNotStrafing = true;
		if( this->key_strafeRight > 0.001f )
		{
			strafeVelocity = rightDir * strafe_velocity;
			isNotStrafing = false;
		}
		if( this->key_strafeLeft > 0.001f )
		{
			strafeVelocity -= rightDir * strafe_velocity;
			isNotStrafing = false;
		}
		if( isNotStrafing )
		{ // dampen right/left strafe velocity if not strafing
			strafeVelocity = NormalProjection( state.previousVelocity, rightDir ) * dampening_factor;
		}
		movementAccumulator += strafeVelocity;
	}

	this->rigidBody->SetLinearVelocity( movementAccumulator );
}