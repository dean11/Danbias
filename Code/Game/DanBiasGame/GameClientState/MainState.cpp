#include "MainState.h"
#include "DllInterfaces/GFXAPI.h"
#include "OysterMath.h"
#include "C_obj/C_Player.h"
#include "C_obj/C_StaticObj.h"
#include "C_obj/C_DynamicObj.h"
#include <GameServerAPI.h>
#include "NetworkClient.h"

#include "EventHandler\EventHandler.h"
#include "Buttons\ButtonRectangle.h"

using namespace ::DanBias::Client;
using namespace ::Oyster;
using namespace ::Oyster::Math3D;
using namespace ::Oyster::Network;
using namespace ::Oyster::Event;
using namespace ::Utility::DynamicMemory;
using namespace ::Utility::StaticArray;

struct MainState::MyData
{
	MyData() {}

	Float4x4 view;
	Float4x4 proj; 
	
	UniquePointer<C_Object> object[2]; 
	NetworkClient *nwClient;

	EventButtonCollection button;
};

void OnButtonInteract( Oyster::Event::ButtonEvent<GameClientState*>& e );

MainState::MainState(void) {}

MainState::~MainState(void) {}

bool MainState::Init( NetworkClient* nwClient )
{
	this->privData = new MyData();

	// create buttons
	ButtonRectangle<GameClientState*> *button = new ButtonRectangle<GameClientState*>( L"earth_md.png", OnButtonInteract, this, 0.5f, 0.5f, 0.1f, 0.1f, true );
	this->privData->button.AddButton( button );

	// bind button collection to the singleton eventhandler
	EventHandler::Instance().AddCollection( &this->privData->button );

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
	Graphics::API::SetView(privData->view);
	Graphics::API::SetProjection( privData->proj);

	Graphics::API::NewFrame();
	// render objects
	//for (int i = 0; i < NumElementsOf(privData->object); i++)
	//{
	//	privData->object[i]->Render();
	//}

	Graphics::API::StartGuiRender();
	this->privData->button.Render();


	// render effects

	// render lights

	Graphics::API::EndFrame();
	return true;
}

bool MainState::Release()
{
	for (int i = 0; i < NumElementsOf(privData->object); i++)
	{
		privData->object[i] = NULL;
	}

	privData = NULL;
	// button collection will be autoreleased from EventHandler

	return true;
}

/// button actions
void OnButtonInteract( Oyster::Event::ButtonEvent<GameClientState*>& e )
{

}
