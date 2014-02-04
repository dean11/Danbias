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
	this->currPhysicsState = this->rigidBody->GetState();
	this->newPhysicsState = this->currPhysicsState;
}

Object::Object(OBJECT_TYPE type)
{
	API::SimpleBodyDescription sbDesc;

	this->rigidBody = API::Instance().CreateRigidBody(sbDesc).Release();
	Oyster::Physics::API::Instance().AddObject(rigidBody);
	this->type = type;
	this->objectID = GID();
	this->currPhysicsState = this->rigidBody->GetState();
	this->newPhysicsState = this->currPhysicsState;
}

Object::Object(Oyster::Physics::ICustomBody *rigidBody, OBJECT_TYPE type)
{
	Oyster::Physics::API::Instance().AddObject(rigidBody);
	this->rigidBody = rigidBody;
	this->type = type;
	this->objectID = GID();
	this->currPhysicsState = this->rigidBody->GetState();
	this->newPhysicsState = this->currPhysicsState;
}

Object::Object(void* collisionFuncBefore, void* collisionFuncAfter, OBJECT_TYPE type)
{
	API::SimpleBodyDescription sbDesc;

	this->rigidBody = API::Instance().CreateRigidBody(sbDesc).Release();
	Oyster::Physics::API::Instance().AddObject(rigidBody);
	
	this->type = type;
	this->objectID = GID();
	this->currPhysicsState = this->rigidBody->GetState();
	this->newPhysicsState = this->currPhysicsState;
}

Object::Object(Oyster::Physics::ICustomBody *rigidBody ,void* collisionFuncBefore, void* collisionFuncAfter, OBJECT_TYPE type)
{
	Oyster::Physics::API::Instance().AddObject(rigidBody);

	this->rigidBody = rigidBody;
	this->rigidBody->SetSubscription((Oyster::Physics::ICustomBody::EventAction_BeforeCollisionResponse)(collisionFuncBefore));
	this->rigidBody->SetSubscription((Oyster::Physics::ICustomBody::EventAction_AfterCollisionResponse)(collisionFuncAfter));	

	this->type = type;
	this->objectID = GID();
	this->currPhysicsState = this->rigidBody->GetState();
	this->newPhysicsState = this->currPhysicsState;
}

Object::Object(Oyster::Physics::ICustomBody *rigidBody ,Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncBefore)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter), Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncAfter)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), OBJECT_TYPE type)
{
	Oyster::Physics::API::Instance().AddObject(rigidBody);
	
	this->rigidBody = rigidBody;
	this->rigidBody->SetSubscription((Oyster::Physics::ICustomBody::EventAction_BeforeCollisionResponse)(collisionFuncBefore));
	this->rigidBody->SetSubscription((Oyster::Physics::ICustomBody::EventAction_AfterCollisionResponse)(collisionFuncAfter));

	
	this->type = type;
	this->objectID = GID();
	this->currPhysicsState = this->rigidBody->GetState();
	this->newPhysicsState = this->currPhysicsState;
}

void Object::ApplyLinearImpulse(Oyster::Math::Float3 force)
{
	newPhysicsState.ApplyLinearImpulse(force);
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
	
	
	if(currPhysicsState.GetLinearMomentum() !=currPhysicsState.GetLinearMomentum())
	{
		//error
		int i =0 ;
	}
	if(currPhysicsState.GetCenterPosition() !=currPhysicsState.GetCenterPosition())
	{
		//error
		int i =0 ;
	}
	if(currPhysicsState.GetAngularAxis() !=currPhysicsState.GetAngularAxis())
	{
		//error
		int i =0 ;
	}
	this->rigidBody->SetState(this->newPhysicsState);
}
// update physic 
void Object::EndFrame()
{
	if(currPhysicsState.GetLinearMomentum() !=currPhysicsState.GetLinearMomentum())
	{
		//error
		int i =0 ;
	}
	this->currPhysicsState = this->rigidBody->GetState();
	if(currPhysicsState.GetLinearMomentum() !=currPhysicsState.GetLinearMomentum())
	{
		//error
		int i =0 ;
	}
	if(currPhysicsState.GetGravityNormal() !=currPhysicsState.GetGravityNormal())
	{
		//error
		int i =0 ;
	}
	if(currPhysicsState.GetGravityNormal()!= Float3::null)
	{
		Oyster::Math::Float4 axis;
		Oyster::Math3D::SnapAngularAxis(Oyster::Math::Float4(currPhysicsState.GetAngularAxis(), 0), Oyster::Math::Float4::standard_unit_y, -Oyster::Math::Float4(currPhysicsState.GetGravityNormal()), axis);
		if(axis !=axis)
		{
			//error
			int i =0 ;
		}
		currPhysicsState.SetRotation(axis.xyz);
		currPhysicsState.SetAngularMomentum(Float3::null);
		Oyster::Math::Float3 debug = ::LinearAlgebra3D::WorldAxisOf(::LinearAlgebra3D::Rotation(axis.xyz), Oyster::Math::Float3::standard_unit_y);
		debug += currPhysicsState.GetGravityNormal();
	}


	if(currPhysicsState.GetLinearMomentum() !=currPhysicsState.GetLinearMomentum())
	{
		//error
		int i =0 ;
	}

	this->newPhysicsState = this->currPhysicsState;
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