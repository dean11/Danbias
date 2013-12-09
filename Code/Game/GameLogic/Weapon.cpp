#include "Weapon.h"

using namespace GameLogic;

struct Weapon::PrivateData
{
	PrivateData()
	{
		
	}

	~PrivateData()
	{

	}

}myData;

Weapon::Weapon()
{
	myData = new PrivateData();
}


Weapon::~Weapon(void)
{
	delete myData;
}
