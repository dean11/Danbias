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

SimpleRigidBody::State SimpleRigidBody::GetState() const
{
	return this->state;
}

SimpleRigidBody::State& SimpleRigidBody::GetState( SimpleRigidBody::State &targetMem ) const
{
	targetMem = this->state;
	return targetMem;
}

void SimpleRigidBody::SetState( const SimpleRigidBody::State &state )
{
	btTransform trans;
	btVector3 position(state.centerPos.x, state.centerPos.y, state.centerPos.z);
	btQuaternion quaternion(state.quaternion.imaginary.x, state.quaternion.imaginary.y, state.quaternion.imaginary.z, state.quaternion.real);
	this->motionState->getWorldTransform(trans);
	trans.setRotation(quaternion);
	trans.setOrigin(position);
	this->motionState->setWorldTransform(trans);
	this->rigidBody->setFriction(state.staticFrictionCoeff);
	this->rigidBody->setRestitution(state.restitutionCoeff);
	btVector3 fallInertia(0, 0, 0);
	collisionShape->calculateLocalInertia(state.mass, fallInertia);
	this->rigidBody->setMassProps(state.mass, fallInertia);

	this->state = state;
}

void SimpleRigidBody::ApplyImpulse(Float3 impulse)
{
	this->rigidBody->applyImpulse(btVector3(impulse.x, impulse.y, impulse.z), btVector3(0.0f, 0.0f, 0.0f));
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

void SimpleRigidBody::SetPosition(::Oyster::Math::Float3 position)
{
	btTransform trans;
	trans = this->rigidBody->getWorldTransform();
	trans.setOrigin(btVector3(position.x, position.y, position.z));
	this->rigidBody->setWorldTransform(trans);
	this->state.centerPos = position;
}

void SimpleRigidBody::SetRotation(Float4 quaternion)
{
	btTransform trans;
	trans = this->rigidBody->getWorldTransform();
	trans.setRotation(btQuaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w));
	this->rigidBody->setWorldTransform(trans);
	this->state.quaternion = Quaternion(quaternion.xyz, quaternion.w);
}

void SimpleRigidBody::SetRotation(::Oyster::Math::Quaternion quaternion)
{
	btTransform trans;
	trans = this->rigidBody->getWorldTransform();
	trans.setRotation(btQuaternion(quaternion.imaginary.x, quaternion.imaginary.y, quaternion.imaginary.z, quaternion.real));
	this->rigidBody->setWorldTransform(trans);
	this->state.quaternion = quaternion;
}

void SimpleRigidBody::SetRotation(Float3 eulerAngles)
{
	btTransform trans;
	trans = this->rigidBody->getWorldTransform();
	trans.setRotation(btQuaternion(eulerAngles.x, eulerAngles.y, eulerAngles.z));
	this->rigidBody->setWorldTransform(trans);
	this->state.quaternion = Quaternion(Float3(trans.getRotation().x(), trans.getRotation().y(), trans.getRotation().z()), trans.getRotation().w());
}

void SimpleRigidBody::SetRotation(::Oyster::Math::Float4x4 rotation)
{
	btTransform trans;
	btMatrix3x3 newRotation;
	btVector3 rightVector(rotation.v[0].x, rotation.v[0].y, rotation.v[0].z);
	btVector3 upVector(rotation.v[1].x, rotation.v[1].y, rotation.v[1].z);
	btVector3 forwardVector(rotation.v[2].x, rotation.v[2].y, rotation.v[2].z);
	

	newRotation[0] = rightVector;
	newRotation[1] = upVector;
	newRotation[2] = forwardVector;

	trans = this->rigidBody->getWorldTransform();
	trans.setBasis(newRotation);
	this->rigidBody->setWorldTransform(trans);

	btQuaternion quaternion;
	quaternion = trans.getRotation();
	this->state.quaternion = Quaternion(Float3(quaternion.x(), quaternion.y(), quaternion.z()), quaternion.w());
}

void SimpleRigidBody::SetRotationAsAngularAxis(::Oyster::Math::Float4 angularAxis)
{
	if(angularAxis.xyz.GetMagnitude() == 0)
	{
		return;
	}

	btTransform trans;
	btVector3 vector(angularAxis.x, angularAxis.y, angularAxis.z);
	btQuaternion quaternion(vector, angularAxis.w);

	trans = this->rigidBody->getWorldTransform();
	trans.setRotation(quaternion);
	this->rigidBody->setWorldTransform(trans);

	this->state.quaternion = Quaternion(Float3(quaternion.x(), quaternion.y(), quaternion.z()), quaternion.w());
}

void SimpleRigidBody::SetAngularFactor(Float factor)
{
	this->rigidBody->setAngularFactor(factor);
}

void SimpleRigidBody::SetMass(Float mass)
{
	btVector3 fallInertia(0, 0, 0);
	collisionShape->calculateLocalInertia(mass, fallInertia);
	this->rigidBody->setMassProps(mass, fallInertia);
	this->state.mass = mass;
}

void SimpleRigidBody::SetGravity(Float3 gravity)
{
	this->rigidBody->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
	this->gravity = gravity;
}

void SimpleRigidBody::SetUpAndRight(::Oyster::Math::Float3 up, ::Oyster::Math::Float3 right)
{
	btTransform trans;
	btMatrix3x3 rotation;
	btVector3 upVector(up.x, up.y, up.z);
	btVector3 rightVector(right.x, right.y, right.z);
	rotation[1] = upVector.normalized();
	rotation[0] = rightVector.normalized();
	rotation[2] = rightVector.cross(upVector).normalized();

	trans = this->rigidBody->getWorldTransform();
	trans.setBasis(rotation);
	this->rigidBody->setWorldTransform(trans);

	btQuaternion quaternion;
	quaternion = trans.getRotation();
	this->state.quaternion = Quaternion(Float3(quaternion.x(), quaternion.y(), quaternion.z()), quaternion.w());
}

void SimpleRigidBody::SetUpAndForward(::Oyster::Math::Float3 up, ::Oyster::Math::Float3 forward)
{
	btTransform trans;
	btMatrix3x3 rotation;
	btVector3 upVector(up.x, up.y, up.z);
	btVector3 forwardVector(forward.x, forward.y, forward.z);
	rotation[1] = upVector.normalized();
	rotation[2] = forwardVector.normalized();
	rotation[0] = upVector.cross(forwardVector).normalized();
	trans = this->rigidBody->getWorldTransform();
	trans.setBasis(rotation);
	this->rigidBody->setWorldTransform(trans);

	btQuaternion quaternion;
	quaternion = trans.getRotation();
	this->state.quaternion = Quaternion(Float3(quaternion.x(), quaternion.y(), quaternion.z()), quaternion.w());
}

void SimpleRigidBody::SetUp(::Oyster::Math::Float3 up)
{
	Float3 vector = Float3(0, 1, 0).Cross(up);

	if(vector == Float3::null)
	{
		return;
	}

	Float sine = vector.GetLength();
	Float cosine = acos(Float3(0, 1, 0).Dot(up));

	btQuaternion quaternion(btVector3(vector.x, vector.y, vector.z),cosine);

	btTransform trans;
	trans = this->rigidBody->getWorldTransform();
	trans.setRotation(quaternion);
	this->rigidBody->setWorldTransform(trans);
	this->state.quaternion = Quaternion(Float3(quaternion.x(), quaternion.y(), quaternion.z()), quaternion.w());
}

Float4x4 SimpleRigidBody::GetRotation() const
{
	return this->state.GetRotation();
}
Float4 SimpleRigidBody::GetRotationAsAngularAxis()
{
	Float4 axis = Float4::null;
	Float s = sqrtf(1 - this->state.quaternion.real*this->state.quaternion.real);

	axis.w = 2*acos(this->state.quaternion.real*this->state.quaternion.real);

	if(1 - this->state.quaternion.real > 0.001f)
	{
		axis.x = this->state.quaternion.imaginary.x/s;
		axis.y = this->state.quaternion.imaginary.y/s;
		axis.z = this->state.quaternion.imaginary.z/s;
	}
	else
	{
		axis.x = this->state.quaternion.imaginary.x;
		axis.y = this->state.quaternion.imaginary.y;
		axis.z = this->state.quaternion.imaginary.z;
	}

	return axis;
}

Float4x4 SimpleRigidBody::GetOrientation() const
{
	return this->state.GetOrientation();
}

Float4x4 SimpleRigidBody::GetView() const
{
	return this->state.GetView();
}

Float4x4 SimpleRigidBody::GetView( const ::Oyster::Math::Float3 &offset ) const
{
	return this->state.GetView(offset);
}

Float3 SimpleRigidBody::GetGravity() const
{
	return this->rigidBody->getGravity();
}
Float3 SimpleRigidBody::GetLinearVelocity() const
{
	return this->rigidBody->getLinearVelocity();
}


void SimpleRigidBody::CallSubscription_AfterCollisionResponse(ICustomBody* bodyA, ICustomBody* bodyB, Oyster::Math::Float kineticEnergyLoss)
{
	if(this->afterCollision)
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

void * SimpleRigidBody::GetCustomTag() const
{
	return this->customTag;
}


void SimpleRigidBody::SetCustomTag( void *ref )
{
	this->customTag = ref;
}


void SimpleRigidBody::PreStep (const btCollisionWorld* collisionWorld)
{
	btTransform xform;
	this->rigidBody->getMotionState()->getWorldTransform (xform);
	btVector3 down = -xform.getBasis()[1];
	btVector3 forward = xform.getBasis()[2];
	down.normalize ();
	forward.normalize();

	this->raySource[0] = xform.getOrigin();
	this->raySource[1] = xform.getOrigin();

	this->rayTarget[0] = this->raySource[0] + down * this->state.reach.y * btScalar(1.1);
	this->rayTarget[1] = this->raySource[1] + forward * this->state.reach.y * btScalar(1.1);

	class ClosestNotMe : public btCollisionWorld::ClosestRayResultCallback
	{
	public:
		ClosestNotMe (btRigidBody* me) : btCollisionWorld::ClosestRayResultCallback(btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0))
		{
			m_me = me;
		}

		virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult,bool normalInWorldSpace)
		{
			if (rayResult.m_collisionObject == m_me)
				return 1.0;

			return ClosestRayResultCallback::addSingleResult (rayResult, normalInWorldSpace);
		}
	protected:
		btRigidBody* m_me;
	};

	ClosestNotMe rayCallback(this->rigidBody);

	int i = 0;
	for (i = 0; i < 2; i++)
	{
		rayCallback.m_closestHitFraction = 1.0;
		if((this->raySource[i] - this->rayTarget[i]).length() != 0)
			collisionWorld->rayTest (this->raySource[i], this->rayTarget[i], rayCallback);
		if (rayCallback.hasHit())
		{
			this->rayLambda[i] = rayCallback.m_closestHitFraction;
		} else {
			this->rayLambda[i] = 1.0;
		}
	}
}

float SimpleRigidBody::GetLamda() const
{
	return this->rayLambda[0];
}