#include "OptionState.h"
#include "EventHandler\EventHandler.h"
#include "..\Buttons\ButtonRectangle.h"
#include "..\Buttons\ButtonEllipse.h"

using namespace DanBias; 
using namespace Client;
using namespace Oyster;
using namespace Oyster::Math;
using namespace Oyster::Network;
using namespace Oyster::Math3D;
using namespace Oyster::Network;
using namespace Oyster::Event;
using namespace Utility;
using namespace Utility::DynamicMemory;
using namespace Utility::StaticArray;


static wchar_t* resolution[] = 
{
	{ L"640x480" }, { L"800x600" }, { L"1024x600" }, { L"1024x768" }, 
	{ L"1152x864" }, { L"1280x720" }, { L"1280x768" }, { L"1280x800" }, 
	{ L"1280x960" }, { L"1280x1024" }, { L"1360x768" }, { L"1366x768" }, 
	{ L"1400x1050" }, { L"1440x900" }, { L"1600x900" }, { L"1600x1200" }, 
	{ L"1680x1050" }, { L"1920x1080" }, { L"1920x1200" }, { L"2048x1152" },
	{ L"2560x1440" }, { L"2560x1600" }
};

std::wstring Float2ToStr(Float2 v)
{
	std::wstring str;
	wchar_t buff[10];
	_itow_s((int)v.x, buff, 10); 
	str.append(buff);
	str.append(L"x");
	_itow_s((int)v.y, buff, 10); 
	str.append(buff);

	return str;
}
Float2 WStrToFloat2(std::wstring str)
{
	Float2 val;
	static std::vector<std::wstring> s;
	Utility::String::Split(s, str, L"x");
	val.x = _wtof(s[0].c_str());
	val.y = _wtof(s[1].c_str());
	s.clear();
	return val;
}


OptionState::OptionState()
{
	this->sharedData = 0;
	this->nextState = GameClientState::ClientState_Same;

	Graphics::API::Option op = Graphics::API::GetOption();
	this->options.toogleFullScreen = op.fullscreen;
	this->options.resolution = op.resolution;
	
}
OptionState::~OptionState()
{
	this->sharedData = 0;
}

bool OptionState::Init( SharedStateContent &shared )
{
	this->sharedData = &shared;
	this->nextState = GameClientState::ClientState_Same;
	this->musOrientation = Float3( 0.0f );

	this->sharedData->keyboardDevice->AddKeyboardEvent(this);

	// create buttons
	ButtonRectangle<OptionState*> *button;
	Float4 TextCol = Float4(0.1f,0.1f,0.1f,1.0f);
	Float4 BackCol = Float4(1.0f);
	Float4 HoverCol = Float4(1.2f);
	Float4 PressCol = Float4(1.5f);
	
	button = new ButtonRectangle<OptionState*>( L"noedge-btn-fScreenOn.png", L"", TextCol, BackCol, HoverCol, PressCol, OnButtonInteract, this, Float3(0.64f, 0.2f, 0.5f), Float2(0.045f, 0.045f));
	this->guiElements.AddButton( button );
	button->SetUserData((int)ButtonType_FullScreen);

	fScreenBtnToggle = new ButtonRectangle<OptionState*>( L"noedge-btn-fScreenOff.png", L"", TextCol, BackCol, HoverCol, PressCol, OnButtonInteract, this, Float3(0.64f, 0.2f, 0.5f), Float2(0.045f, 0.045f));
	this->guiElements.AddButton( fScreenBtnToggle );
	fScreenBtnToggle->SetUserData((int)ButtonType_FullScreen);

	if(this->options.toogleFullScreen)
	{
		fScreenBtnToggle->SetEnabled(false);
	}
	else
	{
		button->SetEnabled(false);
		fScreenBtnToggle = button;
	}

	button = new ButtonRectangle<OptionState*>( L"noedge-btn-flipresLeft.png", L"", TextCol, BackCol, HoverCol, PressCol, OnButtonInteract, this, Float3(0.3f, 0.4f, 0.5f), Float2(0.035f, 0.035f));
	this->guiElements.AddButton( button );
	button->SetUserData((void*)(int)ButtonType_FlipResLeft);

	button = new ButtonRectangle<OptionState*>( L"noedge-btn-flipresright.png", L"", TextCol, BackCol, HoverCol, PressCol, OnButtonInteract, this, Float3(0.7f, 0.4f, 0.5f), Float2(0.035f, 0.035f));
	this->guiElements.AddButton( button );
	button->SetUserData((void*)(int)ButtonType_FlipResRight);

	button = new ButtonRectangle<OptionState*>( L"noedge-btn-apply.png", L"", TextCol, BackCol, HoverCol, PressCol, OnButtonInteract, this, Float3(0.5f, 0.65f, 0.5f), Float2(0.3f, 0.1f));
	this->guiElements.AddButton( button );
	button->SetUserData((void*)(int)ButtonType_Apply);

	button = new ButtonRectangle<OptionState*>( L"noedge-btn-back.png", L"", TextCol, BackCol, HoverCol, PressCol, OnButtonInteract, this, Float3(0.5f, 0.8f, 0.5f), Float2(0.3f, 0.18f));
	this->guiElements.AddButton( button );
	button->SetUserData((void*)(int)ButtonType_Back);

	// bind button collection to the singleton eventhandler
	EventHandler::Instance().AddCollection( &this->guiElements );

	return true;
}
GameClientState::ClientState OptionState::Update( float deltaTime )
{
	MouseInput mouseState;
	{
		::Input::Struct::SAIPointFloat2D pos;
		this->sharedData->mouseDevice->GetNormalizedPosition( pos );
		this->musOrientation.x = mouseState.x = pos.x;
		this->musOrientation.y = mouseState.y = pos.y;

		mouseState.mouseButtonPressed = this->sharedData->mouseDevice->IsBtnDown( ::Input::Enum::SAMI_MouseLeftBtn );
	}
	EventHandler::Instance().Update( mouseState );

	return this->nextState;
}
bool OptionState::Render()
{
	Graphics::API::NewFrame();
	Graphics::API::StartGuiRender();
	
	
	if(this->sharedData->mouseDevice->IsBtnDown(Input::Enum::SAMI_MouseLeftBtn))
		Graphics::API::RenderGuiElement( this->sharedData->mouseCursor, this->musOrientation, Float2(0.15f), Float4(1.0f) );
	else
		Graphics::API::RenderGuiElement( this->sharedData->mouseCursor, this->musOrientation, Float2(0.15f, 0.24f), Float4(1.0f) );
	
	Graphics::API::RenderGuiElement( this->sharedData->background, Float3(0.5f, 0.5f, 0.9f), Float2(1.0f), Float4(0.0f, 0.0f, 0.0f, 1.0f) );
		this->guiElements.RenderTexture();

	Graphics::API::StartTextRender();
		Graphics::API::RenderText(L"Full Screen" , Float3(0.33f, 0.14f, 0.1f), Float2(0.5f, 0.24f), 0.034f);
		Graphics::API::RenderText(resolution[this->options.currentRes], Float3(0.36f, 0.36f, 0.1f), Float2(0.3f, 0.18f), 0.034f);
		this->guiElements.RenderText();

	Graphics::API::EndFrame();

	return true;
}
bool OptionState::Release()
{
	EventHandler::Instance().ReleaseCollection( &this->guiElements );
	this->sharedData->keyboardDevice->RemoveKeyboardEvent(this);

	return true;
}
void OptionState::ChangeState( ClientState next )
{

}
const DanBias::Client::GameClientState::NetEvent & OptionState::DataRecieved( const NetEvent &message )
{
	/* Todo update when in game options */
	return message;
}


void OptionState::OnMouse				( const Input::Struct::MouseEventData& eventData )
{
}
void OptionState::OnMousePress			( Input::Enum::SAMI key, Input::Mouse* sender )
{
}
void OptionState::OnMouseDown			( Input::Enum::SAMI key, Input::Mouse* sender )
{
}
void OptionState::OnMouseRelease		( Input::Enum::SAMI key, Input::Mouse* sender )
{
}
void OptionState::OnMouseMovePixelPos	( Input::Struct::SAIPointInt2D coordinate, Input::Mouse* sender )
{
}
void OptionState::OnMouseMoveVelocity	( Input::Struct::SAIPointInt2D coordinate, Input::Mouse* sender )
{
}
void OptionState::OnMouseScroll			( int delta, Input::Mouse* sender )
{
}
void OptionState::OnKeyEvent			(const Input::Struct::KeyboardEventData& eventData)
{
}
void OptionState::OnKeyPress			(Input::Enum::SAKI key, Input::Keyboard* sender)
{
}
void OptionState::OnKeyDown				(Input::Enum::SAKI key, Input::Keyboard* sender)
{
}
void OptionState::OnKeyRelease			(Input::Enum::SAKI key, Input::Keyboard* sender)
{
	if(key == Input::Enum::SAKI_Escape)
		this->nextState = GameClientState::ClientState_Main;
}



void OptionState::OnButtonInteract(Oyster::Event::ButtonEvent<OptionState*>& e)
{
	ButtonType op = (ButtonType)(int)e.userData;
	switch (op)
	{
		case DanBias::Client::OptionState::ButtonType_FullScreen:
			if(e.state == ButtonState_Released)
			{
				e.owner->options.toogleFullScreen = !e.owner->options.toogleFullScreen;
				e.sender->SetEnabled(false);
				e.owner->fScreenBtnToggle->SetEnabled(true);
				e.owner->fScreenBtnToggle = (ButtonRectangle<OptionState*>*)e.sender;
			}
		break;
		case DanBias::Client::OptionState::ButtonType_FlipResLeft:
			if(e.state == ButtonState_Released)
			{
				int a = (e.owner->options.currentRes - 1);
				static const int b = (int)Utility::StaticArray::NumElementsOf(resolution);
				
				e.owner->options.currentRes = a % b;
				if(e.owner->options.currentRes < 0) e.owner->options.currentRes = b - 1;
				e.owner->options.resolution = WStrToFloat2( resolution[e.owner->options.currentRes] );
			}
		break;
		case DanBias::Client::OptionState::ButtonType_FlipResRight:
			if(e.state == ButtonState_Released)
			{
				e.owner->options.currentRes = (e.owner->options.currentRes + 1) % Utility::StaticArray::NumElementsOf(resolution);
				e.owner->options.resolution = WStrToFloat2( resolution[e.owner->options.currentRes] );
			}
		break;
		case DanBias::Client::OptionState::ButtonType_Apply:
			if(e.state == ButtonState_Released)
			{
				//e.owner->sharedData->mouseDevice->InversY(e.owner->options.invertMouse);
				//e.owner->sharedData->sound->Toggle(e.owner->options.toggleSound);
				Graphics::API::Option op = Graphics::API::GetOption();
				op.fullscreen = e.owner->options.toogleFullScreen;
				//op.fullscreen = true;
				op.resolution = e.owner->options.resolution;


				Graphics::API::SetOptions(op);
			}
		break;
		case DanBias::Client::OptionState::ButtonType_Back:
			if(e.state == ButtonState_Released)
			{
				e.owner->nextState = GameClientState::ClientState_Main;
			}
		break;
	}
}









