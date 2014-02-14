#include "Game.h"
#include "Player.h"

using namespace GameLogic;

Game::PlayerData::PlayerData()
{	
	//set some stats that are appropriate to a player

	Oyster::Math::Float3 centerPosition = Oyster::Math::Float3(50,130,0);

	Oyster::Math::Float3 size = Oyster::Math::Float3(0.25f,2.0f,0.5f);
	Oyster::Math::Float mass = 60;
	Oyster::Math::Float restitutionCoeff = 0.5f;
	Oyster::Math::Float frictionCoeff_Static = 0.4f;
	Oyster::Math::Float frictionCoeff_Dynamic = 0.3f;
	//sbDesc.quaternion = Oyster::Math::Float3(0, Oyster::Math::pi, 0);

	//create rigid body
	Oyster::Physics::ICustomBody* rigidBody = Oyster::Physics::API::Instance().AddCollisionBox(size, Oyster::Math::Float4(0, 0, 0, 1), centerPosition, mass, 0.5f, 0.8f, 0.6f );
	rigidBody->SetAngularFactor(0.0f);
	//create player with this rigid body
	this->player = new Player(rigidBody, Player::PlayerCollision, ObjectSpecialType_Player,0,0);

	//this->player->GetRigidBody()->SetCustomTag(this);
	player->EndFrame();
}
Game::PlayerData::PlayerData(int playerID,int teamID)
{
	Oyster::Physics::ICustomBody* rigidBody;
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
Oyster::Math::Quaternion Game::PlayerData::GetRotation()
{
	return this->player->GetRotation();
}
Oyster::Math::Float3 Game::PlayerData::GetScale()
{
	return this->player->GetScale();
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

ObjectSpecialType Game::PlayerData::GetObjectType()	const
{
	return this->player->GetObjectType();
}
void Game::PlayerData::Rotate(const Oyster::Math3D::Float3 lookDir, const Oyster::Math3D::Float3 right)
{
	this->player->Rotate(lookDir, right);
}