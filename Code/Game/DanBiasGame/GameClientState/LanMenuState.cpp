#include "LanMenuState.h"

#include "C_obj/C_Player.h"
#include "C_obj/C_StaticObj.h"
#include "C_obj/C_DynamicObj.h"
#include "DllInterfaces/GFXAPI.h"

#include "LobbyState.h"
#include "GameState.h"
#include "../Network/NetworkAPI/NetworkClient.h"

#include "EventHandler\EventHandler.h"
#include "Buttons\ButtonRectangle.h"

#include <GameServerAPI.h>
#include <string>

using namespace ::DanBias::Client;
using namespace ::Oyster;
using namespace ::Oyster::Network;
using namespace ::Oyster::Event;
using namespace ::Oyster::Math3D;

struct  LanMenuState::MyData
{
	MyData(){}

	GameClientState::ClientState nextState;
	NetworkClient *nwClient;
	Graphics::API::Texture background;
	EventButtonCollection button;
	::std::wstring connectIP;
	unsigned short connectPort;
} privData;

void OnButtonInteract_Connect( Oyster::Event::ButtonEvent<GameClientState*>& e );

LanMenuState::LanMenuState() {}

LanMenuState::~LanMenuState()
{
	if( this->privData )
		this->Release();
}

bool LanMenuState::Init(Network::NetworkClient* nwClient)
{
	this->privData = new MyData();

	this->privData->nextState = GameClientState::ClientState_Same;
	this->privData->nwClient = nwClient;

	this->privData->background = Graphics::API::CreateTexture( L"grass_md.png" );

	// create buttons
	ButtonRectangle<GameClientState*> *button;
	
	button = new ButtonRectangle<GameClientState*>( L"earth_md.png", OnButtonInteract_Connect, this, 0.5f, 0.2f, 0.3f, 0.1f, true );
	this->privData->button.AddButton( button );

	// bind button collection to the singleton eventhandler
	EventHandler::Instance().AddCollection( &this->privData->button );

	this->privData->connectIP = L"127.0.0.1";
	this->privData->connectPort = 15151;

	return true;
}

GameClientState::ClientState LanMenuState::Update(float deltaTime, InputClass* KeyInput)
{
	MouseInput mouseState;
	{
		mouseState.x = KeyInput->GetPitch();
		mouseState.y = KeyInput->GetYaw();
		mouseState.mouseButtonPressed = KeyInput->IsMousePressed();
	}

	EventHandler::Instance().Update( mouseState );

	return this->privData->nextState;
}

bool LanMenuState::Render( )
{
	Graphics::API::NewFrame();

	Graphics::API::StartGuiRender();

	Graphics::API::RenderGuiElement( this->privData->background, Float2(0.5f), Float2(1.0f) );
	this->privData->button.Render();

	Graphics::API::EndFrame();
	return true;
}

bool LanMenuState::Release()
{
	privData = NULL;
	return true;
}

void LanMenuState::ChangeState( ClientState next )
{
	switch( next )
	{
	case GameClientState::ClientState_Lobby:
		// attempt to connect to lobby
		if( !this->privData->nwClient->Connect(this->privData->connectPort, this->privData->connectIP) )
			return;
		break;
	default: break;
	}

	this->privData->nextState = next;
}

void OnButtonInteract_Connect( Oyster::Event::ButtonEvent<GameClientState*>& e )
{
	switch( e.state )
	{
	case ButtonState_Released:
		e.owner->ChangeState( GameClientState::ClientState_LobbyCreate );
		break;
	default: break;
	}
}