#include "OptionState.h"
#include "EventHandler\EventHandler.h"
#include "..\Buttons\ButtonRectangle.h"
#include "..\Buttons\ButtonEllipse.h"
#include "..\C_obj\C_UIobject.h"

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
	std::wstring c = Float2ToStr(this->options.resolution);

	for (unsigned int i = 0; i < Utility::StaticArray::NumElementsOf(resolution); i++)
	{
		if( resolution[i] == c )
		{
			this->options.currentRes = i;
			break;
		}
	}
}
OptionState::~OptionState()
{
	this->sharedData = 0;
}

void OptionState::CreateMouseSlider()
{
	Float4 TextCol = Float4(0.1f,0.1f,0.1f,1.0f);
	Float4 BackCol = Float4(1.0f);
	Float4 HoverCol = Float4(1.2f);
	Float4 PressCol = Float4(1.5f);

	this->mouseSensitivity.pos = Float3(0.62f, 0.354f, 0.6f);
	this->mouseSensitivity.button =  new ButtonRectangle<OptionState*>( L"noedge-slider-btn.png", L"", TextCol, BackCol, 
																		HoverCol, PressCol, OnButtonInteract, this, 
																		Float3(this->mouseSensitivity.pos.xy.x, this->mouseSensitivity.pos.xy.y, 0.4f), 
																		Float2(0.016f, 0.029f));
	this->guiElements.AddButton( mouseSensitivity.button );

	mouseSensitivity.button->SetUserData((void*)(int)ButtonType_MouseSensitivity); 

	this->mouseSensitivity.mouseSlider = Oyster::Graphics::API::CreateTexture(L"noedge-slider-vertical.png");
	if(!mouseSensitivity.mouseSlider)
	{
		printf("Failed to load texture noedge-slider-vertical.png");
	}

}

bool OptionState::Init( SharedStateContent &shared )
{
	this->sharedData = &shared;
	this->nextState = GameClientState::ClientState_Same;
	this->musOrientation = Float3( 0.0f );

	this->sharedData->keyboardDevice->AddKeyboardEvent(this);
	this->sharedData->mouseDevice->AddMouseEvent(this);

	// create buttons
	ButtonRectangle<OptionState*> *button;
	Float4 TextCol = Float4(0.1f,0.1f,0.1f,1.0f);
	Float4 BackCol = Float4(1.0f);
	Float4 HoverCol = Float4(1.2f);
	Float4 PressCol = Float4(1.5f);
	
	CreateMouseSlider();

	button = new ButtonRectangle<OptionState*>( L"noedge-btn-fScreenOn.png", L"", TextCol, BackCol, HoverCol, PressCol, OnButtonInteract, this, Float3(0.47f, 0.26f, 0.1f), Float2(0.045f, 0.045f));
	this->guiElements.AddButton( button );
	button->SetUserData((int)ButtonType_FullScreen);

	fScreenBtnToggle = new ButtonRectangle<OptionState*>( L"noedge-btn-fScreenOff.png", L"", TextCol, BackCol, HoverCol, PressCol, OnButtonInteract, this, Float3(0.47f, 0.26f, 0.1f), Float2(0.045f, 0.045f));
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

	button = new ButtonRectangle<OptionState*>( L"noedge-btn-flipresLeft.png", L"", TextCol, BackCol, HoverCol, PressCol, OnButtonInteract, this, Float3(0.459f, 0.42f, 0.2f), Float2(0.035f, 0.035f));
	this->guiElements.AddButton( button );
	button->SetUserData((void*)(int)ButtonType_FlipResLeft);

	button = new ButtonRectangle<OptionState*>( L"noedge-btn-flipresright.png", L"", TextCol, BackCol, HoverCol, PressCol, OnButtonInteract, this, Float3(0.7f, 0.42f, 0.2f), Float2(0.035f, 0.035f));
	this->guiElements.AddButton( button );
	button->SetUserData((void*)(int)ButtonType_FlipResRight);

	button = new ButtonRectangle<OptionState*>( L"noedge-btn-apply.png", L"", TextCol, BackCol, HoverCol, PressCol, OnButtonInteract, this, Float3(0.37f, 0.8f, 0.5f), Float2(0.24f, 0.094f));
	this->guiElements.AddButton( button );
	button->SetUserData((void*)(int)ButtonType_Apply);

	button = new ButtonRectangle<OptionState*>( L"noedge-btn-back.png", L"", TextCol, BackCol, HoverCol, PressCol, OnButtonInteract, this, Float3(0.63f, 0.8f, 0.5f), Float2(0.24f, 0.094f));
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
	
	//Render mouse Slider
	Graphics::API::RenderGuiElement( this->mouseSensitivity.mouseSlider, this->mouseSensitivity.pos, Float2(0.35f, 0.003f), Float4(1.0f) );
	
	if(this->sharedData->mouseDevice->IsBtnDown(Input::Enum::SAMI_MouseLeftBtn))
		Graphics::API::RenderGuiElement( this->sharedData->mouseCursor, this->musOrientation, Float2(0.15f), Float4(1.0f) );
	else
		Graphics::API::RenderGuiElement( this->sharedData->mouseCursor, this->musOrientation, Float2(0.15f, 0.24f), Float4(1.0f) );
	
	Graphics::API::RenderGuiElement( this->sharedData->background, Float3(0.5f, 0.5f, 0.9f), Float2(1.0f), Float4(0.0f, 0.0f, 0.0f, 1.0f) );
		this->guiElements.RenderTexture();

	Graphics::API::StartTextRender();
		Graphics::API::RenderText(L"Full Screen" , Float3(0.05f, 0.24f, 0.1f), Float2(0.4f, 0.1f), 0.028f);

		Float3 sp = this->mouseSensitivity.pos;
		sp.x = 0.05f; sp.y -= 0.028f;
		Graphics::API::RenderText(L"Mouse sensitivity" , sp, Float2(0.4f, 0.1f), 0.028f);
		
		wchar_t val[25];
		swprintf_s(val, L"%f", this->sharedData->mouseDevice->GetSensitivity() * 10.0f);
		val[3] = L'\0';
		Float3 t = this->mouseSensitivity.button->GetPosition(); t.y = 0.29f; t.x -= 0.04f;
		Graphics::API::RenderText( val, t, Float2(0.13f, 0.13f), 0.028f);

		Graphics::API::RenderText(L"Resolution" , Float3(0.05f, 0.398f, 0.1f), Float2(0.4f, 0.1f), 0.028f);
		Graphics::API::RenderText(resolution[this->options.currentRes], Float3(0.5f, 0.41f, 0.1f), Float2(0.2f, 0.05f), 0.028f);
		this->guiElements.RenderText();

	Graphics::API::EndFrame();

	return true;
}
bool OptionState::Release()
{
	EventHandler::Instance().ReleaseCollection( &this->guiElements );
	this->sharedData->keyboardDevice->RemoveKeyboardEvent(this);
	this->sharedData->mouseDevice->RemoveMouseEvent(this);

	return true;
}
void OptionState::ChangeState( ClientState next )
{

}
void OptionState::PlaySound( SoundID soundID, ChannelID channelID, PlayMode playMode )
{
	Sound::ISound* sound = this->sharedData->soundManager->getSound(soundID);
	Sound::IChannel* channel = this->sharedData->soundManager->getChannel(channelID);
	
	this->sharedData->soundManager->PlaySoundOnChannel(sound, channel, playMode);
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
	if(this->mouseSensitivity.isHeld)
	{
		this->mouseSensitivity.isHeld = false;
	}
}
void OptionState::OnMouseMovePixelPos	( Input::Struct::SAIPointInt2D coordinate, Input::Mouse* sender )
{
}
void OptionState::OnMouseMoveVelocity	( Input::Struct::SAIPointFloat2D coordinate, Input::Mouse* sender )
{
	if(this->mouseSensitivity.isHeld)
	{
		Float3 temp = this->mouseSensitivity.button->GetPosition();
		temp.x = (sender->GetNormalizedPosition().x);

		temp.x = std::max((this->mouseSensitivity.pos.x - (0.35f/2.0f)), temp.x);
		temp.x = std::min((this->mouseSensitivity.pos.x + (0.35f/2.0f)), temp.x);
		this->mouseSensitivity.button->SetPosition(temp ); 

		float vel = this->sharedData->mouseDevice->GetSensitivity();

		const float minThreshold = 0.1f;
		const float maxVal = 2.0f;
		float x = temp.x;
		float width = 0.35f;
		float halfWidth = 0.35f / 2.0f;
		float left = this->mouseSensitivity.pos.x - halfWidth;
		float localX = (temp.x - left);
		float value = localX * maxVal / width;
		
		this->sharedData->mouseDevice->SetSensitivity( std::max(value, minThreshold) );
	}
}
void OptionState::OnMouseScroll			( int delta, Input::Mouse* sender )
{
}
void OptionState::OnKeyEvent			( const Input::Struct::KeyboardEventData& eventData)
{
}
void OptionState::OnKeyPress			( Input::Enum::SAKI key, Input::Keyboard* sender)
{
}
void OptionState::OnKeyDown				( Input::Enum::SAKI key, Input::Keyboard* sender)
{
}
void OptionState::OnKeyRelease			( Input::Enum::SAKI key, Input::Keyboard* sender)
{
	if(key == Input::Enum::SAKI_Escape)
		this->nextState = GameClientState::ClientState_Main;
}


void ClientResize(HWND hWnd, int nWidth, int nHeight)
{
	RECT rcClient, rcWind;
	POINT ptDiff;
	GetClientRect(hWnd, &rcClient);
	GetWindowRect(hWnd, &rcWind);
	ptDiff.x = (rcWind.right - rcWind.left) - rcClient.right;
	ptDiff.y = (rcWind.bottom - rcWind.top) - rcClient.bottom;
	MoveWindow(hWnd,rcWind.left, rcWind.top, nWidth + ptDiff.x, nHeight + ptDiff.y, TRUE);
}
void OptionState::OnButtonInteract(Oyster::Event::ButtonEvent<OptionState*>& e)
{
	ButtonType op = (ButtonType)(int)e.userData;
	switch (op)
	{
		case DanBias::Client::OptionState::ButtonType_MouseSensitivity:
		{
			if(e.state == ButtonState_Pressed)
			{
				e.owner->mouseSensitivity.isHeld = true;
			}
		}
		break; //Good thing to break..
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
			if(e.state == ButtonState_Hover)
			{
				// SOUND
				e.owner->PlaySound(SoundID_Mouse_Hover, ChannelID_Mouse_Hover_Button1, PlayMode::PlayMode_Restart);
			}
			else if(e.state == ButtonState_Released)
			{
				//e.owner->sharedData->mouseDevice->InversY(e.owner->options.invertMouse);
				//e.owner->sharedData->sound->Toggle(e.owner->options.toggleSound);
				Graphics::API::Option op = Graphics::API::GetOption();
				
				if(!op.fullscreen)
					ClientResize(WindowShell::GetHWND(), (int)e.owner->options.resolution.x, (int)e.owner->options.resolution.y);
				
				if(op.fullscreen)
				{
					Graphics::API::Option optemp = Graphics::API::GetOption();
					optemp.fullscreen = false;
					Graphics::API::SetOptions(optemp);
				}
				
				op.resolution = e.owner->options.resolution;
				op.fullscreen = e.owner->options.toogleFullScreen;
				Graphics::API::SetOptions(op);

				// SOUND
				e.owner->PlaySound(SoundID_Mouse_Click, ChannelID_Mouse_Click_Button1, PlayMode::PlayMode_Restart);
			}
		break;
		case DanBias::Client::OptionState::ButtonType_Back:
			if(e.state == ButtonState_Hover)
			{
				// SOUND
				e.owner->PlaySound(SoundID_Mouse_Hover, ChannelID_Mouse_Hover_Button2, PlayMode::PlayMode_Restart);
			}
			else if(e.state == ButtonState_Released)
			{
				e.owner->nextState = GameClientState::ClientState_Main;
				// SOUND
				e.owner->PlaySound(SoundID_Mouse_Click, ChannelID_Mouse_Click_Button2, PlayMode::PlayMode_Restart);
			}
		break;
	}
}









