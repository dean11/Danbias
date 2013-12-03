//////////////////////////////////////////////////
//Created by Erik and Linda of the GameLogic team
//////////////////////////////////////////////////


#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H

#include "Object.h"

namespace GameLogic
{


	class DynamicObject : public Object
	{

	public:
		DynamicObject(std::wstring objFile);
		~DynamicObject(void);

		void Update();

	};

}

#endif