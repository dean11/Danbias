#include "LobbyState.h"
#include "DllInterfaces/GFXAPI.h"
#include "OysterMath.h"
using namespace DanBias::Client;

struct  myData
{
	Oyster::Math3D::Float4x4 view;
	Oyster::Math3D::Float4x4 proj; 
	Oyster::Graphics::Model::Model *model;
}data;


LobbyState::LobbyState(void)
{
		
}


LobbyState::~LobbyState(void)
{
}
bool LobbyState::Init()
{
	data.model = Oyster::Graphics::API::CreateModel(L"crate");

	Oyster::Math::Float3 dir = Oyster::Math::Float3(0,0,-1);
	Oyster::Math::Float3 up  =Oyster::Math::Float3(0,1,0);
	Oyster::Math::Float3 pos = Oyster::Math::Float3(0, 0, 100);
	data.view = Oyster::Math3D::ViewMatrix_LookAtDirection(dir, up, pos);
	data.proj = Oyster::Math3D::ProjectionMatrix_Orthographic(1024, 768, 1, 10);
	return true;
}
GameClientState::ClientState LobbyState::Update()
{

	//if( startGame) 
	 // return ClientState_Game;
	return ClientState_Same;
}
bool LobbyState::Render()
{
	Oyster::Graphics::API::NewFrame(data.view, data.proj);
	Oyster::Graphics::API::RenderScene(data.model,1);
	Oyster::Graphics::API::EndFrame();
	return true;
}
bool LobbyState::Release()
{
	return true;
}