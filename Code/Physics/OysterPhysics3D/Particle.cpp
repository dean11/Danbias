/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "Particle.h"
#include "Utilities.h"

using namespace ::Oyster::Math3D;
using namespace ::Oyster::Collision3D;
using namespace ::Oyster::Physics3D;

Particle::Particle( const Sphere &s, Float m )
	: sphere(s), linearMomentum(Float3::null), impulseForceSum(Float3::null)
{
	if( m == 0.0f )
	{
		this->mass = ::Utility::Value::numeric_limits<Float>::epsilon();
	}
	else
	{
		this->mass = m;
	}
}

Particle & Particle::operator = ( const Particle &particle )
{
	this->sphere = particle.sphere;
	this->linearMomentum = particle.linearMomentum;
	this->impulseForceSum = particle.impulseForceSum;
	this->mass = particle.mass;
	return *this;
}

void Particle::Update_LeapFrog( Float deltaTime )
{ // Euler leap frog update when Runga Kutta is not needed
	this->impulseForceSum *= (deltaTime / this->mass);  // is now deltaLinearVelocity ( dt * a = dt * F / m )
	this->sphere.center.xyz += deltaTime * ::Utility::Value::AverageWithDelta( Formula::LinearVelocity(this->mass, this->linearMomentum), this->impulseForceSum );
	this->linearMomentum += Formula::LinearMomentum( this->mass, this->impulseForceSum );
	this->impulseForceSum = Float3::null;

	/* Fun notation
		the operations above can be written like this
		p = p0 + dt*( v0 + dt*(F/m)/2 )
		  = p0 + dt*v0 + dt^2*(F/m)/2
		 there:
		 * dt = deltaTime
		 * p  = this->sphere.position at the end of deltaTime
		 * p0 = this->sphere.position at beginning of deltaTime
		 * v0 = this->velocity at beginning of deltaTime
		 * m  = this->mass
		 * F  = this->sumForces pushing during deltaTime
		 
		 It is the law of motion with constant acceleration.
	*/
}

void Particle::ApplyImpulseForce( const Float3 &f )
{
	this->impulseForceSum += f;
}

void Particle::ApplyLinearImpulseAcceleration( const Float3 &a )
{
	this->impulseForceSum += this->mass * a;
}

Float3 & Particle::AccessCenter()
{
	return this->sphere.center.xyz;
}

const Float3 & Particle::AccessCenter() const
{
	return this->sphere.center.xyz;
}

Float & Particle::AccessRadius()
{
	return this->sphere.radius;
}

const Float & Particle::AccessRadius() const
{
	return this->sphere.radius;
}

const Float & Particle::GetMass() const
{
	return this->mass;
}

const Float3 & Particle::GetCenter() const
{
	return this->sphere.center.xyz;
}

const Float & Particle::GetRadius() const
{
	return this->sphere.radius;
}

const Float3 & Particle::GetImpulseForce() const
{
	return this->impulseForceSum;
}

const Float3 & Particle::GetLinearMomentum() const
{
	return this->linearMomentum;
}

Float3 Particle::GetLinearImpulseAcceleration() const
{
	return Formula::LinearImpulseAcceleration( this->mass, this->impulseForceSum );
}

Float3 Particle::GetLinearVelocity() const
{
	return Formula::LinearVelocity( this->mass, this->linearMomentum );
}

void Particle::SetMass_KeepVelocity( Float m )
{
	Float3 velocity = Formula::LinearVelocity( this->mass, this->linearMomentum );
	this->SetMass_KeepMomentum( m );
	this->linearMomentum = Formula::LinearMomentum( this->mass, velocity );
}

void Particle::SetMass_KeepMomentum( Float m )
{
	if( m == 0.0f )
	{
		this->mass = ::Utility::Value::numeric_limits<Float>::epsilon();
	}	
	else
	{
		this->mass = m;
	}
}

void Particle::SetCenter( const Float3 &p )
{
	this->sphere.center = p;
}

void Particle::SetRadius( Float r )
{
	this->sphere.radius = r;
}

void Particle::SetImpulseForce( const Float3 &f )
{
	this->impulseForceSum = f;
}

void Particle::SetLinearMomentum( const Float3 &g )
{
	this->linearMomentum = g;
}

void Particle::SetLinearImpulseAcceleration( const Float3 &a )
{
	this->impulseForceSum = Formula::ImpulseForce( this->mass, a );
}

void Particle::SetLinearVelocity( const Float3 &v )
{
	this->linearMomentum = Formula::LinearMomentum( this->mass, v );
}
