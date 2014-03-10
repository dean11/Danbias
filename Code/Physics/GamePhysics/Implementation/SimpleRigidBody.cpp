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

	this->collisionFlags = 0;

	this->afterCollision = NULL;
	this->onMovement = NULL;

	this->overrideGravity = false;
	this->inLimbo = false;

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
	if(state.mass != 0)
		collisionShape->calculateLocalInertia(state.mass, fallInertia);
	this->rigidBody->setMassProps(state.mass, fallInertia);

	this->state = state;
}

void SimpleRigidBody::ApplyImpulse(Float3 impulse)
{
	this->rigidBody->applyCentralImpulse(btVector3(impulse.x, impulse.y, impulse.z));
	this->rigidBody->setActivationState(ACTIVE_TAG);
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
	this->collisionFlags = rigidBody->getCollisionFlags();
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

void SimpleRigidBody::AddRotationAroundY(::Oyster::Math::Float angle)
{
	btTransform trans;
	btQuaternion quaternion;

	trans = this->rigidBody->getWorldTransform();

	quaternion = btQuaternion(trans.getBasis().getColumn(1), angle);
	trans.setRotation(quaternion*trans.getRotation());
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
	if(mass != 0)
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
	btQuaternion newRotation;
	btTransform trans;
	trans = this->rigidBody->getWorldTransform();

	btVector3 v1 = trans.getBasis().getColumn(1);
	btVector3 v2(up.x, up.y, up.z);

	btQuaternion q;
	btVector3 a = v1.cross(v2);

	if (v1.dot(v2) < -0.999999)
	{
		btVector3 xCrossPre = btVector3(1, 0 ,0).cross(v1);
		if(xCrossPre.length() < 0.000001)
			xCrossPre = btVector3(0, 1 ,0).cross(v1);
		xCrossPre.normalize();
		q.setRotation(xCrossPre, 3.1415f);
	}
	else if (v1.dot(v2) > 0.999999)
	{
		q = btQuaternion(0, 0, 0, 1);
	}
	else
	{
		q.setX(a.x());
		q.setY(a.y());
		q.setZ(a.z());

		q.setW(1 + v1.dot(v2));

		q.normalize();
	}

	newRotation = q*trans.getRotation();

	trans.setRotation(newRotation);
	this->rigidBody->setWorldTransform(trans);

	this->state.quaternion = Quaternion(Float3(newRotation.x(), newRotation.y(), newRotation.z()), newRotation.w());
}

Float4x4 SimpleRigidBody::GetRotation() const
{
	return this->state.GetRotation();
}
Float4 SimpleRigidBody::GetRotationAsAngularAxis()
{
	Float4 axis = Float4::null;
	btTransform trans;

	trans = this->rigidBody->getWorldTransform();
	axis.xyz = trans.getRotation().getAxis();
	axis.w = trans.getRotation().getAngle();

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
	btVector3 gravity = this->rigidBody->getGravity();
	return Float3(gravity.x(), gravity.y(), gravity.z());
}

Float3 SimpleRigidBody::GetLinearVelocity() const
{
	btVector3 linearVelocity = this->rigidBody->getLinearVelocity();
	return Float3(linearVelocity.x(), linearVelocity.y(), linearVelocity.z());
}

void SimpleRigidBody::OverrideGravity(const ::Oyster::Math::Float3& point, const ::Oyster::Math::Float gravityForce)
{
	this->overrideGravity = true;
	if(this->state.centerPos - point != Float3::null)
		this->SetGravity(-(this->state.centerPos - point).GetNormalized()*gravityForce);
}

void SimpleRigidBody::SetOverrideGravity(bool overrideGravity)
{
	this->overrideGravity = overrideGravity;
}

bool SimpleRigidBody::IsGravityOverrided()
{
	return this->overrideGravity;
}

void SimpleRigidBody::CallSubscription_AfterCollisionResponse(ICustomBody* bodyA, ICustomBody* bodyB)
{
	if(this->afterCollision)
		this->afterCollision(bodyA, bodyB);
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
	xform = this->rigidBody->getWorldTransform ();
	btVector3 down(-xform.getBasis().getColumn(1));
	btVector3 forward(xform.getBasis().getColumn(2));
	down.normalize();
	forward.normalize();

	this->raySource[0] = xform.getOrigin();
	this->raySource[1] = xform.getOrigin();

	btVector3 targetPlus = down*this->state.reach.y*btScalar(1.1);

	this->rayTarget[0] = this->raySource[0] + targetPlus;
	targetPlus = this->raySource[1] + forward*this->state.reach.z*btScalar(1.1);
	this->rayTarget[1] = btVector3(targetPlus.x(), targetPlus.y(), targetPlus.z());

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
			if(i == 0 && this->state.staticFrictionCoeff == 0)
			{
				btVector3 hitNormal = rayCallback.m_hitNormalWorld;
				btScalar angle = hitNormal.normalized().dot(down);

				if(angle < 0.0f)
					this->rigidBody->setFriction(1.6f*fabs(angle));
				else
					this->rigidBody->setFriction(0.0f);
			}
		} 
		else 
		{
			this->rayLambda[i] = 1.0;
		}
	}
}

float SimpleRigidBody::GetLambdaUp() const
{
	return this->rayLambda[0];
}

float SimpleRigidBody::GetLambdaForward() const
{
	return this->rayLambda[1];
}

void SimpleRigidBody::MoveToLimbo()
{
	this->inLimbo = true;
}

void SimpleRigidBody::ReleaseFromLimbo()
{
	this->inLimbo = false;
}

bool SimpleRigidBody::IsInLimbo()
{
	return this->inLimbo;
}

void SimpleRigidBody::SetPreviousVelocity(::Oyster::Math::Float3 velocity)
{
	this->state.previousVelocity = velocity;
}

void SimpleRigidBody::DisableCollisionResponse()
{
	this->rigidBody->setCollisionFlags(this->rigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
}

void SimpleRigidBody::TurnOnCollisionResponse()
{
	this->rigidBody->setCollisionFlags(this->collisionFlags);
}