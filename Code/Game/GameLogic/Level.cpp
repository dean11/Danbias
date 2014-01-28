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
	API::SphericalBodyDescription sbDesc;
	sbDesc.centerPosition = Oyster::Math::Float4(0,0,0,1);
	sbDesc.ignoreGravity = true;
	sbDesc.radius = 8; //radius;
	sbDesc.mass = 10e12f;
	//sbDesc.mass = 0; //10^16
	
	

	ICustomBody* rigidBody = API::Instance().CreateRigidBody(sbDesc).Release();
	
	
	ICustomBody::State state;
	rigidBody->GetState(state);
	state.SetRestitutionCoeff(0.1);
	rigidBody->SetState(state);

	levelObj = new StaticObject(rigidBody, LevelCollision, OBJECT_TYPE::OBJECT_TYPE_WORLD);
	rigidBody->SetCustomTag(levelObj);
	API::Instance().AddObject(rigidBody);
	
	API::SimpleBodyDescription sbDesc_TestBox;
	sbDesc_TestBox.centerPosition = Oyster::Math::Float4(5,15,0,0);
	sbDesc_TestBox.ignoreGravity = false;
	sbDesc_TestBox.mass = 10;
	sbDesc_TestBox.size = Oyster::Math::Float4(2,2,2,0);
	//sbDesc.mass = 0; //10^16

	
	ICustomBody* rigidBody_TestBox = API::Instance().CreateRigidBody(sbDesc_TestBox).Release();
	
	rigidBody_TestBox->SetSubscription(Level::PhysicsOnMoveLevel);
	testBox = new DynamicObject(rigidBody_TestBox,LevelCollision,OBJECT_TYPE::OBJECT_TYPE_BOX);
	rigidBody_TestBox->SetCustomTag(testBox);
	rigidBody_TestBox->GetState(state);
	state.ApplyLinearImpulse(Oyster::Math::Float4(0,0,4,0));
	rigidBody_TestBox->SetState(state);
	API::Instance().AddObject(rigidBody_TestBox);
	
	API::Gravity gravityWell;

	gravityWell.gravityType = API::Gravity::GravityType_Well;
	gravityWell.well.mass = 10e12f;
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
	return (Object*)testBox;
}
void Level::PhysicsOnMoveLevel(const ICustomBody *object)
{
	// function call from physics update when object was moved
	Object* temp = (Object*)object->GetCustomTag();
}
