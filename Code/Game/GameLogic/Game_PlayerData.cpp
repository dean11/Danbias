#include "Game.h"
#include "Player.h"

using namespace GameLogic;

Game::PlayerData::PlayerData()
{
	Oyster::Physics::API::SimpleBodyDescription sbDesc;
	//set some stats that are appropriate to a player

	//create rigidbody
	Oyster::Physics::ICustomBody *rigidBody = Oyster::Physics::API::Instance().CreateRigidBody(sbDesc).Release();


	//create player with this rigidbody


	this->player = new Player(rigidBody,Player::PlayerCollision,OBJECT_TYPE::OBJECT_TYPE_PLAYER);
	this->player->GetRigidBody()->SetCustomTag(this);
}
Game::PlayerData::PlayerData(int playerID,int teamID)
{
	this->player = new Player();
}
Game::PlayerData::~PlayerData()
{
	delete this->player;
}

void Game::PlayerData::Move(const PLAYER_MOVEMENT &movement)
{
	this->player->Move(movement);
}
void Game::PlayerData::UseWeapon(const WEAPON_FIRE &usage)
{
	this->player->UseWeapon(usage);
}
Oyster::Math::Float3 Game::PlayerData::GetPosition()
{
	return this->player->GetPosition();
}
Oyster::Math::Float4x4 Game::PlayerData::GetOrientation()
{
	return this->player->GetOrientation();
}
PLAYER_STATE Game::PlayerData::GetState()	const
{
	return this->player->GetState();
}
int Game::PlayerData::GetID() const
{
	return this->player->GetID();
}
int Game::PlayerData::GetTeamID() const
{
	return this->player->GetTeamID();
}
OBJECT_TYPE Game::PlayerData::GetObjectType()	const
{
	return this->player->GetType();
}
void Game::PlayerData::Rotate(const Oyster::Math3D::Float3 lookDir)
{
	//this->player->Rotate(lookDir);
}