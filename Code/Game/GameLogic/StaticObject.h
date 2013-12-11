//////////////////////////////////////////////////
//Created by Erik and Linda of the GameLogic team
//////////////////////////////////////////////////


#ifndef STATICOBJECT_H
#define STATICOBJECT_H

#include "Object.h"

namespace GameLogic
{

	class StaticObject
	{
	
	public:
		StaticObject();
		~StaticObject(void);

	private:
		struct PrivateData;
		PrivateData *myData;
	};

}

#endif