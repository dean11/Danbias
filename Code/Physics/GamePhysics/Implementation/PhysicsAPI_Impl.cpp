#include "PhysicsAPI_Impl.h"
#include "OysterPhysics3D.h"
#include "SimpleRigidBody.h"

#include <codecvt>

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

	this->deltaTime = 0.0f;
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

	this->importer.deleteAllData();
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

	rigidBody->setCcdMotionThreshold(0.01f);
	rigidBody->setCcdSweptSphereRadius(radius);

	state.centerPos = position;
	state.reach = Float3(radius, radius, radius);
	state.dynamicFrictionCoeff = dynamicFriction;
	state.staticFrictionCoeff = staticFriction;
	state.quaternion = Quaternion(Float3(rotation.xyz), rotation.w);
	state.mass = mass;
	state.restitutionCoeff = restitution;

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

	rigidBody->setCcdMotionThreshold(0.01f);
	rigidBody->setCcdSweptSphereRadius(Min(halfSize.x, Min(halfSize.y, halfSize.z)));

	state.centerPos = position;
	state.reach = halfSize;
	state.dynamicFrictionCoeff = dynamicFriction;
	state.staticFrictionCoeff = staticFriction;
	state.quaternion = Quaternion(Float3(rotation.xyz), rotation.w);
	state.mass = mass;
	state.restitutionCoeff = restitution;

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

	rigidBody->setCcdMotionThreshold(0.01f);
	rigidBody->setCcdSweptSphereRadius(Min(halfSize.x, Min(halfSize.y, halfSize.z)));

	state.centerPos = position;
	state.reach = halfSize;
	state.dynamicFrictionCoeff = dynamicFriction;
	state.staticFrictionCoeff = staticFriction;
	state.quaternion = Quaternion(Float3(rotation.xyz), rotation.w);
	state.mass = mass;
	state.restitutionCoeff = restitution;

	body->SetState(state);

	return body;
}

ICustomBody* API_Impl::AddCharacter(::Oyster::Math::Float height, ::Oyster::Math::Float radius, ::Oyster::Math::Float4 rotation, ::Oyster::Math::Float3 position, float mass, float restitution, float staticFriction, float dynamicFriction)
{
	SimpleRigidBody* body = new SimpleRigidBody;
	SimpleRigidBody::State state;

	// Add collision shape
	btCapsuleShape* collisionShape = new btCapsuleShape(radius, height);
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
	rigidBody->setAngularFactor(0);
	rigidBody->setSleepingThresholds(0, 0);
	body->SetRigidBody(rigidBody);

	// Add rigid body to world
	this->dynamicsWorld->addRigidBody(rigidBody);
	this->customBodies.push_back(body);

	rigidBody->setCcdMotionThreshold(0.01f);
	rigidBody->setCcdSweptSphereRadius(Min(height, radius));

	state.centerPos = position;
	state.reach = Float3(radius, height, radius);
	state.dynamicFrictionCoeff = dynamicFriction;
	state.staticFrictionCoeff = staticFriction;
	state.quaternion = Quaternion(Float3(rotation.xyz), rotation.w);
	state.mass = mass;
	state.restitutionCoeff = restitution;

	body->SetState(state);

	return body;
}

ICustomBody* API_Impl::AddTriangleMesh(const std::wstring fileName, ::Oyster::Math::Float4 rotation, ::Oyster::Math::Float3 position, ::Oyster::Math::Float3 scale, float mass, float restitution, float staticFriction, float dynamicFriction)
{
	SimpleRigidBody* body = new SimpleRigidBody;
	SimpleRigidBody::State state;

	btBulletWorldImporter bulletFile(0);

	typedef std::codecvt_utf8<wchar_t> convert_typeX;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	std::string bulletPath = converterX.to_bytes(fileName);

	// Add collision shape
	this->importer.loadFile(bulletPath.c_str());
	btBvhTriangleMeshShape* collisionShape = (btBvhTriangleMeshShape*)this->importer.getCollisionShapeByIndex(this->nrOfShapes);
	collisionShape->setLocalScaling(btVector3(scale.x, scale.y, scale.z));
	this->nrOfShapes++;

	// Add motion state
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w),btVector3(position.x, position.y, position.z)));
	body->SetMotionState(motionState);

	// Add rigid body
	btVector3 fallInertia(0, 0, 0);
	//collisionShape->calcu%lateLocalInertia(mass, fallInertia);
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(0, motionState, collisionShape, fallInertia);
    btRigidBody* rigidBody = new btRigidBody(rigidBodyCI);
	rigidBody->setFriction(staticFriction);
	rigidBody->setRestitution(restitution);
	rigidBody->setUserPointer(body);
	body->SetRigidBody(rigidBody);

	// Add rigid body to world
	this->dynamicsWorld->addRigidBody(rigidBody);
	this->customBodies.push_back(body);

	state.centerPos = position;
	state.reach = Float3(0, 0, 0);
	state.dynamicFrictionCoeff = dynamicFriction;
	state.staticFrictionCoeff = staticFriction;
	state.quaternion = Quaternion(Float3(rotation.xyz), rotation.w);
	state.mass = 0;
	state.restitutionCoeff = restitution;

	body->SetState(state);

	return body;
}

void API_Impl::SetTimeStep(float timeStep)
{
	this->timeStep = timeStep;
}

void API_Impl::UpdateWorld(float deltaTime)
{
	for(unsigned int i = 0; i < this->customBodies.size(); i++ )
	{
		SimpleRigidBody* simpleBody = dynamic_cast<SimpleRigidBody*>(this->customBodies[i]);

		if(!simpleBody->IsInLimbo())
		{		
			if(!simpleBody->IsGravityOverrided())
			{
				this->customBodies[i]->SetGravity(-(this->customBodies[i]->GetState().centerPos - this->gravityPoint).GetNormalized()*this->gravity);
			}
			else
			{
				simpleBody->SetOverrideGravity(false);
			}
	
			simpleBody->PreStep(this->dynamicsWorld);
			simpleBody->SetPreviousVelocity(simpleBody->GetLinearVelocity());
		}
	}

	this->dynamicsWorld->stepSimulation(deltaTime, 4, this->timeStep);

	int numManifolds = this->dynamicsWorld->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold =  this->dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject* obA = contactManifold->getBody0();
		const btCollisionObject* obB = contactManifold->getBody1();

		ICustomBody* bodyA = (ICustomBody*)obA->getUserPointer();
		ICustomBody* bodyB = (ICustomBody*)obB->getUserPointer();
	
		int numContacts = contactManifold->getNumContacts();
		for (int j=0;j<numContacts;j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			if (pt.getDistance()<0.f)
			{
				const btVector3& ptA = pt.getPositionWorldOnA();
				const btVector3& ptB = pt.getPositionWorldOnB();
				const btVector3& normalOnB = pt.m_normalWorldOnB;

				bodyA->CallSubscription_AfterCollisionResponse(bodyA, bodyB);
				bodyB->CallSubscription_AfterCollisionResponse(bodyB, bodyA);
			}
		}	
	}

	ICustomBody::State state;

	for(unsigned int i = 0; i < this->customBodies.size(); i++ )
	{
		SimpleRigidBody* simpleBody = dynamic_cast<SimpleRigidBody*>(this->customBodies[i]);

		if(!simpleBody->IsInLimbo())
		{
			btTransform trans;
			trans = simpleBody->GetRigidBody()->getWorldTransform();

			if(Float3(trans.getOrigin().x(), trans.getOrigin().y(), trans.getOrigin().z()) != this->customBodies[i]->GetState().centerPos || Float3(trans.getRotation().x(), trans.getRotation().y(), trans.getRotation().z()) != this->customBodies[i]->GetState().quaternion.imaginary)
			{
				this->customBodies[i]->CallSubscription_Move();
				this->customBodies[i]->SetPosition(Float3(trans.getOrigin().x(), trans.getOrigin().y(), trans.getOrigin().z()));
				this->customBodies[i]->SetRotation(Quaternion(Float3(trans.getRotation().x(), trans.getRotation().y(), trans.getRotation().z()), trans.getRotation().w()));
			}		
		}
		else if(simpleBody->IsInLimbo())
		{
			this->dynamicsWorld->removeRigidBody(simpleBody->GetRigidBody());
		}
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
	this->nrOfShapes = 0;
}

void API_Impl::Release()
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
	this->customBodies.clear();
	this->importer.deleteAllData();
}

bool API_Impl::IsInLimbo( const ICustomBody* objRef )
{
	return dynamic_cast<SimpleRigidBody*>((ICustomBody*)objRef)->IsInLimbo();
}

void API_Impl::MoveToLimbo( const ICustomBody* objRef )
{
	dynamic_cast<SimpleRigidBody*>((ICustomBody*)objRef)->MoveToLimbo();
}

void API_Impl::ReleaseFromLimbo( const ICustomBody* objRef )
{
	this->dynamicsWorld->addRigidBody(dynamic_cast<SimpleRigidBody*>((ICustomBody*)objRef)->GetRigidBody());
	dynamic_cast<SimpleRigidBody*>((ICustomBody*)objRef)->ReleaseFromLimbo();
}

void API_Impl::ApplyEffect(Oyster::Collision3D::ICollideable* collideable, void* args, EventAction_ApplyEffect effect)
{
	Sphere* sphere;
	Box* box;
	Cone* cone;

	switch(collideable->type)
	{
		case ICollideable::Type::Type_sphere:
		{
			sphere = dynamic_cast<Sphere*>(collideable);
			// Add collision shape
			btSphereShape btSphere(sphere->radius);

			// Add motion state
			btDefaultMotionState state = btDefaultMotionState(btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f),btVector3(sphere->center.x, sphere->center.y, sphere->center.z)));

			// Add rigid body
			btRigidBody::btRigidBodyConstructionInfo rigidBodyCI = btRigidBody::btRigidBodyConstructionInfo(0, &state, &btSphere);
			btRigidBody body = btRigidBody(rigidBodyCI);

			ContactSensorCallback callback(body, effect, args);
			this->dynamicsWorld->contactTest(&body, callback);
		}
			break;

		case ICollideable::Type::Type_box:
		{
			box = dynamic_cast<Box*>(collideable);
			// Add collision shape
			btBoxShape btBox = btBoxShape(btVector3(box->boundingOffset.x, box->boundingOffset.y, box->boundingOffset.z));

			// Add motion state
			btDefaultMotionState state = btDefaultMotionState(btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f),btVector3(box->center.x, box->center.y, box->center.z)));

			// Add rigid body
			btRigidBody::btRigidBodyConstructionInfo rigidBodyCI = btRigidBody::btRigidBodyConstructionInfo(0, &state, &btBox);
			btRigidBody body = btRigidBody(rigidBodyCI);

			ContactSensorCallback callback(body, effect, args);
			this->dynamicsWorld->contactTest(&body, callback);
		}
			break;

		case ICollideable::Type::Type_cone:
		{
			cone = dynamic_cast<Cone*>(collideable);
			// Add collision shape
			btConeShapeZ coneShape = btConeShapeZ(cone->radius, cone->length);

			// Add motion state
			 btDefaultMotionState state = btDefaultMotionState(btTransform(btQuaternion(cone->quaternion.x, cone->quaternion.y, cone->quaternion.z, cone->quaternion.w)*btQuaternion(0, 1, 0, 0),btVector3(cone->center.x, cone->center.y, cone->center.z)));

			// Add rigid body
			btRigidBody::btRigidBodyConstructionInfo rigidBodyCI = btRigidBody::btRigidBodyConstructionInfo (0, &state, &coneShape);
			btRigidBody body = btRigidBody(rigidBodyCI);

			ContactSensorCallback callback(body, effect, args);
			this->dynamicsWorld->contactTest(&body, callback);
		}
			break;
		default:
			return;
	}
}

ICustomBody* API_Impl::RayClosestObjectNotMe(ICustomBody* self, Float3 origin, Float3 target)
{
	ClosestNotMe rayCallback(dynamic_cast<SimpleRigidBody*>(self)->GetRigidBody());

	if((origin - target).GetLength() != 0)
		this->dynamicsWorld->rayTest (btVector3(origin.x, origin.y, origin.z), btVector3(target.x, target.y, target.z), rayCallback);
	else
		return nullptr;

	if(rayCallback.hasHit())
	{
		//target.x = rayCallback.m_rayToWorld.x();
		//target.y = rayCallback.m_rayToWorld.y();
		//target.z = rayCallback.m_rayToWorld.z();
		return (ICustomBody*)rayCallback.m_collisionObject->getUserPointer();
	}
	else
		return nullptr;
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