#include "LanMenuState.h"

#include "C_obj/C_Player.h"
#include "C_obj/C_StaticObj.h"
#include "C_obj/C_DynamicObj.h"
#include "DllInterfaces/GFXAPI.h"

#include <GameServerAPI.h>

using namespace DanBias::Client;

struct  LanMenuState::myData
{
	myData(){}
	Oyster::Math3D::Float4x4 view;
	Oyster::Math3D::Float4x4 proj; 
	C_Object* object[2]; 
	int modelCount; 
	// UI object
	// game client* 
}privData;

LanMenuState::LanMenuState()
{

}

LanMenuState::~LanMenuState()
{

}

bool LanMenuState::Init(Oyster::Network::NetworkClient* nwClient)
{
	privData = new myData();
	this->nwClient = nwClient;	
	// load models
	LoadModels(L"UImodels.txt");
	InitCamera(Oyster::Math::Float3(0,0,5.4f));

	return true;
}

bool LanMenuState::LoadModels(std::wstring file)
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

	Oyster::Math3D::Float4x4 translate =  Oyster::Math3D::TranslationMatrix(Oyster::Math::Float3(-2,-2,-2));
	modelData.world = modelData.world * translate;

	privData->object[1] = new C_DynamicObj();
	privData->object[1]->Init(modelData);
	return true;
}

bool LanMenuState::InitCamera(Oyster::Math::Float3 startPos)
{
	privData->proj = Oyster::Math3D::ProjectionMatrix_Perspective(Oyster::Math::pi/2,1024.0f/768.0f,.1f,1000);
	//privData->proj = Oyster::Math3D::ProjectionMatrix_Orthographic(1024, 768, 1, 1000);
	Oyster::Graphics::API::SetProjection(privData->proj);

	privData->view = Oyster::Math3D::OrientationMatrix_LookAtDirection(Oyster::Math::Float3(0,0,-1),Oyster::Math::Float3(0,1,0),startPos);
	privData->view = Oyster::Math3D::InverseOrientationMatrix(privData->view);
	return true;
}

GameClientState::ClientState LanMenuState::Update(float deltaTime, InputClass* KeyInput)
{
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
		return ClientState_Lobby;
	}
	// join game
	if( KeyInput->IsKeyPressed(DIK_J)) 
	{
		// game ip
		nwClient->Connect(15151, "194.47.150.56");

		if (!nwClient->IsConnected())
		{
			// failed to connect
			return ClientState_Same;
		}
		return ClientState_Lobby;
	}
	return ClientState_Same;
}

bool LanMenuState::Render()
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

bool LanMenuState::Release()
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

void LanMenuState::Protocol(ProtocolStruct* protocolStruct)
{
	if((PlayerName*)protocolStruct)
		PlayerJoinProtocol((PlayerName*)protocolStruct);
}
void LanMenuState::PlayerJoinProtocol(PlayerName* name)
{

}