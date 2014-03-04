#ifndef GAMELOGIC_SPAWNPOINT_H
#define  GAMELOGIC_SPAWNPOINT_H
#include <DynamicArray.h>
#include "OysterMath.h"

namespace GameLogic
{
class SpawnPoint
{
private:
	Utility::DynamicMemory::DynamicArray<Oyster::Math::Float3> spawnPoints;

public:
	SpawnPoint();
	~SpawnPoint(void);
	void addSpawnPos(Oyster::Math::Float3 pos);
	Oyster::Math::Float3 getSpawnPos();
};
}
#endif // GAMELOGIC_SPAWNPOINT_H
