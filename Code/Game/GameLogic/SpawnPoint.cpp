#include "SpawnPoint.h"

using namespace GameLogic;

SpawnPoint::SpawnPoint()
{
}


SpawnPoint::~SpawnPoint(void)
{
}
void SpawnPoint::addSpawnPos(Oyster::Math::Float3 pos)
{
	spawnPoints.Push(pos);
}
Oyster::Math::Float3 SpawnPoint::getSpawnPos()
{
	int i = rand() % spawnPoints.Size();
	Oyster::Math::Float3 spawnPoint = spawnPoints[i]; 
	return spawnPoint;
}