#include "Object.h"
#include "OysterMath.h"
#include "CollisionManager.h"
#include "GID.h"
#include "PhysicsAPI.h"
#include "Game.h"


using namespace GameLogic;

using namespace Oyster::Math;
using namespace Oyster::Physics;

const Game *Object::gameInstance = (Game*)(&Game::Instance());

Object::Object()
{	

	this->rigidBody = API::Instance().AddCollisionBox(Oyster::Math::Float3(0.0f, 0.0f, 0.0f), Oyster::Math::Float4(0, 0, 0, 1), Oyster::Math::Float3(0, 0, 0), 0);

	this->type = OBJECT_TYPE::OBJECT_TYPE_UNKNOWN;
	this->objectID = GID();
	this->currPhysicsState = this->rigidBody->GetState();
	this->newPhysicsState = this->currPhysicsState;
}

Object::Object(OBJECT_TYPE type)
{
	this->rigidBody = API::Instance().AddCollisionBox(Oyster::Math::Float3(0.0f, 0.0f, 0.0f), Oyster::Math::Float4(0, 0, 0, 1), Oyster::Math::Float3(0, 0, 0), 0);
	this->type = type;
	this->objectID = GID();
	this->currPhysicsState = this->rigidBody->GetState();
	this->newPhysicsState = this->currPhysicsState;
}

Object::Object(Oyster::Physics::ICustomBody *rigidBody, OBJECT_TYPE type)
{
	this->rigidBody = rigidBody;
	this->type = type;
	this->objectID = GID();
	this->currPhysicsState = this->rigidBody->GetState();
	this->newPhysicsState = this->currPhysicsState;
}

Object::Object(void* collisionFuncBefore, void* collisionFuncAfter, OBJECT_TYPE type)
{
	this->rigidBody = API::Instance().AddCollisionBox(Oyster::Math::Float3(0.0f, 0.0f, 0.0f), Oyster::Math::Float4(0, 0, 0, 1), Oyster::Math::Float3(0, 0, 0), 0);
	
	this->type = type;
	this->objectID = GID();
	this->currPhysicsState = this->rigidBody->GetState();
	this->newPhysicsState = this->currPhysicsState;
}

Object::Object(Oyster::Physics::ICustomBody *rigidBody ,void* collisionFuncBefore, void* collisionFuncAfter, OBJECT_TYPE type)
{
	this->rigidBody = rigidBody;

	this->type = type;
	this->objectID = GID();
	this->currPhysicsState = this->rigidBody->GetState();
	this->newPhysicsState = this->currPhysicsState;
}

Object::Object(Oyster::Physics::ICustomBody *rigidBody ,Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncBefore)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter), Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncAfter)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), OBJECT_TYPE type)
{
	this->rigidBody = rigidBody;

	
	this->type = type;
	this->objectID = GID();
	this->currPhysicsState = this->rigidBody->GetState();
	this->newPhysicsState = this->currPhysicsState;
}

void Object::ApplyLinearImpulse(Oyster::Math::Float3 force)
{
	
}


Object::~Object(void)
{

}

OBJECT_TYPE Object::GetObjectType() const
{
	return this->type;
}
int Object::GetID() const
{
	return this->objectID;
}

Oyster::Physics::ICustomBody* Object::GetRigidBody()
{
	return this->rigidBody;
}


void Object::BeginFrame()
{
	

	
	this->rigidBody->SetState(this->newPhysicsState);
}
// update physic 
void Object::EndFrame()
{
	this->newPhysicsState = this->currPhysicsState;
}

void Object::setBeforeCollisonFunc(Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncBefore)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter))
{
	//this->rigidBody->SetSubscription((Oyster::Physics::ICustomBody::EventAction_BeforeCollisionResponse)(collisionFuncBefore));
}
void Object::setAfterCollisonFunc(Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncAfter)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss))
{
	this->rigidBody->SetSubscription((Oyster::Physics::ICustomBody::EventAction_AfterCollisionResponse)(collisionFuncAfter));
}

Oyster::Math::Float3 Object::GetPosition() 
{
	Oyster::Physics::ICustomBody::State state; 
	state = this->rigidBody->GetState();
	return state.centerPos;
}
Oyster::Math::Float4x4 Object::GetOrientation() 
{
	Oyster::Physics::ICustomBody::State state; 
	state = this->rigidBody->GetState();
	return state.GetOrientation();
}