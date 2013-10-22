#include "EffectCarrier.h"
#include "GameLUA.h"
#include "Bullet.h"
#include "Event.h"
#include "Session.h"

using namespace ::GameLogic;
using ::std::vector;

// STATIC FACTORIES /////////////////////////////////////////////////
EffectCarrier * EffectCarrier::createBullet( Player *owner, Weapon::Type weapon, const Float3 &origin, const Float3 &normalizedDirection, const Float &speed, const Float &lifeTime, const ::std::string &modelRef )
{
	class Bullet *carrier = new class Bullet( owner, origin, normalizedDirection, speed, weapon, lifeTime );

	// TODO have a modelID lib to ask
	carrier->setModelID( 10240 );

	Event::BulletCreated *bulletEvent = new Event::BulletCreated(owner->getPlayerID(), origin, normalizedDirection);
	owner->getSession()->addEvent(bulletEvent);

	return carrier;
}

// INSTANCE /////////////////////////////////////////////////////////
EffectCarrier::Collector::Collector( int capacity ) : hitObjects(), hitPosW()
{
	if( capacity > 0 )
	{
		this->hitObjects.reserve( capacity );
		this->hitPosW.reserve( capacity );
	}
}

EffectCarrier::Collector::~Collector( ) {}

void EffectCarrier::Collector::action( Object *hitObject )
{
	this->hitObjects.push_back( hitObject );
	this->hitPosW.push_back( hitObject->getOrientation().v[3].xyz );
}

EffectCarrier::EffectCarrier( Object *_owner, Weapon::Type _weapon ) : weapon(_weapon), state(Deployed), owner(_owner) {}
EffectCarrier::EffectCarrier( const EffectCarrier &carrier ) : weapon(carrier.weapon), state(carrier.state), owner(carrier.owner) {}
EffectCarrier::~EffectCarrier( ) { }

EffectCarrier & EffectCarrier::operator = ( const EffectCarrier & carrier )
{
	this->weapon = carrier.weapon;
	this->state = carrier.state;
	this->owner = carrier.owner;
	return *this;
}

void EffectCarrier::setState( const State &state )
{ this->state = state; }

void EffectCarrier::setWeaponType( Weapon::Type weapon )
{ this->weapon = weapon; }

Object * EffectCarrier::accessOwner( ) const
{ return this->owner; }

EffectCarrier::State EffectCarrier::onHit( const vector<Object*> &targets, const vector<Float3> &hitPosW )
{
	GameLUA::executeOnHitScript( this->weapon, this, targets, hitPosW );
	return this->state;
}