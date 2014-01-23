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
	sbDesc.subscription_onCollisionResponse = Level::LevelCollision;
	levelObj = new StaticObject(OBJECT_TYPE::OBJECT_TYPE_WORLD);
	

	ICustomBody* rigidBody = API::Instance().CreateRigidBody(sbDesc).Release();
	rigidBody->SetCustomTag(levelObj);
	API::Instance().AddObject(rigidBody);
	ICustomBody::State state;
	rigidBody->GetState(state);
	state.SetRestitutionCoeff(0.1);
	rigidBody->SetState(state);

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


