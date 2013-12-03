#include "RefManager.h"

using namespace GameLogic;

typedef std::pair<Oyster::Physics::ICustomBody*, Object*> mapData;

RefManager* RefManager::instance = 0;

RefManager::RefManager(void)
{
}


RefManager::~RefManager(void)
{
}

void RefManager::Release()
{
	if (instance)
	{
		delete instance;
		instance = NULL;
	}
	
}

RefManager* RefManager::getInstance( )
{
	if (!instance) 
	{
		instance = new RefManager();
	};
	return instance;
}

Object* RefManager::GetMap(Oyster::Physics::ICustomBody &body)
{
	return mapper[&body];
}

void RefManager::AddMapping(Oyster::Physics::ICustomBody &body, Object &obj)
{
	mapper.insert(mapData(&body,&obj));
}


