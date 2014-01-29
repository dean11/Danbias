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
	API::SimpleBodyDescription sbDesc;

	this->rigidBody = API::Instance().CreateRigidBody(sbDesc).Release();
	Oyster::Physics::API::Instance().AddObject(rigidBody);

	this->type = OBJECT_TYPE::OBJECT_TYPE_UNKNOWN;
	this->objectID = GID();
	this->getState = this->rigidBody->GetState();
	this->setState = this->getState;
}

Object::Object(OBJECT_TYPE type)
{
	API::SimpleBodyDescription sbDesc;

	this->rigidBody = API::Instance().CreateRigidBody(sbDesc).Release();
	Oyster::Physics::API::Instance().AddObject(rigidBody);
	this->type = type;
	this->objectID = GID();
	this->getState = this->rigidBody->GetState();
	this->setState = this->getState;
}

Object::Object(Oyster::Physics::ICustomBody *rigidBody, OBJECT_TYPE type)
{
	Oyster::Physics::API::Instance().AddObject(rigidBody);
	this->rigidBody = rigidBody;
	this->type = type;
	this->objectID = GID();
	this->getState = this->rigidBody->GetState();
	this->setState = this->getState;
}

Object::Object(void* collisionFuncBefore, void* collisionFuncAfter, OBJECT_TYPE type)
{
	API::SimpleBodyDescription sbDesc;

	this->rigidBody = API::Instance().CreateRigidBody(sbDesc).Release();
	Oyster::Physics::API::Instance().AddObject(rigidBody);
	
	this->type = type;
	this->objectID = GID();
	this->getState = this->rigidBody->GetState();
	this->setState = this->getState;
}

Object::Object(Oyster::Physics::ICustomBody *rigidBody ,void* collisionFuncBefore, void* collisionFuncAfter, OBJECT_TYPE type)
{
	Oyster::Physics::API::Instance().AddObject(rigidBody);

	this->rigidBody = rigidBody;
	this->rigidBody->SetSubscription((Oyster::Physics::ICustomBody::EventAction_BeforeCollisionResponse)(collisionFuncBefore));
	this->rigidBody->SetSubscription((Oyster::Physics::ICustomBody::EventAction_AfterCollisionResponse)(collisionFuncAfter));	

	this->type = type;
	this->objectID = GID();
	this->getState = this->rigidBody->GetState();
	this->setState = this->getState;
}

Object::Object(Oyster::Physics::ICustomBody *rigidBody ,Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncBefore)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter), Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncAfter)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), OBJECT_TYPE type)
{
	Oyster::Physics::API::Instance().AddObject(rigidBody);
	
	this->rigidBody = rigidBody;
	this->rigidBody->SetSubscription((Oyster::Physics::ICustomBody::EventAction_BeforeCollisionResponse)(collisionFuncBefore));
	this->rigidBody->SetSubscription((Oyster::Physics::ICustomBody::EventAction_AfterCollisionResponse)(collisionFuncAfter));

	
	this->type = type;
	this->objectID = GID();
	this->getState = this->rigidBody->GetState();
	this->setState = this->getState;
}

void Object::ApplyLinearImpulse(Oyster::Math::Float3 force)
{
	setState.ApplyLinearImpulse(force);
}


Object::~Object(void)
{

}

OBJECT_TYPE Object::GetType() const
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

	this->rigidBody->SetState(this->setState);

}
// update physic 
void Object::EndFrame()
{

	Oyster::Math::Float4x4 rotMatrix = setState.GetOrientation(); //Oyster::Math3D::RotationMatrix(rot, axis);
	//Oyster::Math3D::SnapAxisYToNormal_UsingNlerp(rotMatrix, -setState.GetGravityNormal());
	//setState.SetOrientation(rotMatrix);

	this->getState = this->rigidBody->GetState();
	this->setState = this->getState;
}

void Object::setBeforeCollisonFunc(Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncBefore)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter))
{
	this->rigidBody->SetSubscription((Oyster::Physics::ICustomBody::EventAction_BeforeCollisionResponse)(collisionFuncBefore));
}
void Object::setAfterCollisonFunc(Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncAfter)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss))
{
	this->rigidBody->SetSubscription((Oyster::Physics::ICustomBody::EventAction_AfterCollisionResponse)(collisionFuncAfter));
}

Oyster::Math::Float3 Object::GetPosition() 
{
	Oyster::Physics::ICustomBody::State state; 
	state = this->rigidBody->GetState();
	return state.GetCenterPosition();
}
Oyster::Math::Float4x4 Object::GetOrientation() 
{
	Oyster::Physics::ICustomBody::State state; 
	state = this->rigidBody->GetState();
	return state.GetOrientation();
}