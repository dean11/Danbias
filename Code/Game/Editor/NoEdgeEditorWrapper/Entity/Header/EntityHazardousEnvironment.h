#ifndef EDITOR_HAZARDOUSENV_H
#define EDITOR_HAZARDOUSENV_H

#include "Entity.h"

class EntityHazardousEnvironment :public Entity
{
public:
	EntityHazardousEnvironment(NoEdgeType_HazardEnv gameType , Oyster::Math::Float3 position);
	~EntityHazardousEnvironment();

private:


};

#endif // !EDITOR_HAZARDOUSENV_H
