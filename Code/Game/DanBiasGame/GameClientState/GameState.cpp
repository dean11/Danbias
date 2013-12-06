#include "GameState.h"
#include "DllInterfaces/GFXAPI.h"
using namespace DanBias::Client;
GameState::GameState(void)
{
}


GameState::~GameState(void)
{
}
bool GameState::Init()
{
	return true;
}
GameClientState::ClientState GameState::Update()
{
	return ClientState_Same;
}
bool GameState::Render()
{
	Oyster::Graphics::API::NewFrame(Oyster::Math3D::Float4x4::null, Oyster::Math3D::Float4x4::null);

	Oyster::Graphics::API::EndFrame();
	return true;
}
bool GameState::Release()
{
	return true;
}