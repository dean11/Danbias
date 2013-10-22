#ifndef GAMELUA_h
#define GAMELUA_h

#include <string>
#include <vector>
#include <lua.hpp>

#include "EffectCarrier.h"
#include "Weapon.h"
#include "Player.h"

namespace GameLogic
{
	class GameLUA
	{
	public:
		static bool init();
		static void release();

		static bool loadScript(const ::std::string &file);

		static void executeOnFireScript( Weapon::Type weapon, const Player &owner );
		static void executeOnUpdateScript( Weapon::Type weapon, Object *object, const ::Oyster::Math::Float &deltaTime );
		static void executeOnHitScript( Weapon::Type weapon, EffectCarrier *effect, const ::std::vector<Object*> &targets, const std::vector<Oyster::Math::Float3> &hitPosW );

		static Weapon::Type activeScriptWeapon;
	private:
		static lua_State *luaState;
	};
}

#endif
