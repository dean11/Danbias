#include "GameState.h"
#include "DllInterfaces/GFXAPI.h"
using namespace DanBias::Client;

struct  GameState::myData
{
	myData(){}
	Oyster::Math3D::Float4x4 view;
	Oyster::Math3D::Float4x4 proj; 
	Oyster::Graphics::Model::Model *model;
}privData;

GameState::GameState(void)
{
}


GameState::~GameState(void)
{
}
bool GameState::Init()
{
	// load models
	privData = new myData();
	privData->model = Oyster::Graphics::API::CreateModel(L"crate");

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
	//Oyster::Graphics::API::RenderModel(*(privData->model));
	Oyster::Graphics::API::EndFrame();
	return true;
}
bool GameState::Release()
{
	return true;
}