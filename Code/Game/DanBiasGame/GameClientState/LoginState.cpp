#include "LoginState.h"
#include "DllInterfaces/GFXAPI.h"
#include "OysterMath.h"
#include "C_obj/C_Player.h"
#include "C_obj/C_StaticObj.h"
#include "C_obj/C_DynamicObj.h"
#include <GameServerAPI.h>

using namespace DanBias::Client;

//Menu buttons
#include "Buttons/ButtonEllipse.h"
#include "Buttons/ButtonRectangle.h"
#include "../Misc/EventHandler/EventHandler.h"
using namespace Oyster::Event;

struct  LoginState::myData
{
	myData(){}
	Oyster::Math3D::Float4x4 view;
	Oyster::Math3D::Float4x4 proj; 
	C_Object* object[2]; 
	int modelCount; 
	// UI object
	// game client* 

	//Menu button collection
	EventButtonCollection* collection;
	bool createGame;
	int testNumber;
}privData;


enum TestEnum
{
	Create,
	Options,
	Incr,
	Decr,
	Exit,
};

LoginState::LoginState(void)
{
}

void LoginState::ButtonCallback(Oyster::Event::ButtonEvent<LoginState*>& e)
{
	TestEnum type = TestEnum((int)e.userData);

	switch(type)
	{
	case Create:
		if(e.state == ButtonState_None)
		{
			int a = 0;
			std::cout << "None" << std::endl;
		}
		else if(e.state == ButtonState_Hover)
		{
			int a = 0;
			std::cout << "Hover" << std::endl;
		}
		else if(e.state == ButtonState_Down)
		{
			int a = 0;
			std::cout << "Down" << std::endl;
		}
		else if(e.state == ButtonState_Pressed)
		{
			int a = 0;
			std::cout << "Pressed" << std::endl;
		}
		else if(e.state == ButtonState_Released)
		{
			//Change to create state or something similar
			int a = 0;
			std::cout << "Released" << std::endl;
			e.owner->privData->createGame = true;
		}
		break;
	case Options:
		break;
	case Exit:
		break;

	case Incr:
		if(e.state == ButtonState_Released)
			e.owner->privData->testNumber++;
		break;
	case Decr:
		if(e.state == ButtonState_Released)
			e.owner->privData->testNumber--;
		break;
	}
}

LoginState::~LoginState(void)
{

}

bool LoginState::Init(Oyster::Network::NetworkClient* nwClient)
{
	privData = new myData();
	this->nwClient = nwClient;
	// load models
	//LoadModels(L"UImodels.txt");
	InitCamera(Oyster::Math::Float3(0,0,5.4f));

	//Create menu buttons
	privData->collection = new EventButtonCollection;
	EventHandler::Instance().AddCollection(privData->collection);
	privData->collection->AddButton(new ButtonEllipse<LoginState*>(L"circle.png", &LoginState::ButtonCallback, this, (void*)Options, 0.2f, 0.2f, 0.1f, 0.1f));
	privData->collection->AddButton(new ButtonEllipse<LoginState*>(L"circle.png", &LoginState::ButtonCallback, this, (void*)Options, 0.2f, 0.3f, 0.1f, 0.1f));
	privData->collection->AddButton(new ButtonEllipse<LoginState*>(L"circle.png", &LoginState::ButtonCallback, this, (void*)Options, 0.2f, 0.4f, 0.1f, 0.1f));
	privData->collection->AddButton(new ButtonEllipse<LoginState*>(L"circle.png", &LoginState::ButtonCallback, this, (void*)Options, 0.2f, 0.5f, 0.1f, 0.1f));

	privData->collection->AddButton(new ButtonRectangle<LoginState*>(L"button.png", &LoginState::ButtonCallback, this, (void*)Options, 0.15f, 0.05f, 0.1f, 0.1f));
	privData->collection->AddButton(new ButtonRectangle<LoginState*>(L"button.png", &LoginState::ButtonCallback, this, (void*)Options, 0.25f, 0.05f, 0.1f, 0.1f));
	privData->collection->AddButton(new ButtonRectangle<LoginState*>(L"button.png", &LoginState::ButtonCallback, this, (void*)Options, 0.35f, 0.05f, 0.1f, 0.1f));
	privData->collection->AddButton(new ButtonRectangle<LoginState*>(L"button.png", &LoginState::ButtonCallback, this, (void*)Options, 0.45f, 0.05f, 0.1f, 0.1f));

	privData->collection->AddButton(new ButtonRectangle<LoginState*>(L"button.png", &LoginState::ButtonCallback, this, (void*)Create, 0.5f, 0.5f, 0.3f, 0.3f));

	//Incr/decr buttons
	privData->collection->AddButton(new ButtonRectangle<LoginState*>(L"button.png", &LoginState::ButtonCallback, this, (void*)Incr, 0.85f, 0.2f, 0.1f, 0.1f));
	privData->collection->AddButton(new ButtonRectangle<LoginState*>(L"button.png", &LoginState::ButtonCallback, this, (void*)Decr, 0.55f, 0.2f, 0.1f, 0.1f));
	
	privData->createGame = false;
	privData->testNumber = 0;

	return true;
}
bool LoginState::LoadModels(std::wstring file)
{
	Oyster::Graphics::Definitions::Pointlight plight;
	plight.Pos = Oyster::Math::Float3(0,0,5);
	plight.Color = Oyster::Math::Float3(1,0,1);
	plight.Radius = 100;
	plight.Bright = 1;
	Oyster::Graphics::API::AddLight(plight);
	// open file
	// read file 
	// init models
	privData->modelCount = 2;

	ModelInitData modelData;

	modelData.rotation = Oyster::Math::Quaternion::identity;
	modelData.scale =  Oyster::Math::Float3(1,1,1);
	modelData.visible = true;
	modelData.modelPath = L"box.dan";
	

	modelData.position = Oyster::Math::Float3(2,2,2);
	privData->object[0] = new C_StaticObj();
	privData->object[0]->Init(modelData);

	modelData.position = Oyster::Math::Float3(-2,0,-2);
	privData->object[1] = new C_StaticObj();
	privData->object[1]->Init(modelData);
	return true;
}

bool LoginState::InitCamera(Oyster::Math::Float3 startPos)
{
	privData->proj = Oyster::Math3D::ProjectionMatrix_Perspective(Oyster::Math::pi/2,1024.0f/768.0f,.1f,1000);
	//privData->proj = Oyster::Math3D::ProjectionMatrix_Orthographic(1024, 768, 1, 1000);
	Oyster::Graphics::API::SetProjection(privData->proj);

	privData->view = Oyster::Math3D::OrientationMatrix_LookAtDirection(Oyster::Math::Float3(0,0,-1),Oyster::Math::Float3(0,1,0),startPos);
	privData->view = Oyster::Math3D::InverseOrientationMatrix(privData->view);
	return true;
}
GameClientState::ClientState LoginState::Update(float deltaTime, InputClass* KeyInput)
{
	// picking 
	// mouse events
	// different menus
	// play sounds
	// update animation
	// send data to server
	// check data from server

	// create game
	if( KeyInput->IsKeyPressed(DIK_C) || privData->createGame) 
	{
		DanBias::GameServerAPI::ServerInitDesc desc; 

		DanBias::GameServerAPI::ServerInitiate(desc);
		DanBias::GameServerAPI::ServerStart();
		// my ip
		nwClient->Connect(15152, "127.0.0.1");

		if (!nwClient->IsConnected())
		{
			// failed to connect
			return ClientState_Same;
		}
		privData->collection->SetState(EventCollectionState_Disabled);
		return ClientState_LobbyCreated;
	}
	// join game
	if( KeyInput->IsKeyPressed(DIK_J)) 
	{
		// game ip
		nwClient->Connect(15152, "127.0.0.1");

		if (!nwClient->IsConnected())
		{
			// failed to connect
			return ClientState_Same;
		}
		privData->collection->SetState(EventCollectionState_Disabled);
		return ClientState_Lobby;
	}
	return ClientState_Same;
}
bool LoginState::Render(float dt)
{
	Oyster::Graphics::API::SetView(privData->view);
	Oyster::Graphics::API::SetProjection( privData->proj);

	Oyster::Graphics::API::NewFrame();
	// render objects
	for (int i = 0; i < privData->modelCount; i++)
	{
		privData->object[i]->Render();
	}

	// render effects

	// render lights

	//Render buttons
	Oyster::Graphics::API::StartGuiRender();
	EventHandler::Instance().Render();

	std::wstring number;
	wchar_t temp[10];
	_itow_s(privData->testNumber, temp, 10);
	number = temp;

	Oyster::Graphics::API::StartTextRender();
	Oyster::Graphics::API::RenderText(number, Oyster::Math::Float2(0.7f, 0.2f), Oyster::Math::Float2(0.1f, 0.1f*(1008.0f/730.0f)), Oyster::Math::Float3(1.0f, 0.0f, 0.0f));

	Oyster::Graphics::API::EndFrame();
	return true;
}
bool LoginState::Release()
{
	Oyster::Graphics::API::ClearLights();
	for (int i = 0; i < privData->modelCount; i++)
	{
		privData->object[i]->Release();
		delete privData->object[i];
		privData->object[i] = NULL;
	}

	delete privData->collection;

	delete privData;  
	privData = NULL;
	return true;
}
void LoginState::Protocol(ProtocolStruct* protocol)
{
	if((PlayerName*)protocol)
		PlayerJoinProtocol((PlayerName*)protocol);

}
void LoginState::PlayerJoinProtocol(PlayerName* name)
{

}