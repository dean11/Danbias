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

	GameClientState::ClientState nextState;
	NetworkClient *nwClient;
	Graphics::API::Texture background;
	EventButtonCollection button;
};

void OnButtonInteract_Create( Oyster::Event::ButtonEvent<GameClientState*>& e );
void OnButtonInteract_Join( Oyster::Event::ButtonEvent<GameClientState*>& e );
void OnButtonInteract_Quit( Oyster::Event::ButtonEvent<GameClientState*>& e );

MainState::MainState(void) {}

MainState::~MainState(void)
{
	if( this->privData )
		this->Release();
}

bool MainState::Init( NetworkClient* nwClient )
{
	this->privData = new MyData();

	this->privData->nextState = GameClientState::ClientState_Same;
	this->privData->nwClient = nwClient;

	this->privData->background = Graphics::API::CreateTexture( L"grass_md.png" );

	// create buttons
	ButtonRectangle<GameClientState*> *button;
	
	button = new ButtonRectangle<GameClientState*>( L"earth_md.png", OnButtonInteract_Create, this, 0.5f, 0.2f, 0.3f, 0.1f, true );
	this->privData->button.AddButton( button );

	button = new ButtonRectangle<GameClientState*>( L"skysphere_md.png", OnButtonInteract_Join, this, 0.5f, 0.4f, 0.3f, 0.1f, true );
	this->privData->button.AddButton( button );

	button = new ButtonRectangle<GameClientState*>( L"plane_texture_md.png", OnButtonInteract_Quit, this, 0.5f, 0.8f, 0.3f, 0.1f, true );
	this->privData->button.AddButton( button );

	// bind button collection to the singleton eventhandler
	EventHandler::Instance().AddCollection( &this->privData->button );

	return true;
}

GameClientState::ClientState MainState::Update(float deltaTime, InputClass* KeyInput)
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

bool MainState::Render()
{
	Graphics::API::NewFrame();
	Graphics::API::StartGuiRender();

	Graphics::API::RenderGuiElement( this->privData->background, Float2(0.5f), Float2(1.0f) );
	this->privData->button.Render();

	Graphics::API::EndFrame();
	return true;
}

bool MainState::Release()
{
	if( this->privData )
	{
		Graphics::API::DeleteTexture( this->privData->background ); // TODO: @todo bug caught when exiting by X
		EventHandler::Instance().ReleaseCollection( &this->privData->button );

		this->privData = NULL;
		// button collection will be autoreleased from EventHandler
	}
	return true;
}

void MainState::ChangeState( ClientState next )
{
	this->privData->nextState = next;
}

void OnButtonInteract_Create( Oyster::Event::ButtonEvent<GameClientState*>& e )
{
	switch( e.state )
	{
	case ButtonState_Released:
		e.owner->ChangeState( GameClientState::ClientState_LobbyCreate );
		break;
	default: break;
	}
}

void OnButtonInteract_Join( Oyster::Event::ButtonEvent<GameClientState*>& e )
{
	switch( e.state )
	{
	case ButtonState_Released:
		e.owner->ChangeState( GameClientState::ClientState_Lan );
		break;
	default: break;
	}
}

void OnButtonInteract_Quit( Oyster::Event::ButtonEvent<GameClientState*>& e )
{
	switch( e.state )
	{
	case ButtonState_Released:
		e.owner->ChangeState( GameClientState::ClientState_Quit );
		break;
	default: break;
	}
}