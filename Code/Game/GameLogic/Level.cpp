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
	//API::SphericalBodyDescription sbDesc;
	//sbDesc.centerPosition = Oyster::Math::Float4(0,0,0,1);
	//sbDesc.ignoreGravity = true;
	//sbDesc.radius = radius;
	//sbDesc.mass = 1e16f; //10^16
	//sbDesc.mass = 0; //10^16
	//sbDesc.subscription_onCollision = CollisionManager::LevelCollision;
	//
	//ICustomBody* rigidBody = API::Instance().CreateRigidBody(sbDesc).Release();
	//API::Instance().AddObject(rigidBody);
	
	//API::Gravity gravityWell;
	//
	//gravityWell.gravityType = API::Gravity::GravityType_Well;
	//gravityWell.well.mass = 1e16f;
	//gravityWell.well.position = Oyster::Math::Float4(0,0,0,1);
	//
	//API::Instance().AddGravity(gravityWell);
	
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


