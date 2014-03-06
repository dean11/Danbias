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
	this->background		= nullptr;
	this->mouseCursor		= nullptr;
	this->nextState 		= GameStateUI::UIState_same;
	this->render			= false;
}

IngameMenyUI::IngameMenyUI( SharedStateContent* shared ) :
	GameStateUI( shared )
{
	this->mousePos = Float3( 0.0f );
	this->background		= nullptr;
	this->mouseCursor		= nullptr;
	this->nextState			= GameStateUI::UIState_same;
	this->render			= false;
}

IngameMenyUI::~IngameMenyUI() { }
void OnButtonInteract_InGame_Exit( Oyster::Event::ButtonEvent<IngameMenyUI*>& e );
void OnButtonInteract_InGame_Options( Oyster::Event::ButtonEvent<IngameMenyUI*>& e );
void OnButtonInteract_InGame_Resume( Oyster::Event::ButtonEvent<IngameMenyUI*>& e );

bool IngameMenyUI::Init()
{
	
	// z value should be between 0.1 - 0.5 so that it will be above other states
	// add textures and text
	this->background	= new Plane_UI(L"color_white.png", Float3(0.5f, 0.5f, 0.5f), Float2(0.7f, 0.7f), Float4(0,0,0,0.5));
	this->mouseCursor	= new Plane_UI(L"cursor.png", Float3(0.5f, 0.5f, 0.5f), Float2(0.7f, 0.7f));
	
	ButtonRectangle<IngameMenyUI*> *button;
	Float4 TextCol = Float4(0.1f,0.1f,0.1f,1.0f);
	Float4 BackCol = Float4(0.6f, 0.6f, 0.6f, 1.0f);
	Float4 HoverCol = Float4(0.8f, 0.8f, 0.8f, 1.0f);
	Float4 PressCol = Float4(1.0f, 1.0f, 1.0f, 1.0f);
	
	button = new ButtonRectangle<IngameMenyUI*>( L"noedge-btn-quit.png", L"", TextCol, BackCol, HoverCol, PressCol, OnButtonInteract_InGame_Exit, this, Float3(0.5f, 0.3f, 0.2f), Float2(0.5f, 0.18f));
	menyButtons.AddButton( button );

	button = new ButtonRectangle<IngameMenyUI*>( L"noedge-btn-settings.png", L"", TextCol, BackCol, HoverCol, PressCol, OnButtonInteract_InGame_Options, this, Float3(0.5f, 0.45f, 0.2f), Float2(0.5f, 0.18f));
	menyButtons.AddButton( button );

	button = new ButtonRectangle<IngameMenyUI*>( L"noedge-btn-back.png", L"", TextCol, BackCol, HoverCol, PressCol, OnButtonInteract_InGame_Resume, this, Float3(0.5f, 0.6f, 0.2f), Float2(0.5f, 0.18f));
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
	MouseInput mouseState;
	{
		::Input::Struct::SAIPointFloat2D pos;
		this->shared->mouseDevice->GetNormalizedPosition( pos );

		this->mousePos.x = mouseState.x = pos.x;
		this->mousePos.y = mouseState.y = pos.y;
		mouseState.mouseButtonPressed = false;
	}
	EventHandler::Instance().Update( mouseState );

	// HACK: debug 
	this->debugOutput->setText(std::to_wstring(Oyster::Graphics::API::GetOption().bytesUsed));

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
	if(this->shared->mouseDevice->IsBtnDown(Input::Enum::SAMI_MouseLeftBtn))
		mouseCursor->RenderTexture( this->mousePos, Float2(0.15f));
	else
		mouseCursor->RenderTexture( this->mousePos, Float2(0.15f, 0.24) );

	this->background->RenderTexture();
	this->menyButtons.RenderTexture();
}

void IngameMenyUI::RenderText() 
{
	this->debugOutput->RenderText();
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

void IngameMenyUI::ActivateInput()
{
	this->render = true;
	this->shared->mouseDevice->AddMouseEvent( this );
	this->shared->keyboardDevice->AddKeyboardEvent( this );
}

void IngameMenyUI::DeactivateInput()
{
	this->render = false;
	this->shared->mouseDevice->RemoveMouseEvent( this );
	this->shared->keyboardDevice->RemoveKeyboardEvent( this );
}

void IngameMenyUI::OnMouseRelease( SAMI key, Mouse* sender )
{
	if( sender == this->shared->mouseDevice && key == SAMI_MouseLeftBtn )
	{
		MouseInput mouseState;
		{
			mouseState.x = this->mousePos.x;
			mouseState.y = this->mousePos.y;
			mouseState.mouseButtonPressed = true;
		}

		EventHandler::Instance().Update( mouseState );
	}
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
	case ButtonState_Released:
		e.owner->ChangeState( GameStateUI::UIState_main_menu );
		break;
	default: break;
	}
}

void OnButtonInteract_InGame_Options( Oyster::Event::ButtonEvent<IngameMenyUI*>& e )
{
	switch( e.state )
	{
	case ButtonState_Released:
		e.owner->ChangeState( GameStateUI::UIState_ingame_options );
		break;
	default: break;
	}
}

void OnButtonInteract_InGame_Resume( Oyster::Event::ButtonEvent<IngameMenyUI*>& e )
{
	switch( e.state )
	{
	case ButtonState_Released:
		e.owner->ChangeState( GameStateUI::UIState_resume_game );
		break;
	default: break;
	}
}