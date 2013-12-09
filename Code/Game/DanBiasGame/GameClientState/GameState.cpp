#include "GameState.h"
#include "DllInterfaces/GFXAPI.h"
#include "GameState/C_Player.h"
using namespace DanBias::Client;

struct  GameState::myData
{
	myData(){}
	Oyster::Math3D::Float4x4 view;
	Oyster::Math3D::Float4x4 proj; 
	C_Object* player;
}privData;

GameState::GameState(void)
{
}


GameState::~GameState(void)
{

}
bool GameState::Init()
{
	ModelInitData modelData;

	modelData.world = Oyster::Math3D::Float4x4::identity;
	modelData.visible = true;
	modelData.modelPath = L"worldDummy";
	// load models
	privData = new myData();
	privData->player = new C_Player;
	privData->player->Init(modelData);

	privData->proj = Oyster::Math3D::ProjectionMatrix_Perspective(Oyster::Math::pi/2,1024.0f/768.0f,.1f,1000);
	Oyster::Graphics::API::SetProjection(privData->proj);

	privData->view = Oyster::Math3D::OrientationMatrix_LookAtDirection(Oyster::Math::Float3(0,0,-1),Oyster::Math::Float3(0,1,0),Oyster::Math::Float3(0,0,5.4f));
	privData->view = Oyster::Math3D::InverseOrientationMatrix(privData->view);

	return true;
}
GameClientState::ClientState GameState::Update(float deltaTime, InputClass* KeyInput)
{
	if(KeyInput->IsKeyPressed(DIK_L))
		return ClientState_Lobby;

	// send key input to server. 
	return ClientState_Same;
}
bool GameState::Render()
{

	Oyster::Graphics::API::SetView(privData->view);
	Oyster::Graphics::API::SetProjection( privData->proj);
	Oyster::Graphics::API::NewFrame();
	privData->player->Render();
	Oyster::Graphics::API::EndFrame();
	return true;
}
bool GameState::Release()
{
	privData->player->Release();
	delete privData->player;
	privData->player = NULL;
	delete privData;  
	privData = NULL;
	return true;
}