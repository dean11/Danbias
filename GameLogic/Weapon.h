#ifndef WEAPON_H
#define WEAPON_H

namespace GameLogic
{
	class Weapon
	{
	public:
		enum Type
		{
			Test = 0,
			Shotgun,
			Powerup,
			Count,
			Undefined
		};

		const char *scriptFile;
		const char *onFireFunction;
		const char *onUpdateFunction;
		const char *onHitFunction;

	public:
		static const Weapon weapons[Weapon::Count];
	};
};

#endif
