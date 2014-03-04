#ifndef EDITOR_ENTITYSTATIC_H
#define EDITOR_ENTITYSTATIC_H

#include "Entity.h"

class EntityBuilding	:public Entity
{
public:
	EntityBuilding( NoEdgeType_Buildings gameType , Oyster::Math::Float3 position );
	~EntityBuilding();


private:
};

#endif // !EDITOR_ENTITYSTATIC_H
