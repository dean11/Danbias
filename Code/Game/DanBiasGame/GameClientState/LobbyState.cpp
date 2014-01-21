#include "LobbyState.h"
#include "DllInterfaces/GFXAPI.h"
#include "OysterMath.h"
#include "C_obj/C_Player.h"
#include "C_obj/C_StaticObj.h"
#include "C_obj/C_DynamicObj.h"

using namespace DanBias::Client;

struct  LobbyState::myData
{
	myData(){}
	Oyster::Math3D::Float4x4 view;
	Oyster::Math3D::Float4x4 proj; 
	C_Object* object[2]; 
	int modelCount; 
	// UI object
	// game client* 
}privData;

LobbyState::LobbyState(void)
{
		
}

LobbyState::~LobbyState(void)
{
	
}

bool LobbyState::Init(Oyster::Network::NetworkClient* nwClient)
{
	privData = new myData();
	
	// load models
	LoadModels(L"UImodels.txt");
	InitCamera(Oyster::Math::Float3(0,0,5.4f));
	return true;
}
bool LobbyState::LoadModels(std::wstring file)
{
	Oyster::Graphics::Definitions::Pointlight plight;
	plight.Pos = Oyster::Math::Float3(-2,3,0);
	plight.Color = Oyster::Math::Float3(0,1,0);
	plight.Radius = 10;
	plight.Bright = 1;
	Oyster::Graphics::API::AddLight(plight);
	// open file
	// read file 
	// init models
	privData->modelCount = 2;

	ModelInitData modelData;

	modelData.world = Oyster::Math3D::Float4x4::identity;
	modelData.visible = true;
	modelData.modelPath = L"..\\Content\\Models\\box_2.dan";
	// load models
	privData->object[0] = new C_StaticObj();
	privData->object[0]->Init(modelData);

	Oyster::Math3D::Float4x4 translate =  Oyster::Math3D::TranslationMatrix(Oyster::Math::Float3(2,2,2));
	modelData.world = modelData.world * translate;

	privData->object[1] = new C_DynamicObj();
	privData->object[1]->Init(modelData);
	return true;
}

bool LobbyState::InitCamera(Oyster::Math::Float3 startPos)
{
	privData->proj = Oyster::Math3D::ProjectionMatrix_Perspective(Oyster::Math::pi/2,1024.0f/768.0f,.1f,1000);
	//privData->proj = Oyster::Math3D::ProjectionMatrix_Orthographic(1024, 768, 1, 1000);
	Oyster::Graphics::API::SetProjection(privData->proj);

	privData->view = Oyster::Math3D::OrientationMatrix_LookAtDirection(Oyster::Math::Float3(0,0,-1),Oyster::Math::Float3(0,1,0),startPos);
	privData->view = Oyster::Math3D::InverseOrientationMatrix(privData->view);
	return true;
}
GameClientState::ClientState LobbyState::Update(float deltaTime, InputClass* KeyInput)
{
	// picking 
	// mouse events
	// different menus
	// play sounds
	// update animation
	// send data to server
	// check data from server

	if( KeyInput->IsKeyPressed(DIK_G)) 
	  return ClientState_Game;
	return ClientState_Same;
}
bool LobbyState::Render()
{

	Oyster::Graphics::API::SetView(privData->view);
	Oyster::Graphics::API::SetProjection( privData->proj);


	Oyster::Graphics::API::NewFrame();
	// render objects
	for (int i = 0; i < privData->modelCount; i++)
	{
		privData->object[i]->Render();
	}

	// render effects

	// render lights

	Oyster::Graphics::API::EndFrame();
	return true;
}
bool LobbyState::Release()
{
	for (int i = 0; i < privData->modelCount; i++)
	{
		privData->object[i]->Release();
		delete privData->object[i];
		privData->object[i] = NULL;
	}

	delete privData;  
	privData = NULL;
	return true;
}
void LobbyState::Protocol(ProtocolStruct* protocol)
{
	if((PlayerName*)protocol)
		PlayerJoinProtocol((PlayerName*)protocol);
	
}
void LobbyState::PlayerJoinProtocol(PlayerName* name)
{

}