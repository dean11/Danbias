//////////////////////////////////////////////////
//Created by Erik and Linda of the GameLogic team
//////////////////////////////////////////////////


#ifndef STATICOBJECT_H
#define STATICOBJECT_H

#include "Object.h"

namespace GameLogic
{

	class StaticObject : public Object
	{
	
	public:
		StaticObject();
		StaticObject(void* collisionFunc, OBJECT_TYPE type);
		~StaticObject(void);

	private:

	};

}

#endif