#ifndef DANBIAS_CLIENT_GAMESTATE_H
#define DANBIAS_CLIENT_GAMESTATE_H
#include "GameClientState.h"
#include "OysterMath.h"
#include <string>
#include "Camera.h"
#include "LevelLoader/LevelLoader.h"
#include "C_obj/C_Player.h"
#include "C_obj/C_DynamicObj.h"
#include "C_obj/C_StaticObj.h"
#include "DynamicArray.h"
namespace DanBias
{
namespace Client
{
class GameState : public GameClientState
{
	enum gameStateState
	{
		gameStateState_loading,
		gameStateState_playing,
		gameStateState_end,
	};
private:

	bool key_forward;
	bool key_backward;
	bool key_strafeRight;
	bool key_strafeLeft;
	bool key_Shoot;
	bool key_Jump;
	Camera* camera;

	int myId;
	float pitch;
	struct myData;
	myData* privData;
	Utility::DynamicMemory::DynamicArray<Utility::DynamicMemory::SmartPointer<C_StaticObj>> staticObjects;
	Utility::DynamicMemory::DynamicArray<Utility::DynamicMemory::SmartPointer<C_Object>> dynamicObjects;
	//Utility::DynamicMemory::DynamicArray<Utility::DynamicMemory::SmartPointer<C_Player>> playObjects;
public:
	GameState(void);
	~GameState(void);
	bool Init(Oyster::Network::NetworkClient* nwClient);
	GameClientState::ClientState Update(float deltaTime, InputClass* KeyInput) override;
	bool LoadModels(std::string mapFile);
	bool LoadModels();
	bool InitCamera(Oyster::Math::Float3 startPos) ;
	void InitiatePlayer(int id, std::wstring modelName, Oyster::Math::Float4x4 world);
	gameStateState LoadGame();
	void readKeyInput(InputClass* KeyInput);
	bool Render()override;
	bool Release()override;

	void Protocol(ProtocolStruct* pos)override;
	void Protocol(PlayerPos* pos);
	void Protocol(ObjPos* pos);
	void Protocol( NewObj* pos );
	void Protocol(RemoveObj* obj);
	//void Protocol(LightPos pos);
};
};
};
#endif