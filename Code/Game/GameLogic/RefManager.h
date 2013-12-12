//////////////////////////////////////////////////
//Created by Erik and Linda of the GameLogic team
//////////////////////////////////////////////////


#ifndef REFMANAGER_H
#define REFMANAGER_H

#include<map>
#include "Object.h"

namespace GameLogic
{

	class RefManager
	{
	public:
		RefManager(void);
		~RefManager(void);

		static RefManager* getInstance( );
		void Release();
		

		Object* GetMap(const Oyster::Physics::ICustomBody &body); //returns the object of an rigidBody, mainly used for CollisionHandler
		void AddMapping(const Oyster::Physics::ICustomBody &body, Object &obj); //adds a mapping with body as key and the object as a value


	private:
		static RefManager* instance;
		std::map<const Oyster::Physics::ICustomBody*,Object*> mapper; //mapper points a rigidBody to an actual game object


	};
}
#endif