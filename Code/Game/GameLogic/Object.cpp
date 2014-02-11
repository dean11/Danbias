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

	this->rigidBody = API::Instance().AddCollisionBox(Oyster::Math::Float3(0.0f, 0.0f, 0.0f), Oyster::Math::Float4(0, 0, 0, 1), Oyster::Math::Float3(0, 0, 0), 0, 0.5f, 0.8f, 0.6f);

	this->type = OBJECT_TYPE::OBJECT_TYPE_UNKNOWN;
	this->objectID = GID();
}

Object::Object(OBJECT_TYPE type)
{
	this->rigidBody = API::Instance().AddCollisionBox(Oyster::Math::Float3(0.0f, 0.0f, 0.0f), Oyster::Math::Float4(0, 0, 0, 1), Oyster::Math::Float3(0, 0, 0), 0, 0.5f, 0.8f, 0.6f);
	this->type = type;
	this->objectID = GID();
}

Object::Object(Oyster::Physics::ICustomBody *rigidBody, OBJECT_TYPE type)
{
	this->rigidBody = rigidBody;
	this->type = type;
	this->objectID = GID();
}

Object::Object(void* collisionFuncBefore, void* collisionFuncAfter, OBJECT_TYPE type)
{
	this->rigidBody = API::Instance().AddCollisionBox(Oyster::Math::Float3(0.0f, 0.0f, 0.0f), Oyster::Math::Float4(0, 0, 0, 1), Oyster::Math::Float3(0, 0, 0), 0, 0.5f, 0.8f, 0.6f);
	
	this->type = type;
	this->objectID = GID();
}

Object::Object(Oyster::Physics::ICustomBody *rigidBody ,void* collisionFuncBefore, void* collisionFuncAfter, OBJECT_TYPE type)
{
	this->rigidBody = rigidBody;

	this->type = type;
	this->objectID = GID();
}

Object::Object(Oyster::Physics::ICustomBody *rigidBody ,Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncBefore)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter), Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncAfter)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), OBJECT_TYPE type)
{
	this->rigidBody = rigidBody;

	this->type = type;
	this->objectID = GID();
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

}
// update physic 
void Object::EndFrame()
{

}

void Object::setBeforeCollisonFunc(Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncBefore)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter))
{
	//this->rigidBody->SetSubscription((Oyster::Physics::ICustomBody::EventAction_BeforeCollisionResponse)(collisionFuncBefore));
}
void Object::setAfterCollisonFunc(Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncAfter)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss))
{
	this->rigidBody->SetSubscription((Oyster::Physics::ICustomBody::EventAction_AfterCollisionResponse)(collisionFuncAfter));
}


Oyster::Math::Float4x4 Object::GetOrientation() 
{
	Oyster::Physics::ICustomBody::State state; 
	state = this->rigidBody->GetState();
	return state.GetOrientation();
}


Oyster::Math::Float3 Object::GetPosition()
{
	return this->position;
}
Oyster::Math::Float3 Object::GetRotation()
{
	return this->rotation;
}
Oyster::Math::Float3 Object::GetScaling()
{
	return this->scale;
}

void Object::SetPosition(Oyster::Math::Float3 position)
{
	this->position = position;
}
void Object::SetRotation(Oyster::Math::Float3 rotation)
{
	this->rotation = rotation;
}
void Object::SetScaling(Oyster::Math::Float3 scale)
{
	this->scale = scale;
}