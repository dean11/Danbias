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

	this->afterCollision = NULL;
	this->onMovement = NULL;

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

void SimpleRigidBody::SetSubscription(EventAction_Move function)
{
	this->onMovement = function;
}

void SimpleRigidBody::SetLinearVelocity(Float3 velocity)
{
	this->rigidBody->setLinearVelocity(btVector3(velocity.x, velocity.y, velocity.z));
}

void SimpleRigidBody::SetRotation(Float4 quaternion)
{
	btTransform trans;
	this->motionState->getWorldTransform(trans);
	trans.setRotation(btQuaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w));
	this->motionState->setWorldTransform(trans);
}

void SimpleRigidBody::SetRotation(Float3 eulerAngles)
{
	btTransform trans;
	this->motionState->getWorldTransform(trans);
	trans.setRotation(btQuaternion(eulerAngles.x, eulerAngles.y, eulerAngles.z));
	this->motionState->setWorldTransform(trans);
}

void SimpleRigidBody::CallSubscription_AfterCollisionResponse(ICustomBody* bodyA, ICustomBody* bodyB, Oyster::Math::Float kineticEnergyLoss)
{
	if(this->onMovement)
		this->afterCollision(bodyA, bodyB, kineticEnergyLoss);
}

void SimpleRigidBody::CallSubscription_Move()
{
	if(this->onMovement)
		this->onMovement(this);
}

btCollisionShape* SimpleRigidBody::GetCollisionShape() const
{
	return this->collisionShape;
}

btDefaultMotionState* SimpleRigidBody::GetMotionState() const
{
	return this->motionState;
}

btRigidBody* SimpleRigidBody::GetRigidBody() const
{
	return this->rigidBody;
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


