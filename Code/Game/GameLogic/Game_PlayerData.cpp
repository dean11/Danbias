#include "Game.h"
#include "Player.h"

using namespace GameLogic;

Game::PlayerData::PlayerData()
{	
	//set some stats that are appropriate to a player
	Oyster::Physics::API::SimpleBodyDescription sbDesc;
	sbDesc.centerPosition = Oyster::Math::Float3(0,308,0);
	sbDesc.size = Oyster::Math::Float3(4,7,4);
	sbDesc.mass = 70;
	sbDesc.restitutionCoeff = 0.5;
	sbDesc.frictionCoeff_Static = 0.4;
	sbDesc.frictionCoeff_Dynamic = 0.3;
	sbDesc.rotation = Oyster::Math::Float3(0, Oyster::Math::pi, 0);

	//create rigid body
	Oyster::Physics::ICustomBody *rigidBody = Oyster::Physics::API::Instance().CreateRigidBody(sbDesc).Release();
	
	//create player with this rigid body
	this->player = new Player(rigidBody,Object::DefaultCollisionBefore, Player::PlayerCollision, OBJECT_TYPE::OBJECT_TYPE_PLAYER);
	this->player->GetRigidBody()->SetCustomTag(this);
	/*Oyster::Physics::ICustomBody::State state;
	this->player->GetRigidBody()->GetState(state);
	state.SetRotation(Oyster::Math::Float3(0, Oyster::Math::pi, 0));
	this->player->GetRigidBody()->SetState(state);
	player->EndFrame();*/
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
	return this->player->GetObjectType();
}
void Game::PlayerData::Rotate(const Oyster::Math3D::Float4 lookDir)
{
	this->player->Rotate(lookDir);
}