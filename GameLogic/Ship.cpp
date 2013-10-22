/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "Ship.h"
#include "OysterMath.h"
#include "SoundEngine.h"

using namespace ::Game;
using namespace ::GameLogic;
using namespace ::Oyster;
using namespace ::Oyster::Math;

Ship::Ship( const Float4x4 &orientation, const Float3 &centerOfMass ) :
	Object(orientation, centerOfMass), transform(Math::Float4x4::identity), worldOutOfDate(true), updatePeriod(0.0f)
{
	this->engineSound = SoundEngine::CreateSoundSource();
	this->engineSound->SetResource("..\\Content\\Sounds\\engine.wav");
	this->engineSound->SetLooping(true);
	this->engineSound->SetVolume(1.0f);
	//this->engineSound->Play();
}

Ship::~Ship( )
{
	SoundEngine::DestroySoundSource(this->engineSound);
}

void Ship::update( const ::Oyster::Math::Float &deltaTime )
{
	this->updatePeriod += deltaTime;
	if( this->updatePeriod >= MoveAble::getDiscreteTimeSlice() )
	{
		this->updatePeriod -= MoveAble::getDiscreteTimeSlice();
		Object::update( );
	}

	Math::transformMatrix( this->world, this->transform, this->body.orientation );
	this->worldOutOfDate = false;

	this->engineSound->SetPosition( this->body.orientation.v[3] );
}

const Math::Float4x4 & Ship::getWorldMatrix( ) const
{
	if( this->worldOutOfDate )
	{
		Math::transformMatrix( this->world, this->transform, this->body.orientation );
		this->worldOutOfDate = false;
	}
	return this->world;
}

void Ship::setMutation( const Math::Float4x4 &matrix )
{
	this->transform = matrix;
	this->worldOutOfDate = true;
}

void Ship::applyDamage( unsigned int magnitude, const Object &attacker )
{
}

void Ship::applyHeal( unsigned int magnitude )
{
}
