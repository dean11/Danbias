//////////////////////////////////////////////////
//Created by Erik and Linda of the GameLogic team
//////////////////////////////////////////////////


#ifndef REFMANAGER_H
#define REFMANAGER_H

#include<map>
#include "Object.h"
#include "PhysicsAPI.h"

namespace GameLogic
{

	class RefManager
	{
	public:
		RefManager(void);
		~RefManager(void);

		Object* GetMap(Oyster::Physics::ICustomBody &body); //returns the object of an rigidBody, mainly used for CollisionHandler
		void AddMapping(Oyster::Physics::ICustomBody &body, Object &obj); //adds a mapping with body as key and the object as a value


	private:
		std::map<Oyster::Physics::ICustomBody*,Object*> mapper; //mapper points a rigidBody to an actual game object

	};
}
#endif