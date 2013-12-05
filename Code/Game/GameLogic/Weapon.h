//////////////////////////////////////////////////
//Created by Erik and Linda of the GameLogic team
//////////////////////////////////////////////////
#ifndef WEAPON_H
#define WEAPON_H

namespace GameLogic
{

	class Weapon
	{

	public:
		Weapon(void);
		~Weapon(void);

	private:
		struct PrivateData;
		PrivateData *myData;
	};

}
#endif