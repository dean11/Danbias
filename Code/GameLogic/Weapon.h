//////////////////////////////////////////////////
//Created by Erik and Linda of the GameLogic team
//////////////////////////////////////////////////


#ifndef WEAPON_H
#define WEAPON_H

#include "Object.h"

namespace GameLogic
{

	class Weapon : public Object
	{

	public:
		Weapon(std::wstring objFile);
		~Weapon(void);

	private:

	};

}
#endif