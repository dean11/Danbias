//////////////////////////////////////////////////
//Created by Erik and Linda of the GameLogic team
//////////////////////////////////////////////////


#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H

namespace GameLogic
{


	class DynamicObject
	{

	public:
		DynamicObject();
		~DynamicObject(void);

		void Update();

	private:
		struct PrivateData;
		PrivateData *myData;
	};

}

#endif