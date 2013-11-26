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

		Object* GetMap(Oyster::Physics::ICustomBody &body);
		void AddMapping(Oyster::Physics::ICustomBody &body, Object &obj);


	private:
		std::map<Oyster::Physics::ICustomBody*,Object*> mapper; //shall be pointer from physics that map to an object

	};
}
#endif