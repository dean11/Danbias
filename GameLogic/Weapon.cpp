#include <cstdlib>

#include "Weapon.h"

using namespace GameLogic;

const Weapon Weapon::weapons[Weapon::Count] =
{
	{"../Content/Scripts/weapon_test.lua", "test_OnFire", "test_OnUpdate", "test_OnHit"}, // WeaponType::Test
	{"../Content/Scripts/weapon_shotgun.lua", "shotgun_OnFire", "shotgun_OnUpdate", "shotgun_OnHit"}, // WeaponType::Shotgun
	{"../Content/Scripts/PowerUpManager.lua", "PowerUp_OnFire", "PowerUp_OnUpdate", "PowerUp_OnHit"} // WeaponType::PowerUpManager
};
