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
#include "Buttons\ButtonEllipse.h"

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
	SharedStateContent* sharedData;
	GameClientState::ClientState nextState;

	Float3 mousePos;
	EventButtonCollection guiElements;
	C_AudioHandler* soundManager;
};

void OnButtonInteract_Create( Oyster::Event::ButtonEvent<MainState*>& e );
void OnButtonInteract_Settings( Oyster::Event::ButtonEvent<MainState*>& e );
void OnButtonInteract_Join( Oyster::Event::ButtonEvent<MainState*>& e );
void OnButtonInteract_Quit( Oyster::Event::ButtonEvent<MainState*>& e );

MainState::MainState() {}

MainState::~MainState()
{
	if( this->privData )
		this->Release();
}

bool MainState::Init( SharedStateContent &shared )
{
	this->privData = new MyData();

	this->privData->sharedData = &shared;
	this->privData->nextState = GameClientState::ClientState_Same;

	this->privData->mousePos = Float3( 0.0f );

	// create buttons
	ButtonRectangle<MainState*> *button;
	Float4 TextCol = Float4(0.1f,0.1f,0.1f,1.0f);
	Float4 BackCol = Float4(1.0f);
	Float4 HoverCol = Float4(1.2f);
	Float4 PressCol = Float4(1.5f);
	
	//button = new ButtonRectangle<MainState*>( L"color_white.png", L"Create",TextCol, BackCol, HoverCol, PressCol, OnButtonInteract_Create, this, Float3(0.5f, 0.2f, 0.5f), Float2(0.3f, 0.1f));
	//this->privData->guiElements.AddButton( button );

	button = new ButtonRectangle<MainState*>( L"noedge-btn-lan.png", L"", TextCol, BackCol, HoverCol, PressCol, OnButtonInteract_Join, this, Float3(0.5f, 0.4f, 0.5f), Float2(0.24f, 0.094f));
	this->privData->guiElements.AddButton( button );

	button = new ButtonRectangle<MainState*>( L"noedge-btn-settings.png", L"", TextCol, BackCol, HoverCol, PressCol, OnButtonInteract_Settings, this, Float3(0.5f, 0.5f, 0.5f), Float2(0.24f, 0.094f));
	this->privData->guiElements.AddButton( button );

	button = new ButtonRectangle<MainState*>( L"noedge-btn-quit.png", L"", TextCol, BackCol, HoverCol, PressCol, OnButtonInteract_Quit, this, Float3(0.5f, 0.6f, 0.5f), Float2(0.24f, 0.094f));
	this->privData->guiElements.AddButton( button );

	// bind button collection to the singleton eventhandler
	EventHandler::Instance().AddCollection( &this->privData->guiElements );
	
	// SOUND
	this->privData->soundManager = this->privData->sharedData->soundManager;
	this->privData->soundManager->getChannel(ChannelID_Menu_Soundtrack)->SetPauseChannel(false);
	return true;
}

GameClientState::ClientState MainState::Update( float deltaTime )
{
	if(this->privData->sharedData->keyboardDevice->IsKeyDown(Input::Enum::SAKI_Escape))
		return GameClientState::ClientState_Quit;

	MouseInput mouseState;
	{
		::Input::Struct::SAIPointFloat2D pos;
		this->privData->sharedData->mouseDevice->GetNormalizedPosition( pos );

		this->privData->mousePos.x = mouseState.x = pos.x;
		this->privData->mousePos.y = mouseState.y = pos.y;
		mouseState.mouseButtonPressed = this->privData->sharedData->mouseDevice->IsBtnDown( ::Input::Enum::SAMI_MouseLeftBtn );
	}
	EventHandler::Instance().Update( mouseState );

	return this->privData->nextState;
}

bool MainState::Render()
{
	Graphics::API::NewFrame();
	Graphics::API::StartGuiRender();

	if(this->privData->sharedData->mouseDevice->IsBtnDown(Input::Enum::SAMI_MouseLeftBtn))
		Graphics::API::RenderGuiElement( this->privData->sharedData->mouseCursor, this->privData->mousePos, Float2(0.15f), Float4(1.0f) );
	else
		Graphics::API::RenderGuiElement( this->privData->sharedData->mouseCursor, this->privData->mousePos, Float2(0.15f, 0.24), Float4(1.0f) );

	Graphics::API::RenderGuiElement( this->privData->sharedData->background, Float3(0.5f, 0.5f, 0.9f), Float2(1.0f), Float4(0.0f, 0.0f, 0.0f, 1.0f) );
	this->privData->guiElements.RenderTexture();

	Graphics::API::StartTextRender();
	this->privData->guiElements.RenderText();

	Graphics::API::EndFrame();
	return true;
}

bool MainState::Release()
{
	if( this->privData )
	{
		EventHandler::Instance().ReleaseCollection( &this->privData->guiElements );

		this->privData = NULL;
		// button collection will be autoreleased from EventHandler
	}
	
	return true;
}

void MainState::ChangeState( ClientState next )
{
	this->privData->nextState = next;
}
void MainState::PlaySound( SoundID soundID, ChannelID channelID, PlayMode playMode )
{
	Sound::ISound* sound = this->privData->soundManager->getSound(soundID);
	Sound::IChannel* channel = this->privData->soundManager->getChannel(channelID);

	this->privData->soundManager->PlaySoundOnChannel(sound, channel, playMode);
}
void OnButtonInteract_Create( Oyster::Event::ButtonEvent<MainState*>& e )
{
	switch( e.state )
	{
	case ButtonState_Hover:
		// SOUND
		e.owner->PlaySound(SoundID_Mouse_Hover, ChannelID_Mouse_Hover_Button1, PlayMode_Restart);
		break;
	case ButtonState_Released:
		e.owner->ChangeState( GameClientState::ClientState_LobbyCreate );
		// SOUND
		e.owner->PlaySound(SoundID_Mouse_Click, ChannelID_Mouse_Click_Button1, PlayMode_Restart);
		break;
	default: break;
	}
}

void OnButtonInteract_Settings( Oyster::Event::ButtonEvent<MainState*>& e )
{
	switch( e.state )
	{
	case ButtonState_Hover:
		// SOUND
		e.owner->PlaySound(SoundID_Mouse_Hover, ChannelID_Mouse_Hover_Button2, PlayMode_Restart);
		break;
	case ButtonState_Released:
		e.owner->ChangeState( GameClientState::ClientState_Options );
		// SOUND
		e.owner->PlaySound(SoundID_Mouse_Click, ChannelID_Mouse_Click_Button2, PlayMode_Restart);
		break;
	default: break;
	}
}

void OnButtonInteract_Join( Oyster::Event::ButtonEvent<MainState*>& e )
{
	switch( e.state )
	{
	case ButtonState_Hover:
		// SOUND
		e.owner->PlaySound(SoundID_Mouse_Hover, ChannelID_Mouse_Hover_Button3, PlayMode_Restart);
		break;
	case ButtonState_Released:
		e.owner->ChangeState( GameClientState::ClientState_Lan );
		// SOUND
		e.owner->PlaySound(SoundID_Mouse_Click, ChannelID_Mouse_Click_Button3, PlayMode_Restart);
		break;
	default: break;
	}
}

void OnButtonInteract_Quit( Oyster::Event::ButtonEvent<MainState*>& e )
{
	switch( e.state )
	{
	case ButtonState_Hover:
		// SOUND
		e.owner->PlaySound(SoundID_Mouse_Hover, ChannelID_Mouse_Hover_Button4, PlayMode_Restart);
		break;
	case ButtonState_Released:
		e.owner->ChangeState( GameClientState::ClientState_Quit );
		// SOUND
		e.owner->PlaySound(SoundID_Mouse_Click, ChannelID_Mouse_Click_Button4, PlayMode_Restart);
		break;
	default: break;
	}
}