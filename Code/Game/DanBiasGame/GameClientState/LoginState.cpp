#include "LoginState.h"
#include "DllInterfaces/GFXAPI.h"
#include "OysterMath.h"
#include "C_obj/C_Player.h"
#include "C_obj/C_StaticObj.h"
#include "C_obj/C_DynamicObj.h"
#include <GameServerAPI.h>

using namespace DanBias::Client;

struct  LoginState::myData
{
	myData(){}
	Oyster::Math3D::Float4x4 view;
	Oyster::Math3D::Float4x4 proj; 
	C_Object* object[2]; 
	int modelCount; 
	// UI object
	// game client* 
}privData;

LoginState::LoginState(void)
{

}

LoginState::~LoginState(void)
{

}

bool LoginState::Init(Oyster::Network::NetworkClient* nwClient)
{
	privData = new myData();
	this->nwClient = nwClient;	
	// load models
	LoadModels(L"UImodels.txt");
	InitCamera(Oyster::Math::Float3(0,0,5.4f));
	return true;
}
bool LoginState::LoadModels(std::wstring file)
{
	Oyster::Graphics::Definitions::Pointlight plight;
	plight.Pos = Oyster::Math::Float3(0,0,5);
	plight.Color = Oyster::Math::Float3(1,1,1);
	plight.Radius = 100;
	plight.Bright = 1;
	Oyster::Graphics::API::AddLight(plight);
	// open file
	// read file 
	// init models
	privData->modelCount = 2;

	ModelInitData modelData;

	modelData.rotation = Oyster::Math::Quaternion::identity;
	modelData.scale =  Oyster::Math::Float3(1,1,1);
	modelData.visible = true;
	modelData.modelPath = L"identityPlane.dan";
	

	modelData.position = Oyster::Math::Float3(2,2,2);
	privData->object[0] = new C_StaticObj();
	privData->object[0]->Init(modelData);

	modelData.position = Oyster::Math::Float3(0,0,-2);
	privData->object[1] = new C_StaticObj();
	privData->object[1]->Init(modelData);
	return true;
}

bool LoginState::InitCamera(Oyster::Math::Float3 startPos)
{
	privData->proj = Oyster::Math3D::ProjectionMatrix_Perspective(Oyster::Math::pi/2,1024.0f/768.0f,.1f,1000);
	//privData->proj = Oyster::Math3D::ProjectionMatrix_Orthographic(1024, 768, 1, 1000);
	Oyster::Graphics::API::SetProjection(privData->proj);

	privData->view = Oyster::Math3D::OrientationMatrix_LookAtDirection(Oyster::Math::Float3(0,0,-1),Oyster::Math::Float3(0,1,0),startPos);
	privData->view = Oyster::Math3D::InverseOrientationMatrix(privData->view);
	return true;
}
GameClientState::ClientState LoginState::Update(float deltaTime, InputClass* KeyInput)
{
	// picking 
	// mouse events
	// different menus
	// play sounds
	// update animation
	// send data to server
	// check data from server

	// create game
	if( KeyInput->IsKeyPressed(DIK_C)) 
	{
		DanBias::GameServerAPI::ServerInitDesc desc; 

		DanBias::GameServerAPI::ServerInitiate(desc);
		DanBias::GameServerAPI::ServerStart();
		// my ip
		nwClient->Connect(15151, "127.0.0.1");

		if (!nwClient->IsConnected())
		{
			// failed to connect
			return ClientState_Same;
		}
		return ClientState_LobbyCreated;
	}
	// join game
	if( KeyInput->IsKeyPressed(DIK_J)) 
	{
		// game ip
		nwClient->Connect(15151, "127.0.0.1");

		if (!nwClient->IsConnected())
		{
			// failed to connect
			return ClientState_Same;
		}
		return ClientState_Lobby;
	}
	return ClientState_Same;
}
bool LoginState::Render()
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
bool LoginState::Release()
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
void LoginState::Protocol(ProtocolStruct* protocol)
{
	if((PlayerName*)protocol)
		PlayerJoinProtocol((PlayerName*)protocol);

}
void LoginState::PlayerJoinProtocol(PlayerName* name)
{

}