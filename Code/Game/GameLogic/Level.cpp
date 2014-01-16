#include "Level.h"
#include "StaticObject.h"
#include "DynamicObject.h"
#include "GameMode.h"
#include "Player.h"
#include "PhysicsAPI.h"
#include "TeamManager.h"
#include "DynamicArray.h"

using namespace GameLogic;
using namespace Utility::DynamicMemory;
using namespace Oyster::Physics;

struct Level::PrivateData
{
	PrivateData()	
	{
		
	}
	~PrivateData()
	{
	}

	SmartPointer<TeamManager> teamManager;

	DynamicArray<SmartPointer<StaticObject>> staticObjects;
		
	DynamicArray<SmartPointer<DynamicObject>> dynamicObjects;

	SmartPointer<GameMode> gameMode;

	SmartPointer<ICustomBody> rigidBodyLevel;

}myData;

Level::Level(void)
{
	myData = new PrivateData();
}


Level::~Level(void)
{
	delete myData;
}

void Level::InitiateLevel(std::string levelPath)
{

}
void Level::InitiateLevel(float radius)
{
	API::SphericalBodyDescription sbDesc;
	sbDesc.centerPosition = Oyster::Math::Float4(0,0,0,0);
	sbDesc.ignoreGravity = true;
	sbDesc.radius = radius;
	sbDesc.mass = 1e16; //10^20
	sbDesc.subscription = CollisionManager::LevelCollision;

	ICustomBody* rigidBody = API::Instance().CreateRigidBody(sbDesc).Release();

	
}

void Level::AddPlayerToTeam(Player *player, int teamID)
{
	myData->teamManager->AddPlayerToTeam(player,teamID);
}

void Level::CreateTeam(int teamSize)
{
	myData->teamManager->CreateTeam(teamSize);
}

void Level::RespawnPlayer(Player *player)
{
	myData->teamManager->RespawnPlayerRandom(player);
}


