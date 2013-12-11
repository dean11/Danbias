#include "GameState.h"
#include "DllInterfaces/GFXAPI.h"
#include "Obj/C_Player.h"
#include "Obj/C_DynamicObj.h"

using namespace DanBias::Client;

struct  GameState::myData
{
	myData(){}
	Oyster::Math3D::Float4x4 view;
	Oyster::Math3D::Float4x4 proj; 
	C_Object* object[3];
	int modelCount;
	gameStateState state;
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
	privData->state = gameStateState_loading;
	privData->state = LoadGame();
	return true;
}
GameState::gameStateState GameState::LoadGame()
{
	LoadModels(L"map");
	InitCamera(Oyster::Math::Float3(0,0,5.4f));
	return gameStateState_playing;
}
bool GameState::LoadModels(std::wstring mapFile)
{
	// open file
	// read file 
	// init models
	privData->modelCount = 2;

	ModelInitData modelData;

	modelData.world = Oyster::Math3D::Float4x4::identity;
	modelData.visible = true;
	modelData.modelPath = L"worldDummy";
	// load models
	privData->object[0] = new C_Player();
	privData->object[0]->Init(modelData);

	Oyster::Math3D::Float4x4 translate =  Oyster::Math3D::TranslationMatrix(Oyster::Math::Float3(2,2,2));
	modelData.world = modelData.world * translate;
	modelData.modelPath = L"crate";

	privData->object[1] = new C_DynamicObj();
	privData->object[1]->Init(modelData);
	return true;
}
bool GameState::InitCamera(Oyster::Math::Float3 startPos)
{
	privData->proj = Oyster::Math3D::ProjectionMatrix_Perspective(Oyster::Math::pi/2,1024.0f/768.0f,.1f,1000);
	//privData->proj = Oyster::Math3D::ProjectionMatrix_Orthographic(1024, 768, 1, 1000);
	Oyster::Graphics::API::SetProjection(privData->proj);

	privData->view = Oyster::Math3D::OrientationMatrix_LookAtDirection(Oyster::Math::Float3(0,0,-1),Oyster::Math::Float3(0,1,0),startPos);
	privData->view = Oyster::Math3D::InverseOrientationMatrix(privData->view);
	return true;
}

GameClientState::ClientState GameState::Update(float deltaTime, InputClass* KeyInput)
{
	switch (privData->state)
	{
	case gameStateState_loading:
		// load map
		// wait for all players
		LoadGame();
		privData->state = gameStateState_playing;
		break;
	case gameStateState_playing:
		// read server data
		// update objects
		if(KeyInput->IsKeyPressed(DIK_L))
			privData->state = GameState::gameStateState_end;
		break;
	case gameStateState_end:
		return ClientState_Lobby;
		break;
	default:
		break;
	}
	
	// send key input to server. 
	return ClientState_Same;
}
bool GameState::Render()
{
	Oyster::Graphics::API::SetView(privData->view);
	Oyster::Graphics::API::SetProjection(privData->proj);
	Oyster::Graphics::API::NewFrame();
	for (int i = 0; i < privData->modelCount; i++)
	{
		privData->object[i]->Render();
	}
	Oyster::Graphics::API::EndFrame();
	return true;
}
bool GameState::Release()
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