#ifndef DANBIAS_CLIENT_GAMESTATE_H
#define DANBIAS_CLIENT_GAMESTATE_H
#include "GameClientState.h"
#include "OysterMath.h"
#include <string>
#include "Camera.h"
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

	struct myData;
	myData* privData;
public:
	GameState(void);
	~GameState(void);
	bool Init(Oyster::Network::NetworkClient* nwClient);
	GameClientState::ClientState Update(float deltaTime, InputClass* KeyInput) override;
	bool LoadModels(std::wstring mapFile) ;
	bool InitCamera(Oyster::Math::Float3 startPos) ;
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