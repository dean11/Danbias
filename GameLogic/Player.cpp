/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "Player.h"
#include "Session.h"
#include "GameLUA.h"

#include <iostream> // TODO: to be removed

using namespace ::GameLogic;
using namespace ::Oyster::Math;
using namespace ::Oyster::Collision;
using ::std::string;

const unsigned char Player::noTeam = 255;

Player::Player( ) : Object(), session(NULL), name(), playerID(-1), teamID(Player::noTeam), killCount(0), teamKillCount(0), deathCount(0), lockedTarget(NULL), hullPoints(255), shieldPoints(0), specialID(255), primaryWeapon(Weapon::Test), primaryWeaponCooldown(0.0f) {}
Player::Player( const Player &player ) : Object(player), session(player.session), name(player.name), playerID(player.playerID), teamID(player.teamID), killCount(player.killCount), teamKillCount(player.teamKillCount), deathCount(player.deathCount), lockedTarget(player.lockedTarget), hullPoints(player.hullPoints), shieldPoints(player.shieldPoints), specialID(player.specialID), primaryWeapon(player.primaryWeapon), primaryWeaponCooldown(player.primaryWeaponCooldown) {}
Player::Player( const Float4x4 &orientation, const Float3 &centerOfMass ) : Object(orientation, centerOfMass), session(NULL), name(), playerID(-1), teamID(Player::noTeam), killCount(0), teamKillCount(0), deathCount(0), lockedTarget(NULL), hullPoints(255), shieldPoints(0), specialID(255), primaryWeapon(Weapon::Test), primaryWeaponCooldown(0.0f) {}
Player::Player( const Box &body, const Float3 &centerOfMass ) : Object(body, centerOfMass), session(NULL), name(), playerID(-1), teamID(Player::noTeam), killCount(0), teamKillCount(0), deathCount(0), lockedTarget(NULL), hullPoints(255), shieldPoints(0), specialID(255), primaryWeapon(Weapon::Test), primaryWeaponCooldown(0.0f) {}
Player::~Player( ) {}

Player & Player::operator = ( const Player &player )
{
	Object::operator=( player );
	this->session = player.session;
	this->name = player.name;
	this->playerID = player.playerID;
	this->teamID = player.teamID;
	this->killCount = player.killCount;
	this->teamKillCount = player.teamKillCount;
	this->deathCount = player.deathCount;
	this->hullPoints = player.hullPoints;
	this->shieldPoints = player.shieldPoints;
	this->specialID = player.specialID;
	this->primaryWeapon = player.primaryWeapon;
	this->primaryWeaponCooldown = player.primaryWeaponCooldown;

	return *this;
}

Object * Player::clone( ) const
{ return new Player(*this); }

void Player::update( float deltaTime )
{
	Object::update( );

	if (primaryWeaponCooldown > 0.0f)
		primaryWeaponCooldown -= deltaTime;
}

void Player::firePrimaryWeapon( )
{
	if (primaryWeaponCooldown <= 0.0f)
		GameLUA::executeOnFireScript( primaryWeapon, *this );
}

void Player::fireSecondaryWeapon( )
{
	/* TODO */
}

void Player::fireSpecial( )
{
	/* TODO */
}

void Player::lockTarget( unsigned int objectID )
{ this->lockedTarget = this->session->objectIDLib[objectID]; }

void Player::setPrimaryWeapon(Weapon::Type weapon)
{
	primaryWeapon = weapon;
}

void Player::setPrimaryWeaponCooldown(float cooldown)
{
	primaryWeaponCooldown = cooldown;
}

Object::Type Player::getType( ) const
{ return Object::Player; }

const string & Player::getName( ) const
{ return this->name; }

int Player::getPlayerID( ) const
{
	return this->playerID;
}

unsigned char Player::getTeamID( ) const
{ return this->teamID; }

unsigned int Player::getNumKills( ) const
{ return this->killCount; }

unsigned int Player::getNumTeamKills( ) const
{ return this->teamKillCount; }

unsigned int Player::getNumDeaths( ) const
{ return this->deathCount; }

Session *Player::getSession( ) const
{ return this->session; }

unsigned char Player::getHullPoints( ) const
{ return this->hullPoints; }

unsigned char Player::getShieldPoints( ) const
{ return this->shieldPoints; }

void Player::setName( const string &name )
{ this->name = name; }

void Player::setPlayerID( int id )
{
	playerID = id;
}

void Player::setTeam( unsigned char teamID )
{ this->teamID = teamID; }

void Player::setHullPoints( unsigned char value )
{
	this->hullPoints = value;

	if( value == 0 )
		this->onDeath( *this );
}

void Player::setShieldPoints( unsigned char value )
{ this->shieldPoints = value; }

void Player::resetScores( )
{ this->killCount = this->teamKillCount = this->deathCount = 0; }

void Player::applyDamage( unsigned char magnitude, Object &attacker )
{
	if( this->shieldPoints > 0 )
	{
		magnitude = (unsigned char)(unsigned int)((float)magnitude * ( (float)this->shieldPoints / (float)this->getConfigData( this->getConfigID() ).shieldPoints ));

		if( magnitude == 0 )
			magnitude = 1;

		if( this->shieldPoints >= magnitude )
		{
			this->shieldPoints -= magnitude;
			return;
		}
		else
		{
			magnitude -= this->shieldPoints;
			this->shieldPoints = 0;
		}
	}

	if( this->hullPoints <= magnitude )
	{
		this->hullPoints = 0;
		this->onDeath( attacker );
	}
	else
		this->hullPoints -= magnitude;
}

void Player::applyHeal( unsigned char magnitude )
{
	this->hullPoints += magnitude;

	unsigned int id = this->getConfigID();
	if( id != Object::invalidID )
		this->hullPoints = ::Utility::Value::min( this->hullPoints, (unsigned char)this->getConfigData(id).hullPoints );
}

void Player::onDeath( Object &killer )
{
	killer.onKill( *this );
	++this->deathCount;
	Player* p = dynamic_cast<Player*>(&killer);
	Event::ShipDestroyed* e = new Event::ShipDestroyed(this->playerID,p->playerID);
	for(int i=0;i < PLAYER_MAX_COUNT; ++i)
	{
		e->setScore(i,this->session->player[i].killCount, this->session->player[i].deathCount, this->session->player[i].teamKillCount);
	}
	this->session->eventList.push_back(e);

	this->moveToLimbo();
	
	this->spawn();
}

void Player::onKill( Object &victim )
{
	if( victim.getType() == Object::Player )
	{
		Player *v = (Player*)&victim;
		if( this->teamID == Player::noTeam || this->teamID != v->teamID )
			++this->killCount;
		else
			++this->teamKillCount;
	}
}

void Player::spawn( )
{
	int num = (int)this->session->spawnPoint.size();
	this->stop();

	if( num > 0 )
	{
		int	spawnRef = rand() % (int)num;
		Box spawnZone = this->body;
		unsigned int channel = (unsigned int)Object::Player;
	
		for( int i = 0; i < num; ++i )
		{
			spawnZone.orientation = this->session->spawnPoint[spawnRef];
			if( this->session->world.collidesWithSomething(&channel, 1, &spawnZone) )
				spawnRef = (spawnRef + 1) % (int)num;
			else
			{
				this->setOrientation( spawnZone.orientation );
				this->setConfig(this->getConfigID());
				this->releaseFromLimbo();
				return;
			}
		}
	}
	this->moveToLimbo();
}

void Player::loadConfig( const InstanceBlueprint &config )
{
	Object::loadConfig( config );
	this->hullPoints = config.hullPoints;
	this->shieldPoints = config.shieldPoints;
}