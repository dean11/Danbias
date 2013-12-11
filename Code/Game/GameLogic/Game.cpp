#include "Game.h"

using namespace GameLogic;

Game::Game(void)
{
	player	= NULL;
	level	= NULL;
	camera	= NULL;
}


Game::~Game(void)
{
	//SAFE_DELETE(player);
	if(player)
	{
		delete player;
		player = NULL;
	}
	if(camera)
	{
		delete camera;
		camera = NULL;
	}
}

void Game::Init()
{
	//Oyster::Physics::API::SetSubscription("remove object");
	
	player = new Player(L"worldDummy");

	box = new DynamicObject(L"crate");
	//poi
	//box = new physcTestObj("box");
	camera = new Camera();
}
void Game::StartGame()
{
	Oyster::Math::Float3 dir = Oyster::Math::Float3(0,0,-1);
	Oyster::Math::Float3 up  =Oyster::Math::Float3(0,1,0);
	Oyster::Math::Float3 pos = Oyster::Math::Float3(0, 0, 100);

	camera->LookAt(pos, dir, up);
	camera->SetLens(3.14f/2, 1024/768, 1, 1000);
}
void Game::Update(keyInput keyPressed, float pitch, float yaw)
{
	//player->Update(keyPressed);
	camera->Yaw(yaw);
	camera->Pitch(pitch);
	if(keyPressed == keyInput_A)
	{
		camera->Strafe(-0.1);
	}
	if(keyPressed == keyInput_D)
	{
		camera->Strafe(0.1);
	}
	if(keyPressed == keyInput_S)
	{
		camera->Walk(-0.1);
	}
	if(keyPressed == keyInput_W)
	{
		camera->Walk(0.1);
	}
	camera->UpdateViewMatrix();
	//poi Oyster::Physics::API::Update();
}
void Game::Render()
{
	Oyster::Graphics::API::SetView(camera->View());
	Oyster::Graphics::API::SetProjection(camera->Proj());
	Oyster::Graphics::API::NewFrame();
	player->Render();
	box->Render();
}