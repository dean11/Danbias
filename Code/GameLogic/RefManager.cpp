#include "RefManager.h"

using namespace GameLogic;

typedef std::pair<Oyster::Physics::ICustomBody*, Object*> mapData;

RefManager::RefManager(void)
{
}


RefManager::~RefManager(void)
{
}

Object* RefManager::GetMap(Oyster::Physics::ICustomBody &body)
{
	return mapper[&body];
}

void RefManager::AddMapping(Oyster::Physics::ICustomBody &body, Object &obj)
{
	mapper.insert(mapData(&body,&obj));
}


