//////////////////////////////////////////////////
//Created by Erik and Linda of the GameLogic team
//////////////////////////////////////////////////
#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H
#include "Object.h"
#include "GameLogicDef.h"

namespace GameLogic
{


	class DANBIAS_GAMELOGIC_DLL DynamicObject : public Object
	{

	public:
		DynamicObject();
		DynamicObject(void* collisionFunc, OBJECT_TYPE type);
		~DynamicObject(void);

	private:

	};

}

#endif