#include "MainState.h"
#include "DllInterfaces/GFXAPI.h"
#include "OysterMath.h"
#include "C_obj/C_Player.h"
#include "C_obj/C_StaticObj.h"
#include "C_obj/C_DynamicObj.h"
#include <GameServerAPI.h>
#include "NetworkClient.h"

using namespace ::DanBias::Client;
using namespace ::Oyster::Math3D;
using namespace ::Oyster::Network;
using namespace ::Utility::DynamicMemory;

struct MainState::MyData
{
	MyData() {}

	Float4x4 view;
	Float4x4 proj; 
	
	UniquePointer<C_Object> object[2]; 
	int modelCount; 
	NetworkClient *nwClient;
};

MainState::MainState(void) {}

MainState::~MainState(void) {}

bool MainState::Init( NetworkClient* nwClient )
{
	this->privData = new MyData();

	// load models
	LoadModels(L"UImodels.txt");
	InitCamera( Float3(0.0f, 0.0f, 5.4f) );
	return true;
}

bool MainState::LoadModels(std::wstring file)
{
	Oyster::Graphics::Definitions::Pointlight plight;
	plight.Pos = Float3(0,0,5);
	plight.Color = Float3(1,1,1);
	plight.Radius = 100;
	plight.Bright = 1;
	Oyster::Graphics::API::AddLight(plight);
	// open file
	// read file 
	// init models
	privData->modelCount = 2;

	ModelInitData modelData;

	modelData.rotation = Quaternion::identity;
	modelData.scale =  Float3(1,1,1);
	modelData.visible = true;
	modelData.modelPath = L"box.dan";
	

	modelData.position = Float3(2,2,2);
	privData->object[0] = new C_StaticObj();
	privData->object[0]->Init(modelData);

	modelData.position = Float3(-2,0,-2);
	privData->object[1] = new C_StaticObj();
	privData->object[1]->Init(modelData);
	return true;
}

bool MainState::InitCamera(Float3 startPos)
{
	privData->proj = ProjectionMatrix_Perspective(pi/2,1024.0f/768.0f,.1f,1000);
	//privData->proj = ProjectionMatrix_Orthographic(1024, 768, 1, 1000);
	Oyster::Graphics::API::SetProjection(privData->proj);

	privData->view = OrientationMatrix_LookAtDirection(Float3(0,0,-1),Float3(0,1,0),startPos);
	privData->view = InverseOrientationMatrix(privData->view);
	return true;
}

GameClientState::ClientState MainState::Update(float deltaTime, InputClass* KeyInput)
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
		this->privData->nwClient->Connect(15152, "127.0.0.1");

		if (!this->privData->nwClient->IsConnected())
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
		this->privData->nwClient->Connect(15152, "127.0.0.1");
		//nwClient->Connect(15152, "83.254.217.248");

		if (!this->privData->nwClient->IsConnected())
		{
			// failed to connect
			return ClientState_Same;
		}
		return ClientState_Lobby;
	}
	return ClientState_Same;
}

bool MainState::Render()
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

bool MainState::Release()
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

