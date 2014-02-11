#include "PhysicsAPI_Impl.h"
#include "OysterPhysics3D.h"
#include "SimpleRigidBody.h"
#include "SphericalRigidBody.h"

using namespace ::Oyster;
using namespace ::Oyster::Physics;
using namespace ::Oyster::Math;
using namespace ::Oyster::Collision3D;
using namespace ::Utility::DynamicMemory;
using namespace ::Utility::Value;

API_Impl API_instance;

API & API::Instance()
{
	return API_instance;
}

API_Impl::API_Impl()
{
	this->broadphase = NULL;
	this->collisionConfiguration = NULL;
    this->dispatcher = NULL;
	this->solver = NULL;
	this->dynamicsWorld = NULL;

	this->timeStep = 1.0f/120.0f;

	this->gravityPoint = Float3(0.0f, 0.0f, 0.0f);
	this->gravity = 10.0f;
}

API_Impl::~API_Impl() 
{
	delete this->dynamicsWorld;
	this->dynamicsWorld = NULL;
    delete this->solver;
	this->solver = NULL;
    delete this->dispatcher;
	this->dispatcher = NULL;
    delete this->collisionConfiguration;
	this->collisionConfiguration = NULL;
    delete this->broadphase;
	this->broadphase = NULL;

	for(unsigned int i = 0; i < this->customBodies.size(); i++)
	{
		delete this->customBodies[i];
		this->customBodies[i] = NULL;
	}
}

void API_Impl::SetGravityPoint(::Oyster::Math::Float3 gravityPoint)
{
	this->gravityPoint = gravityPoint;
}

void API_Impl::SetGravity(float gravity)
{
	this->gravity = gravity;
}

// Bullet physics
ICustomBody* API_Impl::AddCollisionSphere(float radius, ::Oyster::Math::Float4 rotation, ::Oyster::Math::Float3 position, float mass, float restitution, float staticFriction, float dynamicFriction)
{
	SimpleRigidBody* body = new SimpleRigidBody;
	SimpleRigidBody::State state;

	// Add collision shape
	btCollisionShape* collisionShape = new btSphereShape(radius);
	body->SetCollisionShape(collisionShape);

	// Add motion state
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w),btVector3(position.x, position.y, position.z)));
	body->SetMotionState(motionState);

	// Add rigid body
	btVector3 fallInertia(0, 0, 0);
	collisionShape->calculateLocalInertia(mass, fallInertia);
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, collisionShape, fallInertia);
    btRigidBody* rigidBody = new btRigidBody(rigidBodyCI);
	rigidBody->setFriction(staticFriction);
	rigidBody->setRestitution(restitution);
	rigidBody->setUserPointer(body);
	body->SetRigidBody(rigidBody);

	// Add rigid body to world
	this->dynamicsWorld->addRigidBody(rigidBody);
	this->customBodies.push_back(body);

	state.centerPos = position;
	state.reach = Float3(radius, radius, radius);
	state.dynamicFrictionCoeff = 0.5f;
	state.staticFrictionCoeff = 0.5f;
	state.quaternion = Quaternion(Float3(rotation.xyz), rotation.w);
	state.mass = mass;

	body->SetState(state);

	return body;
}

ICustomBody* API_Impl::AddCollisionBox(Float3 halfSize, ::Oyster::Math::Float4 rotation, ::Oyster::Math::Float3 position, float mass, float restitution, float staticFriction, float dynamicFriction)
{
	SimpleRigidBody* body = new SimpleRigidBody;
	SimpleRigidBody::State state;

	// Add collision shape
	btCollisionShape* collisionShape = new btBoxShape(btVector3(halfSize.x, halfSize.y, halfSize.z));
	body->SetCollisionShape(collisionShape);

	// Add motion state
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w),btVector3(position.x, position.y, position.z)));
	body->SetMotionState(motionState);

	// Add rigid body
	btVector3 fallInertia(0, 0, 0);
	collisionShape->calculateLocalInertia(mass, fallInertia);
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, collisionShape, fallInertia);
    btRigidBody* rigidBody = new btRigidBody(rigidBodyCI);
	rigidBody->setFriction(staticFriction);
	rigidBody->setRestitution(restitution);
	rigidBody->setUserPointer(body);
	body->SetRigidBody(rigidBody);

	// Add rigid body to world
	this->dynamicsWorld->addRigidBody(rigidBody);
	this->customBodies.push_back(body);

	state.centerPos = position;
	state.reach = halfSize;
	state.dynamicFrictionCoeff = 0.5f;
	state.staticFrictionCoeff = 0.5f;
	state.quaternion = Quaternion(Float3(rotation.xyz), rotation.w);
	state.mass = mass;

	body->SetState(state);

	return body;
}

ICustomBody* API_Impl::AddCollisionCylinder(::Oyster::Math::Float3 halfSize, ::Oyster::Math::Float4 rotation, ::Oyster::Math::Float3 position, float mass, float restitution, float staticFriction, float dynamicFriction)
{
	SimpleRigidBody* body = new SimpleRigidBody;
	SimpleRigidBody::State state;

	// Add collision shape
	btCollisionShape* collisionShape = new btCylinderShape(btVector3(halfSize.x, halfSize.y, halfSize.z));
	body->SetCollisionShape(collisionShape);

	// Add motion state
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w),btVector3(position.x, position.y, position.z)));
	body->SetMotionState(motionState);

	// Add rigid body
	btVector3 fallInertia(0, 0, 0);
	collisionShape->calculateLocalInertia(mass, fallInertia);
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, collisionShape, fallInertia);
    btRigidBody* rigidBody = new btRigidBody(rigidBodyCI);
	rigidBody->setFriction(staticFriction);
	rigidBody->setRestitution(restitution);
	rigidBody->setUserPointer(body);
	body->SetRigidBody(rigidBody);

	// Add rigid body to world
	this->dynamicsWorld->addRigidBody(rigidBody);
	this->customBodies.push_back(body);

	state.centerPos = position;
	state.reach = halfSize;
	state.dynamicFrictionCoeff = 0.5f;
	state.staticFrictionCoeff = 0.5f;
	state.quaternion = Quaternion(Float3(rotation.xyz), rotation.w);
	state.mass = mass;

	body->SetState(state);

	return body;
}

void API_Impl::SetTimeStep(float timeStep)
{
	this->timeStep = timeStep;
}

void API_Impl::UpdateWorld()
{
	for(unsigned int i = 0; i < this->customBodies.size(); i++ )
	{
		this->customBodies[i]->SetGravity(-(this->customBodies[i]->GetState().centerPos - this->gravityPoint).GetNormalized()*this->gravity);	
	}

	this->dynamicsWorld->stepSimulation(this->timeStep, 1, this->timeStep);

	ICustomBody::State state;

	for(unsigned int i = 0; i < this->customBodies.size(); i++ )
	{
		SimpleRigidBody* simpleBody = dynamic_cast<SimpleRigidBody*>(this->customBodies[i]);
		btTransform trans;
		simpleBody->GetMotionState()->getWorldTransform(trans);
		this->customBodies[i]->SetPosition(Float3(trans.getOrigin().x(), trans.getOrigin().y(), trans.getOrigin().z()));
		this->customBodies[i]->SetRotation(Quaternion(Float3(trans.getRotation().x(), trans.getRotation().y(), trans.getRotation().z()), trans.getRotation().w()));
		
		if(simpleBody->GetRigidBody()->getActivationState() == ACTIVE_TAG)
		{
			simpleBody->CallSubscription_Move();
		}
	}

	int numManifolds = this->dynamicsWorld->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold =  this->dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject* obA = contactManifold->getBody0();
		const btCollisionObject* obB = contactManifold->getBody1();

		ICustomBody* bodyA = (ICustomBody*)obA->getUserPointer();
		ICustomBody* bodyB = (ICustomBody*)obB->getUserPointer();

		dynamic_cast<SimpleRigidBody*>(bodyA)->CallSubscription_AfterCollisionResponse(bodyA, bodyB, 0.0f);
		dynamic_cast<SimpleRigidBody*>(bodyB)->CallSubscription_AfterCollisionResponse(bodyB, bodyA, 0.0f);
	}

}

void API_Impl::Init()
{
	this->broadphase = new btDbvtBroadphase();
	this->collisionConfiguration = new btDefaultCollisionConfiguration();
    this->dispatcher = new btCollisionDispatcher(this->collisionConfiguration);
	this->solver = new btSequentialImpulseConstraintSolver;
	this->dynamicsWorld = new btDiscreteDynamicsWorld(this->dispatcher,this->broadphase,this->solver,this->collisionConfiguration);
	this->dynamicsWorld->setGravity(btVector3(0,-10,0));
}


bool API_Impl::IsInLimbo( const ICustomBody* objRef )
{
	return true;
}

void API_Impl::MoveToLimbo( const ICustomBody* objRef )
{
	
}

void API_Impl::ReleaseFromLimbo( const ICustomBody* objRef )
{
	
}

void API_Impl::ApplyEffect( const Oyster::Collision3D::ICollideable& collideable, void* args, void(hitAction)(ICustomBody*, void*) )
{
	
}

namespace Oyster 
{ 
	namespace Physics
	{
		namespace Default
		{
			void EventAction_Destruction( ::Utility::DynamicMemory::UniquePointer<::Oyster::Physics::ICustomBody> proto )
			{ /* Do nothing except allowing the proto uniquePointer destroy itself. */ }

			::Oyster::Physics::ICustomBody::SubscriptMessage EventAction_BeforeCollisionResponse( const ::Oyster::Physics::ICustomBody *proto, const ::Oyster::Physics::ICustomBody *deuter )
			{ /* Do nothing except returning business as usual. */
				return ::Oyster::Physics::ICustomBody::SubscriptMessage_none;
			}

			void EventAction_AfterCollisionResponse( const ::Oyster::Physics::ICustomBody *proto, const ::Oyster::Physics::ICustomBody *deuter, ::Oyster::Math::Float kineticEnergyLoss )
			{ /* Do nothing except returning business as usual. */ }

			void EventAction_Move( const ::Oyster::Physics::ICustomBody *object )
			{ /* Do nothing. */ }
		}
	} 
}