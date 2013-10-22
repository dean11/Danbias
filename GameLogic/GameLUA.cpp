#include "GameLUA.h"
#include "Object.h"
#include "OysterMath.h"
#include "EffectCarrier.h"
#include "Weapon.h"
#include "Player.h"
#include "Session.h"
#include "Bullet.h"

using namespace ::Oyster::Math;
using namespace ::GameLogic;
using ::std::vector;
using ::std::string;

lua_State *GameLUA::luaState = NULL;
Weapon::Type GameLUA::activeScriptWeapon = Weapon::Undefined;

namespace GameInterface { void registerBindings(lua_State *lua, const char *nameSpace); }

Float3 lua_tofloat3(lua_State *lua, int index);
void lua_pushfloat3(lua_State *lua, const Float3 &value);
template<typename T>
void lua_pushstdvector(lua_State *lua, const std::vector<T *> &values);
void lua_pushstdvector(lua_State *lua, const std::vector<Float3> &values);

bool GameLUA::init()
{
	if (luaState == NULL)
	{
		luaState = luaL_newstate();
		luaL_openlibs(luaState);
		
		GameInterface::registerBindings(luaState, "engine");

		// Load all weapon scripts.
		for(int i = 0; i < Weapon::Count; i++)
		{
			GameLUA::loadScript(Weapon::weapons[i].scriptFile);
		}
	}

	return true;
}

void GameLUA::release()
{
	if (luaState != NULL)
	{
		lua_close(luaState);
		luaState = NULL;
	}
}

bool GameLUA::loadScript(const std::string &file)
{
	if (luaL_loadfile(luaState, file.c_str()) != LUA_OK)
	{
		printf("Failed to load script '%s'\n%s\n", file.c_str(), lua_tostring(luaState, -1));
		return false;
	}

	if (lua_pcall(luaState, 0, 0, 0) != LUA_OK)
	{
		printf("Failed to run script '%s'\n%s\n", file.c_str(), lua_tostring(luaState, -1));
		return false;
	}

	return true;
}

void GameLUA::executeOnFireScript( Weapon::Type weapon, const Player &owner )
{
	lua_getglobal( luaState, Weapon::weapons[weapon].onFireFunction );

	int numArguments = 1;
	lua_pushlightuserdata(luaState, (void *)&owner);

	int numReturnValues = 0;

	activeScriptWeapon = weapon;

	if (lua_pcall(luaState, numArguments, numReturnValues, 0) != LUA_OK)
		printf("Failed to run scriptfunction '%s'\n%s\n", Weapon::weapons[weapon].onFireFunction, lua_tostring(luaState, -1));

	activeScriptWeapon = Weapon::Undefined;
}

void GameLUA::executeOnUpdateScript( Weapon::Type weapon, Object *object, const Float &deltaTime )
{
	lua_getglobal( luaState, Weapon::weapons[weapon].onUpdateFunction );

	int numArguments = 2;
	lua_pushlightuserdata(luaState, object);
	lua_pushnumber(luaState, deltaTime);

	int numReturnValues = 0;

	activeScriptWeapon = weapon;

	if (lua_pcall(luaState, numArguments, numReturnValues, 0) != LUA_OK)
		printf("Failed to run scriptfunction '%s'\n%s\n", Weapon::weapons[weapon].onUpdateFunction, lua_tostring(luaState, -1));

	activeScriptWeapon = Weapon::Undefined;
}

void GameLUA::executeOnHitScript( Weapon::Type weapon, EffectCarrier *effect, const ::std::vector<Object*> &targets, const vector<Float3> &hitPosW )
{
	lua_getglobal( luaState, Weapon::weapons[weapon].onHitFunction );
	int numArguments = 4;
	lua_pushlightuserdata(luaState, effect);
	lua_pushlightuserdata(luaState, effect->accessOwner());
	lua_pushstdvector(luaState, targets);
	lua_pushstdvector(luaState, hitPosW);

	int numReturnValues = 0;

	activeScriptWeapon = weapon;

	if (lua_pcall(luaState, numArguments, numReturnValues, 0) != LUA_OK)
		printf("Failed to run scriptfunction '%s'\n%s\n", Weapon::weapons[weapon].onHitFunction, lua_tostring(luaState, -1));

	activeScriptWeapon = Weapon::Undefined;
}


Float3 lua_tofloat3(lua_State *lua, int index)
{
	Float3 value;

	lua_rawgeti(lua, index, 1);
	value.x = (float)lua_tonumber(lua, -1);
	lua_pop(lua, 1);

	lua_rawgeti(lua, index, 2);
	value.y = (float)lua_tonumber(lua, -1);
	lua_pop(lua, 1);

	lua_rawgeti(lua, index, 3);
	value.z = (float)lua_tonumber(lua, -1);
	lua_pop(lua, 1);

	return value;
}

void lua_pushfloat3(lua_State *lua, const Float3 &value)
{
	lua_createtable(lua, 3, 0);

	lua_pushnumber(lua, value.x);
	lua_rawseti(lua, -2, 1);
	lua_pushnumber(lua, value.y);
	lua_rawseti(lua, -2, 2);
	lua_pushnumber(lua, value.z);
	lua_rawseti(lua, -2, 3);
}

template<typename T>
void lua_pushstdvector(lua_State *lua, const std::vector<T *> &values)
{
	lua_createtable(lua, (int)values.size(), 0);

	for(int i = 0; i < (int)values.size(); i++)
	{
		lua_pushlightuserdata(lua, values[i]);
		lua_rawseti(lua, -2, i + 1);
	}
}

void lua_pushstdvector(lua_State *lua, const std::vector<Float3> &values)
{
	lua_createtable(lua, (int)values.size(), 0);

	for(int i = 0; i < (int)values.size(); i++)
	{
		lua_pushfloat3(lua, values[i]);
		lua_rawseti(lua, -2, i + 1);
	}
}


namespace GameInterface
{
	int applyDamage( lua_State *lua )
	{
		Object *object = (Object*)lua_touserdata( lua, -3 );
		unsigned int magnitude = lua_tounsigned( lua, -2 );
		Object *attacker = (Object*)lua_touserdata( lua, -1 );

		object->applyDamage( magnitude, *attacker );
		return 0;
	}

	int applyHeal( lua_State *lua )
	{
		Object *object = (Object*)lua_touserdata( lua, -2 );
		unsigned int magnitude = lua_tounsigned( lua, -1 );

		object->applyHeal( magnitude );
		return 0;
	}

	int applyForce( lua_State *lua )
	{
		Object *object = (Object*) lua_touserdata( lua, -3 );
		Float3 force = lua_tofloat3(lua, -2);
		Float3 position = lua_tofloat3(lua, -1);

		object->applyForceW( force, position );

		return 0;
	}

	int createBullet(lua_State *lua)
	{
		Weapon::Type weapon = GameLUA::activeScriptWeapon;

		if (weapon == Weapon::Undefined)
			return 0;

		Player *owner = (Player *)lua_touserdata(lua, -6);
		Float4 position = Float4(lua_tofloat3(lua, -5), 1.0f);
		Float4 direction = Float4(lua_tofloat3(lua, -4), 0.0f);
		float speed = (float)lua_tonumber(lua, -3);
		float lifeTime = (float)lua_tonumber(lua, -2);
		std::string modelRef = lua_tostring(lua, -1);

		// transform local to world
		transformVector( position, position, owner->getOrientation() );
		transformVector( direction, direction, owner->getOrientation() );

		// we want projectiles to move relative to the Players movement at the time
		direction.xyz *= speed;
		//direction.xyz += owner->getMovement();
		speed = direction.xyz.length();
		direction.xyz /= speed;

		EffectCarrier *bullet = EffectCarrier::createBullet( owner, weapon, position.xyz, direction.xyz, speed, lifeTime, modelRef );
		owner->getSession()->addEffect(*bullet);

		return 0;
	}

	int getBulletDirection(lua_State *lua)
	{
		Bullet *bullet = (Bullet *)lua_touserdata(lua, -1);

		lua_pushfloat3(lua, bullet->getDirection());

		return 1;
	}

	int setWeaponCooldown(lua_State *lua)
	{
		Player *player = (Player *)lua_touserdata(lua, -2);
		float cooldown = (float)lua_tonumber(lua, -1);

		player->setPrimaryWeaponCooldown(cooldown);

		return 0;
	}

	struct FunctionBinding
	{
		const char *name;
		lua_CFunction function;
	};

	static const FunctionBinding functionBindings[] =
	{
		{"applyDamage", applyDamage},
		{"applyHeal", applyHeal},
		{"applyForce", applyForce},
		{"createBullet", createBullet},
		{"getBulletDirection", getBulletDirection},
		{"setWeaponCooldown", setWeaponCooldown},
		{NULL, NULL}
	};

	void registerBindings(lua_State *lua, const char *nameSpace)
	{
		int i = 0;

		lua_newtable(lua);

		while(functionBindings[i].name != NULL)
		{
			lua_pushstring(lua, functionBindings[i].name);
			lua_pushcfunction(lua, functionBindings[i].function);

			lua_rawset(lua, -3);

			i++;
		}

		lua_setglobal(lua, nameSpace);
	}
}