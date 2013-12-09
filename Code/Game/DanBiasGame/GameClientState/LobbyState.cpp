#include "LobbyState.h"
#include "DllInterfaces/GFXAPI.h"
#include "OysterMath.h"
using namespace DanBias::Client;

struct  LobbyState::myData
{
	myData(){}
	Oyster::Math3D::Float4x4 view;
	Oyster::Math3D::Float4x4 proj; 
	Oyster::Graphics::Model::Model *model;
}privData;


LobbyState::LobbyState(void)
{
		
}


LobbyState::~LobbyState(void)
{
	 Oyster::Graphics::API::DeleteModel(privData->model);
	 Oyster::Graphics::API::Clean();
}
bool LobbyState::Init()
{
	// load models
	privData = new myData();
	privData->model = Oyster::Graphics::API::CreateModel(L"crate");

	privData->proj = Oyster::Math3D::ProjectionMatrix_Perspective(Oyster::Math::pi/2,1024.0f/768.0f,.1f,1000);
	//privData->proj = Oyster::Math3D::ProjectionMatrix_Orthographic(1024, 768, 1, 1000);
	Oyster::Graphics::API::SetProjection(privData->proj);

	privData->view = Oyster::Math3D::OrientationMatrix_LookAtDirection(Oyster::Math::Float3(0,0,-1),Oyster::Math::Float3(0,1,0),Oyster::Math::Float3(0,0,5.4f));
	privData->view = Oyster::Math3D::InverseOrientationMatrix(privData->view);



	return true;
}
GameClientState::ClientState LobbyState::Update(float deltaTime, InputClass* KeyInput)
{

	if( KeyInput->IsKeyPressed(DIK_Q)) 
	  return ClientState_Game;
	return ClientState_Same;
}
bool LobbyState::Render()
{

	Oyster::Graphics::API::SetView(privData->view);
	Oyster::Graphics::API::SetProjection( privData->proj);
	Oyster::Graphics::API::NewFrame();
	Oyster::Graphics::API::RenderModel(*(privData->model));
	Oyster::Graphics::API::EndFrame();
	return true;
}
bool LobbyState::Release()
{
	return true;
}