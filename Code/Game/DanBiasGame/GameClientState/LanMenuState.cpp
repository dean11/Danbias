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
#include "Buttons\TextField.h"

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
	EventButtonCollection guiElements;

	TextField<LanMenuState*> *connectIP;
	unsigned short connectPort;
} privData;

void OnButtonInteract_Connect( Oyster::Event::ButtonEvent<LanMenuState*>& e );
void OnButtonInteract_Exit( Oyster::Event::ButtonEvent<LanMenuState*>& e );

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

	// create guiElements
	this->privData->connectIP = new TextField<LanMenuState*>( L"earth_md.png", Float3(1.0f), this, Float3(0.5f, 0.3f, 0.5f), Float2(0.8f, 0.09f), ResizeAspectRatio_None );
	this->privData->connectIP->ReserveLines( 1 );
	this->privData->connectIP->AppendText( L"127.0.0.1" );
	this->privData->connectIP->SetFontHeight( 0.08f );
	this->privData->connectIP->SetLineSpacing( 0.005f );
	this->privData->connectIP->SetTopAligned();
	
	this->privData->guiElements.AddButton( this->privData->connectIP );

	ButtonRectangle<LanMenuState*> *guiElements;
	guiElements = new ButtonRectangle<LanMenuState*>( L"earth_md.png", L"Connect", Float3(1.0f), OnButtonInteract_Connect, this, Float3(0.5f, 0.4f, 0.5f), Float2(0.3f, 0.05f), ResizeAspectRatio_None );
	this->privData->guiElements.AddButton( guiElements );

	guiElements = new ButtonRectangle<LanMenuState*>( L"earth_md.png", L"Exit", Float3(1.0f), OnButtonInteract_Exit, this, Float3(0.5f, 0.5f, 0.5f), Float2(0.3f, 0.05f), ResizeAspectRatio_None );
	this->privData->guiElements.AddButton( guiElements );

	// bind guiElements collection to the singleton eventhandler
	EventHandler::Instance().AddCollection( &this->privData->guiElements );

	this->privData->connectPort = 15151;

	return true;
}

GameClientState::ClientState LanMenuState::Update(float deltaTime, InputClass* KeyInput)
{
	MouseInput mouseState;
	{
		KeyInput->GetMousePos( mouseState.x, mouseState.y );
		mouseState.mouseButtonPressed = KeyInput->IsMousePressed();
	}

	EventHandler::Instance().Update( mouseState );

	return this->privData->nextState;
}

bool LanMenuState::Render( )
{
	Graphics::API::NewFrame();

	Graphics::API::StartGuiRender();

	Graphics::API::RenderGuiElement( this->privData->background, Float3(0.5f, 0.5f, 1.0f), Float2(1.0f) );
	this->privData->guiElements.RenderTexture();

	Graphics::API::StartTextRender();
	this->privData->guiElements.RenderText();

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
		//if( !this->privData->nwClient->Connect(this->privData->connectPort, (*this->privData->connectIP)[0]) )
		//	return; // TODO: temporary commented out
		break;
	default: break;
	}

	this->privData->nextState = next;
}

void OnButtonInteract_Connect( Oyster::Event::ButtonEvent<LanMenuState*>& e )
{
	switch( e.state )
	{
	case ButtonState_Released:
		e.owner->ChangeState( GameClientState::ClientState_Lobby );
		break;
	default: break;
	}
}

void OnButtonInteract_Exit( Oyster::Event::ButtonEvent<LanMenuState*>& e )
{
	switch( e.state )
	{
	case ButtonState_Released:
		e.owner->ChangeState( GameClientState::ClientState_Main );
		break;
	default: break;
	}
}
