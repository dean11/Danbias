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
	Oyster::Math::Float3 spawnPoint;

	if(spawnPoints.Size() == 0)
	{
		spawnPoint = Oyster::Math::Float3(0,180,0);
		return spawnPoint;
	}
	int i = rand() % spawnPoints.Size();
	spawnPoint = spawnPoints[i]; 
	return spawnPoint;
}