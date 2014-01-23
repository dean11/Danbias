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
	//sbDesc.centerPosition = 

	//poi
	ICustomBody* rigidBody = API::Instance().CreateRigidBody(sbDesc).Release();


	Oyster::Physics::API::Instance().AddObject(rigidBody);

	//rigidBody->gameObjectRef = this;

	this->objectID = GID();

	this->type = OBJECT_TYPE::OBJECT_TYPE_UNKNOWN;
}

Object::Object(void* collisionFunc, OBJECT_TYPE type)
{
	API::SimpleBodyDescription sbDesc;

	//poi
	this->rigidBody = API::Instance().CreateRigidBody(sbDesc).Release();

	Oyster::Physics::API::Instance().AddObject(rigidBody);
	
	rigidBody->SetSubscription((Oyster::Physics::ICustomBody::EventAction_Collision)(collisionFunc));

	//rigidBody->gameObjectRef = this;

	this->objectID = GID();

	this->type = type;
}

Object::Object(ICustomBody *rigidBody ,void* collisionFunc, OBJECT_TYPE type)
{
	Oyster::Physics::API::Instance().AddObject(rigidBody);

	rigidBody->SetSubscription((Oyster::Physics::ICustomBody::EventAction_Collision)(collisionFunc));

	this->objectID = GID();

	this->type = type;
}

void Object::ApplyLinearImpulse(Oyster::Math::Float4 force)
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

	//Oyster::Math::Float rot = (setState.GetGravityNormal().xyz).Dot(getState.GetGravityNormal().xyz);	
	//Oyster::Math::Float3 axis = (setState.GetGravityNormal().xyz).Cross(getState.GetGravityNormal().xyz);
	Oyster::Math::Float4x4 rotMatrix = setState.GetOrientation(); //Oyster::Math3D::RotationMatrix(rot, axis);
	Oyster::Math3D::SnapAxisYToNormal_UsingNlerp(rotMatrix, -setState.GetGravityNormal());
	setState.SetOrientation(rotMatrix);


	this->getState = this->rigidBody->GetState();


	
	this->setState = this->getState;
	
}