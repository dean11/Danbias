#ifndef NET_UPD_STRUCTS_H
#define NET_UPD_STRUCTS_H
#include "NetworkIncludes.h"
namespace Network
{
	struct EffectData
	{
		int identifier;
		Float3 head;
		Float3 tail;
	};
	struct ServerToClientUpdateData
	{
		int pid;
		Oyster::Math::Float4x4 position;
		float dirVecLen;
		int hp;
		int shield;
		long updateCount;
		ServerToClientUpdateData()
		{
			pid=0;
			updateCount=0;
			hp=0;
			shield=0;
		}
	};
	const int SERVER_PLAYER_DATA_SIZE = 84;
	struct ClientToServerUpdateData
	{
		__int8 pid;
		//Oyster::Math::Float4x4 position;
		__int8 forward;
		__int8 roll;
		__int8 straferight;
		__int8 strafeup;
		bool firePrim;
		bool fireSecond;
		bool fireSpecial;
		long updateCount;
		bool braking;
		float TurnHor;
		float TurnVer;
		ClientToServerUpdateData()
		{
			pid=0;
			forward=0;
			roll=0;
			straferight=0;
			strafeup=0;
			firePrim=false;
			fireSecond=false;
			fireSpecial=false;
			updateCount=0;
			braking=false;
			TurnHor= 0.0f;
			TurnVer= 0.0f;
		}
	};
	const int CLIENT_PLAYER_DATA_SIZE = sizeof(ClientToServerUpdateData);
}
#endif