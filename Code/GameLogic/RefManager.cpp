#include "RefManager.h"

using namespace GameLogic;

typedef std::pair<const Oyster::Physics::ICustomBody*, Object*> mapData;

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

Object* RefManager::GetMap(const Oyster::Physics::ICustomBody &body)
{
	return mapper[&body];
}

void RefManager::AddMapping( const Oyster::Physics::ICustomBody &body, Object &obj)
{
	mapper.insert(mapData(&body,&obj));
}


