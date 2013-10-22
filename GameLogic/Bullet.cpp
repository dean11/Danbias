/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "OysterMath.h"
#include "Bullet.h"
#include "GameLUA.h"
#include "Event.h"
#include "Session.h"

using namespace ::Oyster::Math;
using namespace ::Oyster::Collision;
using namespace ::GameLogic;

namespace PrivateStatic
{
	// note current position is actually at the end of the lineOfEffect
	inline void updatePosition( Ray &trail, const Float &distance )
	{ trail.origin += trail.direction * distance; }

	inline Float3 posAt( const Ray &ray, const Float &distance )
	{ return ray.origin + (ray.direction * distance); }
}

Bullet::BulletCollector::BulletCollector( const Line *_sampler, int capacity )
	: EffectCarrier::Collector(capacity), sampler(_sampler), closest(::std::numeric_limits<Float>::max()) {}

Bullet::BulletCollector::~BulletCollector( ) {}

void Bullet::BulletCollector::action( Object *hitObject )
{
	if( this->hitObjects.size() == 0 )
	{
		this->closest = this->sampler->ray.collisionDistance;
		this->hitObjects.push_back( hitObject );
		this->hitPosW.push_back( PrivateStatic::posAt(this->sampler->ray, this->sampler->ray.collisionDistance) );
	}
	else if( this->closest > this->sampler->ray.collisionDistance )
	{
		this->closest = this->sampler->ray.collisionDistance;
		this->hitObjects.push_back( this->hitObjects[0] );
		this->hitPosW.push_back( this->hitPosW[0] );
		this->hitObjects[0] = hitObject;
		this->hitPosW[0] = PrivateStatic::posAt( this->sampler->ray, this->sampler->ray.collisionDistance );
	}
	else
	{
		this->hitObjects.push_back( hitObject );
		this->hitPosW.push_back( PrivateStatic::posAt(this->sampler->ray, this->sampler->ray.collisionDistance) );
	}
}

Bullet::Bullet( Player *owner, const Float3 &origin, const Float3 &normalizedDirection, const Float &_speed, Weapon::Type weapon, const Float &lifeTime )
	: EffectCarrier(owner, weapon), lineOfEffect(origin, normalizedDirection, _speed), speed(_speed), lifeCountDown(lifeTime), modelID(-1) {}
Bullet::Bullet( const Bullet &bullet ) : EffectCarrier(bullet), lineOfEffect(bullet.lineOfEffect), speed(bullet.speed), lifeCountDown(bullet.lifeCountDown), modelID(bullet.modelID) {}
Bullet::~Bullet( ) {}

Bullet & Bullet::operator = ( const Bullet &bullet )
{
	EffectCarrier::operator=( bullet );
	this->lineOfEffect = bullet.lineOfEffect;
	this->speed = bullet.speed;
	this->lifeCountDown = bullet.lifeCountDown;
	this->modelID = bullet.modelID;
	return *this;
}

void Bullet::setDirection( const Float3 &normalizedVector )
{ this->lineOfEffect.ray.direction = normalizedVector; }

void Bullet::setSpeed( const Float &coordLengthPerSecond )
{ this->speed = coordLengthPerSecond; }

void Bullet::setLifeTime( const Float &seconds )
{ this->lifeCountDown = seconds; }

void Bullet::setModelID( int ID )
{ this->modelID = ID; }

Float3 Bullet::getDirection() const
{
	return this->lineOfEffect.ray.direction;
}

EffectCarrier::State Bullet::update( const Float &deltaTime )
{
	switch( this->state )
	{
	case Deployed: this->state = Armed;
	default: case Armed:
		if( (this->lifeCountDown -= deltaTime) <= 0.0f )
		{
			this->state = Dead;
			printf("gammal och gaggig\n");
		}
		else
		{
			::PrivateStatic::updatePosition( this->lineOfEffect.ray, this->lineOfEffect.length );
			this->lineOfEffect.length = this->speed * deltaTime;
		}
	case Dead: break;
	};

	return this->state;
}

const ICollideable * Bullet::getVolumeOfEffect( ) const
{ return &this->lineOfEffect; }

EffectCarrier * Bullet::clone( ) const
{ return new Bullet(*this); }

EffectCarrier::Result Bullet::writeToKeyFrame( KeyFrame &buffer ) const
{
	if( buffer.numParticles < (int)::Utility::StaticArray::numElementsOf( buffer.Particle ) )
	{
		if( this->modelID >= 0 ) if( this->state != Dead )
		{
			buffer.Particle[buffer.numParticles].Head = this->lineOfEffect.ray.origin + (this->lineOfEffect.ray.direction * this->lineOfEffect.length);
			buffer.Particle[buffer.numParticles].Tail = this->lineOfEffect.ray.origin;
			buffer.Particle[buffer.numParticles].EffectType = this->modelID;
			++buffer.numParticles;
		}
		return Success;
	}
	return Failure;
}

EffectCarrier::Result Bullet::writeToNetEffect( Network::EffectData &buffer ) const
{
	if( this->modelID >= 0 ) if( this->state != Dead )
	{
		buffer.head = this->lineOfEffect.ray.origin + (this->lineOfEffect.ray.direction * /* this->lineOfEffect.length*/ 500);
		buffer.tail = this->lineOfEffect.ray.origin;
		buffer.identifier = 0;
		return Success;
	}
	return Failure;
}

using ::std::vector;
EffectCarrier::State Bullet::onHit( const vector<Object*> &target, const vector<Float3> &hitPosW )
{
	this->state = Dead;

	//target[0]->applyDamage( 10, *this->owner );
	//target[0]->applyForceW( this->lineOfEffect.ray.direction * 100.0f, hitPosW[0] );

	Player *owner = (Player *)this->owner;
	Event::BulletHit *hitEvent = new Event::BulletHit(owner->getPlayerID(), ((Player *)target[0])->getPlayerID()/*, target[0]->getOrientation().v[3].xyz*/);
	owner->getSession()->addEvent(hitEvent);

	return EffectCarrier::onHit( target, hitPosW );
}