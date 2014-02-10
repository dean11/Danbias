#include "SimpleRigidBody.h"
#include "PhysicsAPI_Impl.h"

using namespace ::Oyster::Physics;
using namespace ::Oyster::Physics3D;
using namespace ::Oyster::Math3D;
using namespace ::Oyster::Collision3D;
using namespace ::Utility::DynamicMemory;
using namespace ::Utility::Value;

SimpleRigidBody::SimpleRigidBody()
{
	this->collisionShape = NULL;
	this->motionState = NULL;
	this->rigidBody = NULL;

	this->state.centerPos = Float3(0.0f, 0.0f, 0.0f);
	this->state.quaternion = Quaternion(Float3(0.0f, 0.0f, 0.0f), 1.0f);
	this->state.dynamicFrictionCoeff = 0.0f;
	this->state.staticFrictionCoeff = 0.0f;
	this->state.mass = 0.0f;
	this->state.restitutionCoeff = 0.0f;
	this->state.reach = Float3(0.0f, 0.0f, 0.0f);

	this->customTag = nullptr;
}

SimpleRigidBody::SimpleRigidBody( const API::SimpleBodyDescription &desc )
{	
	this->collisionShape = NULL;
	this->motionState = NULL;
	this->rigidBody = NULL;

	this->customTag = nullptr;
}

SimpleRigidBody::~SimpleRigidBody()
{
	delete this->motionState;
	this->motionState = NULL;
	delete this->collisionShape;
	this->collisionShape = NULL;
	delete this->rigidBody;
	this->rigidBody = NULL;
}

void SimpleRigidBody::SetCollisionShape(btCollisionShape* shape)
{
	this->collisionShape = shape;
}

void SimpleRigidBody::SetMotionState(btDefaultMotionState* motionState)
{
	this->motionState = motionState;
}

void SimpleRigidBody::SetRigidBody(btRigidBody* rigidBody)
{
	this->rigidBody = rigidBody;
	
}

void SimpleRigidBody::SetSubscription(EventAction_AfterCollisionResponse function)
{
	this->afterCollision = function;
}

void SimpleRigidBody::CallSubsciptMessage(ICustomBody* bodyA, ICustomBody* bodyB, Oyster::Math::Float kineticEnergyLoss)
{
	this->afterCollision(bodyA, bodyB, kineticEnergyLoss);
}

btDefaultMotionState* SimpleRigidBody::GetMotionState() const
{
	return this->motionState;
}

SimpleRigidBody::State SimpleRigidBody::GetState() const
{
	return this->state;
}

SimpleRigidBody::State & SimpleRigidBody::GetState( SimpleRigidBody::State &targetMem ) const
{
	targetMem = this->state;
	return targetMem;
}

void SimpleRigidBody::SetState( const SimpleRigidBody::State &state )
{
	this->state = state;
}

void * SimpleRigidBody::GetCustomTag() const
{
	return this->customTag;
}


void SimpleRigidBody::SetCustomTag( void *ref )
{
	this->customTag = ref;
}


