#include "IngameMenyUI.h"
#include <Protocols.h>
#include "Utilities.h"
// Debug

using namespace ::DanBias::Client;
using namespace ::Oyster::Network;
using namespace ::GameLogic;
using namespace ::Utility::Value;
using namespace ::Oyster::Math;
using namespace ::Input;
using namespace ::Input::Enum;
using namespace ::Oyster::Event;

IngameMenyUI::IngameMenyUI() :
	GameStateUI( nullptr )
{
	/* Should never be called! */
	this->mousePos			= Float3( 0.0f );
	this->audioHandler		= nullptr;
	this->background		= nullptr;
	this->mouseCursor		= nullptr;
	this->nextState 		= GameStateUI::UIState_same;
	this->render			= true;
}

IngameMenyUI::IngameMenyUI( SharedStateContent* shared ) :
	GameStateUI( shared )
{
	this->mousePos = Float3( 0.0f );
	this->audioHandler = shared->soundManager;
	this->background		= nullptr;
	this->mouseCursor		= nullptr;
	this->nextState			= GameStateUI::UIState_same;
	this->render			= true;
}

IngameMenyUI::~IngameMenyUI() { }
void OnButtonInteract_InGame_Exit( Oyster::Event::ButtonEvent<IngameMenyUI*>& e );
void OnButtonInteract_InGame_Options( Oyster::Event::ButtonEvent<IngameMenyUI*>& e );
void OnButtonInteract_InGame_Resume( Oyster::Event::ButtonEvent<IngameMenyUI*>& e );

bool IngameMenyUI::Init()
{
	
	// z value should be between 0.1 - 0.5 so that it will be above other states
	// add textures and text
	this->background	= new Plane_UI(L"color_white.png", Float3(0.5f, 0.5f, 0.5f), Float2(0.28f, 0.4f), Float4(1,1,1,0.5));
	this->mouseCursor	= new Plane_UI(L"cursor.png", Float3(0.5f, 0.5f, 0.5f), Float2(0.7f, 0.7f));
	
	ButtonRectangle<IngameMenyUI*> *button;
	Float4 TextCol = Float4(0.1f,0.1f,0.1f,1.0f);
	Float4 BackCol = Float4(0.6f, 0.6f, 0.6f, 1.0f);
	Float4 HoverCol = Float4(0.8f, 0.8f, 0.8f, 1.0f);
	Float4 PressCol = Float4(1.0f, 1.0f, 1.0f, 1.0f);
	
	button = new ButtonRectangle<IngameMenyUI*>( L"noedge-btn-quit.png", L"", TextCol, BackCol, HoverCol, PressCol, OnButtonInteract_InGame_Exit, this, Float3(0.5f, 0.6f, 0.2f), Float2(0.24f, 0.094f));
	menyButtons.AddButton( button );
	button->SetUserData(0);

	button = new ButtonRectangle<IngameMenyUI*>( L"noedge-btn-settings.png", L"", TextCol, BackCol, HoverCol, PressCol, OnButtonInteract_InGame_Options, this, Float3(0.5f, 0.5f, 0.2f), Float2(0.24f, 0.094f));
	menyButtons.AddButton( button );

	button = new ButtonRectangle<IngameMenyUI*>( L"noedge-btn-back.png", L"", TextCol, BackCol, HoverCol, PressCol, OnButtonInteract_InGame_Resume, this, Float3(0.5f, 0.4f, 0.2f), Float2(0.24f, 0.094f));
	menyButtons.AddButton( button );

	// HACK remove this later
	this->debugOutput 	= new Text_UI(L"", Float3(0.2f,0.8f,0.1f), Float2(0.3f,0.1f), 0.05f);

	// bind button collection to the singleton eventhandler
	EventHandler::Instance().AddCollection( &menyButtons );

	// setting input mode to all raw
	//this->mouseInput->AddMouseEvent(this);

	return true; 
}

GameStateUI::UIState IngameMenyUI::Update( float deltaTime )
{
	if(this->active)
	{
		MouseInput mouseState;
		{
			::Input::Struct::SAIPointFloat2D pos;
			this->shared->mouseDevice->GetNormalizedPosition( pos );
			this->mousePos.x = mouseState.x = pos.x;
			this->mousePos.y = mouseState.y = pos.y;
			mouseState.mouseButtonPressed = this->shared->mouseDevice->IsBtnDown(Input::Enum::SAMI_MouseLeftBtn);
		}
		EventHandler::Instance().Update( mouseState );

		// HACK: debug 
		this->debugOutput->setText(std::to_wstring(Oyster::Graphics::API::GetOption().bytesUsed));
	}

	return this->nextState;
}

bool IngameMenyUI::HaveGUIRender() const
{
	return this->render; 
}

bool IngameMenyUI::HaveTextRender() const
{
	return this->render; 
}

void IngameMenyUI::RenderGUI() 
{
	if(this->active)
	{
		if(this->shared->mouseDevice->IsBtnDown(Input::Enum::SAMI_MouseLeftBtn)) 
			Oyster::Graphics::API::RenderGuiElement( this->shared->mouseCursor, this->mousePos, Float2(0.15f));
			//mouseCursor->RenderTexture( this->mousePos, Float2(0.15f));
		else
			mouseCursor->RenderTexture( this->mousePos, Float2(0.15f, 0.24) );

		this->background->RenderTexture();
		this->menyButtons.RenderTexture();
	}
}

void IngameMenyUI::RenderText() 
{
	if(this->active)
	{
		this->debugOutput->RenderText();
	}
}

bool IngameMenyUI::Release()
{
	//Release as input event
	this->DeactivateInput();

	// TODO: Release UI components here.
	if(this->background) 	delete this->background;
	if(this->mouseCursor) 	delete this->mouseCursor;

	if(this->debugOutput) 	delete this->debugOutput;

	EventHandler::Instance().ReleaseCollection( &this->menyButtons );
	return true;
}

void IngameMenyUI::ChangeState( UIState next )
{
	this->nextState = next;
}
void IngameMenyUI::PlaySound( SoundID soundID, ChannelID channelID, PlayMode playMode )
{
	Sound::ISound* sound = this->audioHandler->getSound(soundID);
	Sound::IChannel* channel = this->audioHandler->getChannel(channelID);

	this->audioHandler->PlaySoundOnChannel(sound, channel, playMode);
}


void IngameMenyUI::ActivateInput()
{
	this->shared->mouseSensitivity = this->shared->mouseDevice->GetSensitivity();
	this->shared->mouseDevice->SetSensitivity(1.0f);
	this->active = true;
	this->menyButtons.SetState(Oyster::Event::EventCollectionState_Enabled);

	this->shared->mouseDevice->AddMouseEvent( this );
	this->shared->keyboardDevice->AddKeyboardEvent( this );
}

void IngameMenyUI::DeactivateInput()
{
	this->shared->mouseDevice->SetSensitivity(this->shared->mouseSensitivity);
	this->active = false;
	this->menyButtons.SetState(Oyster::Event::EventCollectionState_Disabled);

	this->shared->mouseDevice->RemoveMouseEvent( this );
	this->shared->keyboardDevice->RemoveKeyboardEvent( this );
}

void IngameMenyUI::OnMouseMoveVelocity( Input::Struct::SAIPointFloat2D key, Input::Mouse* sender )
{
	
}

void IngameMenyUI::OnMouseRelease( SAMI key, Mouse* sender )
{
	
}

void IngameMenyUI::OnKeyRelease( SAKI key, Keyboard* sender )
{
	if( key == SAKI_Escape )
	{
		this->ChangeState( UIState_previous );
	}
}

void OnButtonInteract_InGame_Exit( Oyster::Event::ButtonEvent<IngameMenyUI*>& e )
{
	switch( e.state )
	{
	case ButtonState_Hover:
		// SOUND
		e.owner->PlaySound(SoundID_Mouse_Hover, ChannelID_Mouse_Hover_Button1, PlayMode_Restart);
		break;
	case ButtonState_Released:
		e.owner->ChangeState( GameStateUI::UIState_main_menu );
		// SOUND
		e.owner->PlaySound(SoundID_Mouse_Click, ChannelID_Mouse_Click_Button1, PlayMode_Restart);
		break;
	default: break;
	}
}

void OnButtonInteract_InGame_Options( Oyster::Event::ButtonEvent<IngameMenyUI*>& e )
{
	switch( e.state )
	{
	case ButtonState_Hover:
		// SOUND
		e.owner->PlaySound(SoundID_Mouse_Hover, ChannelID_Mouse_Hover_Button2, PlayMode_Restart);
		break;
	case ButtonState_Released:
		e.owner->ChangeState( GameStateUI::UIState_ingame_options );
		// SOUND
		e.owner->PlaySound(SoundID_Mouse_Click, ChannelID_Mouse_Click_Button2, PlayMode_Restart);
		break;
	default: break;
	}
}

void OnButtonInteract_InGame_Resume( Oyster::Event::ButtonEvent<IngameMenyUI*>& e )
{
	switch( e.state )
	{
	case ButtonState_Hover:
		// SOUND
		e.owner->PlaySound(SoundID_Mouse_Hover, ChannelID_Mouse_Hover_Button3, PlayMode_Restart);
		break;
	case ButtonState_Released:
		e.owner->ChangeState( GameStateUI::UIState_resume_game );
		// SOUND
		e.owner->PlaySound(SoundID_Mouse_Click, ChannelID_Mouse_Click_Button3, PlayMode_Restart);
		break;
	default: break;
	}

}
