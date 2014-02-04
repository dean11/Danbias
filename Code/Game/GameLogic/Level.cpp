#include "Level.h"
#include "CollisionManager.h"

using namespace GameLogic;
using namespace Utility::DynamicMemory;
using namespace Oyster::Physics;


Level::Level(void)
{

}
Level::~Level(void)
{
}

void Level::InitiateLevel(std::string levelPath)
{

}
void Level::InitiateLevel(float radius)
{

	// add level sphere
	API::SphericalBodyDescription sbDesc;
	sbDesc.centerPosition = Oyster::Math::Float4(0,0,0,1);
	sbDesc.ignoreGravity = true;
	sbDesc.radius = 300; 
	sbDesc.mass = 10e12f;
	sbDesc.frictionCoeff_Static = 0;
	sbDesc.frictionCoeff_Dynamic = 0;
	ICustomBody* rigidBody = API::Instance().CreateRigidBody(sbDesc).Release();
	
	ICustomBody::State state;
	rigidBody->GetState(state);
	state.SetRestitutionCoeff(0.2);
	rigidBody->SetState(state);
	
	levelObj = new StaticObject(rigidBody, LevelCollisionBefore, LevelCollisionAfter, OBJECT_TYPE::OBJECT_TYPE_WORLD);
	rigidBody->SetCustomTag(levelObj);
	
	//this->dynamicObjects = new DynamicArray< DynamicObject>;
	// add box
	API::SimpleBodyDescription sbDesc_TestBox;
	sbDesc_TestBox.centerPosition = Oyster::Math::Float4(10,320,0,0);
	sbDesc_TestBox.ignoreGravity = false;

	sbDesc_TestBox.mass = 50;
	sbDesc_TestBox.size = Oyster::Math::Float4(2,2,2,0);

	
	ICustomBody* rigidBody_TestBox = API::Instance().CreateRigidBody(sbDesc_TestBox).Release();
	rigidBody_TestBox->SetSubscription(Level::PhysicsOnMoveLevel);

	this->dynamicObjects.Push(new DynamicObject(rigidBody_TestBox,Object::DefaultCollisionBefore, Object::DefaultCollisionAfter, OBJECT_TYPE::OBJECT_TYPE_BOX));
	rigidBody_TestBox->SetCustomTag(this->dynamicObjects[0]);
	rigidBody_TestBox->GetState(state);
	state.ApplyLinearImpulse(Oyster::Math::Float3(0,0,0));
	rigidBody_TestBox->SetState(state);

	// add crystal
	API::SimpleBodyDescription sbDesc_Crystal;
	sbDesc_Crystal.centerPosition = Oyster::Math::Float4(10, 305, 0, 0);
	sbDesc_Crystal.ignoreGravity = false;

	sbDesc_Crystal.mass = 70;
	sbDesc_Crystal.size = Oyster::Math::Float4(2,3,2,0);


	ICustomBody* rigidBody_Crystal = API::Instance().CreateRigidBody(sbDesc_Crystal).Release();
	rigidBody_Crystal->SetSubscription(Level::PhysicsOnMoveLevel);
	this->dynamicObjects.Push(new DynamicObject(rigidBody_Crystal,Object::DefaultCollisionBefore, Object::DefaultCollisionAfter, OBJECT_TYPE::OBJECT_TYPE_BOX));
	rigidBody_Crystal->SetCustomTag(this->dynamicObjects[1]);
	rigidBody_Crystal->GetState(state);
	state.ApplyLinearImpulse(Oyster::Math::Float3(0,0,0));
	rigidBody_Crystal->SetState(state);

	// add house
	API::SimpleBodyDescription sbDesc_House;
	sbDesc_House.centerPosition = Oyster::Math::Float4(50, 300, 0, 0);
	sbDesc_House.ignoreGravity = false;

	sbDesc_House.mass = 70;
	sbDesc_House.size = Oyster::Math::Float4(2,3,2,0);


	ICustomBody* rigidBody_House = API::Instance().CreateRigidBody(sbDesc_House).Release();
	rigidBody_House->SetSubscription(Level::PhysicsOnMoveLevel);
	this->staticObjects.Push(new StaticObject(rigidBody_House,Object::DefaultCollisionBefore, Object::DefaultCollisionAfter, OBJECT_TYPE::OBJECT_TYPE_GENERIC));
	rigidBody_House->SetCustomTag(this->staticObjects[0]);
	rigidBody_House->GetState(state);
	state.ApplyLinearImpulse(Oyster::Math::Float3(0,0,0));
	rigidBody_House->SetState(state);
	
	
	// add gravitation 
	API::Gravity gravityWell;
	gravityWell.gravityType = API::Gravity::GravityType_Well;
	gravityWell.well.mass = 1e17f;
	gravityWell.well.position = Oyster::Math::Float4(0,0,0,1);
	API::Instance().AddGravity(gravityWell);
}

void Level::AddPlayerToTeam(Player *player, int teamID)
{
	this->teamManager.AddPlayerToTeam(player,teamID);
}

void Level::CreateTeam(int teamSize)
{
	this->teamManager.CreateTeam(teamSize);
}

void Level::RespawnPlayer(Player *player)
{
	this->teamManager.RespawnPlayerRandom(player);
}

Object* Level::GetObj( int ID) const
{
	for (int i = 0; i< this->dynamicObjects.Size(); i++)
	{
		if(this->dynamicObjects[i]->GetID() == ID)
			return this->dynamicObjects[i];
	}
	return NULL;
}
void Level::PhysicsOnMoveLevel(const ICustomBody *object)
{
	// function call from physics update when object was moved
	Object* temp = (Object*)object->GetCustomTag();
}
