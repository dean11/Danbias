
#include "Player.h"
#include "Weapon.h"
#include "CollisionManager.h"
#include "Game.h"

using namespace GameLogic;
using namespace Oyster::Physics;
using namespace Oyster::Math3D;
using namespace Utility::Value;

const float KEY_TIMER = 0.08f;
const Float3 normalized_weapon_muzzle_offset = Float3( 0.5f, 0.0f, 3.0f ); // on rightside of hip, slightly forward

Player::Player()
	:DynamicObject()
{
	Player::initPlayerData();
	this->weapon = NULL;
	this->teamID = -1; 
	this->playerScore.killScore = 0;
	this->playerScore.deathScore = 0;
	this->lookDir = Float3( 0.0f, 0.0f, -1.0f );
	this->deathTimer = 0.0f;

}

Player::Player(Oyster::Physics::ICustomBody *rigidBody, void (*EventOnCollision)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Float kineticEnergyLoss), ObjectSpecialType type, int objectID, int teamID)
	:DynamicObject(rigidBody, EventOnCollision, type, objectID)
{
	this->weapon = new Weapon(NoEdgeConstants::Values::Weapons::MaxNumberOfSockets , this);
	Player::initPlayerData();
	this->teamID = teamID;
	this->playerScore.killScore = 0;
	this->playerScore.deathScore = 0;
	this->lookDir = Float3( 0.0f, 0.0f, -1.0f );
	this->deathTimer = 0.0f;
}

Player::Player(Oyster::Physics::ICustomBody *rigidBody, Oyster::Physics::ICustomBody::SubscriptMessage (*EventOnCollision)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Float kineticEnergyLoss), ObjectSpecialType type, int objectID, int teamID)
	:DynamicObject(rigidBody, EventOnCollision, type, objectID)
{
	this->weapon = new Weapon(NoEdgeConstants::Values::Weapons::MaxNumberOfSockets , this);
	Player::initPlayerData();
	this->teamID = teamID;
	this->playerScore.killScore = 0;
	this->playerScore.deathScore = 0;
	this->lookDir = Float3( 0.0f, 0.0f, -1.0f );
	this->deathTimer = 0.0f;
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
	this->stunTimer				= 0.0f;
	this->haveRecoveredFromStun = true;

	this->rotationUp = 0.0f;

	ICustomBody::State state;
	this->rigidBody->GetState( state );
	//this->rigidBody->SetRotation(q)
	state.staticFrictionCoeff = 0.0f;
	state.dynamicFrictionCoeff = 0.0f;
	state.restitutionCoeff = 0.0f;
	this->rigidBody->SetState( state );
}

Float3 & Player::GetWeaponMuzzlePosition( Float3 &targetMem )
{
	return this->GetWeaponMuzzlePosition( targetMem, this->GetRigidBody()->GetState() );
}

Float3 & Player::GetWeaponMuzzlePosition( Float3 &targetMem, const ICustomBody::State &state )
{
	targetMem = normalized_weapon_muzzle_offset * this->GetScale(); // TODO: would prefer if state had the scale data

	Float4x4 rotM = OrientationMatrix_LookAtDirection(-this->lookDir * 0.05f, WorldAxisOf(state.quaternion, Float3::standard_unit_y), state.centerPos);

	targetMem = rotM * Float4(targetMem, 1.0f);
	return targetMem;
}

void Player::BeginFrame(float dt)
{
	if( this->playerState != PLAYER_STATE_DEAD && this->playerState != PLAYER_STATE_DIED) 
	{
		this->weapon->Update( dt );

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

		bool haveMoved = false,
			 isGrounded = this->IsWalking();

		if( isGrounded )
		{
			haveMoved = this->UpdateMovement( rotation, state, true );

			if( this->key_jump > 0.001f )
			{ // process jumping
				this->rigidBody->ApplyImpulse( upDir * NoEdgeConstants::Values::Player::JumpVelocity * state.mass );
				haveMoved = true;
				isGrounded = false;

				if( this->playerState != PLAYER_STATE::PLAYER_STATE_JUMPING )
				{
					this->gameInstance->onActionEventFnc( this, PlayerAction::PlayerAction_Jump );
					this->playerState = PLAYER_STATE::PLAYER_STATE_JUMPING;
				}
			}
			else if( haveMoved && this->playerState != PLAYER_STATE::PLAYER_STATE_WALKING )
			{ // if is walking and not attempting to jump. Play walking animation
				this->gameInstance->onActionEventFnc( this, PlayerAction::PlayerAction_Walk );
				this->playerState = PLAYER_STATE::PLAYER_STATE_WALKING;
			}
		}
		else
		{
			haveMoved = this->UpdateMovement( rotation, state, false );
		}
		
		if( !haveMoved && isGrounded && this->playerState != PLAYER_STATE_IDLE )
		{
			this->gameInstance->onActionEventFnc( this, PlayerAction::PlayerAction_Idle );
			this->playerState = PLAYER_STATE::PLAYER_STATE_IDLE;
		}

		Float frameTime = dt;
		this->key_forward = Max( this->key_forward - frameTime, 0.0f );
		this->key_backward = Max( this->key_backward - frameTime, 0.0f );
		this->key_strafeRight = Max( this->key_strafeRight - frameTime, 0.0f );
		this->key_strafeLeft = Max( this->key_strafeLeft - frameTime, 0.0f );
		this->key_jump = Max( this->key_jump - frameTime, 0.0f );
		this->stunTimer = Max( this->stunTimer - frameTime, 0.0f );
	}
	else
	{
		// can't be moved while dead
		this->rigidBody->SetLinearVelocity( 0.0f );
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

void Player::SelectWeaponAttatchment( int socketID )
{
	this->weapon->SelectAttatchment( socketID );
}

void Player::Respawn( Float3 spawnPoint )
{ 
	if( this->playerState == PLAYER_STATE_DEAD) 
	{
		Player::initPlayerData();
		this->rigidBody->SetPosition(spawnPoint);
		this->gameInstance->onRespawnFnc( this, spawnPoint);
		this->gameInstance->onDamageTakenFnc( this, this->playerStats.hp, PlayerHealthEvent::PlayerHealthEvent_Respawn);
		Oyster::Physics::API::Instance().ReleaseFromLimbo(this->rigidBody);
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
	return ( this->rigidBody->GetLambdaUp() < 0.8f );
}

bool Player::IsJumping()
{
	return ( this->rigidBody->GetLambdaUp() >= 0.8f );
}

bool Player::IsIdle()
{
	Float3 v = this->rigidBody->GetLinearVelocity();
	return ( this->rigidBody->GetLambdaUp() < 0.8f && v.Dot( v ) < 0.0001f );
}

bool Player::IsStunned( bool struggled )
{
	if( struggled && this->stunTimer == 0.0f )
	{
		this->haveRecoveredFromStun = true;
	}
	return !this->haveRecoveredFromStun;
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

void Player::Stun( float duration )
{
	this->stunTimer = duration;
	this->haveRecoveredFromStun = false;
}

void Player::DamageLife( float damage )
{
	// don't take dmg while dead
	if( this->playerState != PLAYER_STATE_DEAD && this->playerState != PLAYER_STATE_DIED) 
	{
		if( damage != 0 )
		{
			this->playerStats.hp -= damage;

			if( this->playerStats.hp > 100.0f )
				this->playerStats.hp = 100.0f;

			
			else if( this->playerStats.hp <= 0.0f )
			{
				this->playerStats.hp = 0.0f;
				this->playerState = PLAYER_STATE_DIED;
				this->rigidBody->SetLinearVelocity( 0.0f );
				Oyster::Physics::API::Instance().MoveToLimbo(this->rigidBody);
			}


			// heal 
			if(damage < 0 )
				this->gameInstance->onDamageTakenFnc( this, this->playerStats.hp, PlayerHealthEvent::PlayerHealthEvent_Heal);

			// dmg taken
			else
				this->gameInstance->onDamageTakenFnc( this, this->playerStats.hp, PlayerHealthEvent::PlayerHealthEvent_Damage);
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

bool Player::UpdateMovement( const Float4x4 &orientationMatrix, const ICustomBody::State &state, bool isGrounded )
{
	const Float3 &rightDir = orientationMatrix.v[0].xyz,
				 &upDir = orientationMatrix.v[1].xyz,
				 &forwardDir = orientationMatrix.v[2].xyz;

	bool haveMoved = false;

	// preserve up/down movement
	Float3 movementAccumulator = NormalProjection( this->rigidBody->GetLinearVelocity(), upDir ),
		   forwardVelocity = 0.0f,
		   strafeVelocity = 0.0f;
	bool isNotMovingForwardOrBackward = true,
		 isNotStrafing = true;

	{ // process forward/backward
		if( this->key_forward > 0.001f )
		{
			forwardVelocity = forwardDir * NoEdgeConstants::Values::Player::ForwardVelocity;
			isNotMovingForwardOrBackward = false;
		}
		if( this->key_backward > 0.001f )
		{
			forwardVelocity -= forwardDir * NoEdgeConstants::Values::Player::ForwardVelocity;
			isNotMovingForwardOrBackward = false;
		}

		if( isNotMovingForwardOrBackward )
		{ // dampen forward/backward velocity if not running forward/backward
			forwardVelocity = NormalProjection( state.previousVelocity, forwardDir );
			if( isGrounded )
				forwardVelocity *= NoEdgeConstants::Values::Player::DampeningFactor;
		}
		else
		{
			haveMoved = true;
		}
	}
	{ // process strafe right/left
		if( this->key_strafeRight > 0.001f )
		{
			strafeVelocity = rightDir * NoEdgeConstants::Values::Player::StrafeVelocity;
			isNotStrafing = false;
		}
		if( this->key_strafeLeft > 0.001f )
		{
			strafeVelocity -= rightDir * NoEdgeConstants::Values::Player::StrafeVelocity;
			isNotStrafing = false;
		}

		if( isNotStrafing )
		{ // dampen right/left strafe velocity if not strafing
			strafeVelocity = NormalProjection( state.previousVelocity, rightDir );
			if( isGrounded )
				strafeVelocity *= NoEdgeConstants::Values::Player::DampeningFactor;
		}
		else
		{
			haveMoved = true;
		}
	}

	if( this->IsStunned(haveMoved) )
	{
		if( isGrounded )
		{
			forwardVelocity = NormalProjection( state.previousVelocity, forwardDir ) * 2.0f * NoEdgeConstants::Values::Player::DampeningFactor;
			strafeVelocity = NormalProjection( state.previousVelocity, rightDir ) * 2.0f * NoEdgeConstants::Values::Player::DampeningFactor;
		}
		haveMoved = false;
	}

	movementAccumulator += forwardVelocity;
	movementAccumulator += strafeVelocity;

	if( isGrounded || haveMoved )
		this->rigidBody->SetLinearVelocity( movementAccumulator );
	return haveMoved;
}