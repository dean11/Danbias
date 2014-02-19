#include "Game.h"
#include "Player.h"

using namespace GameLogic;

Game::PlayerData::PlayerData()
{	
	Oyster::Math::Float3 centerPosition = Oyster::Math::Float3(-50,250,0);
	Oyster::Math::Float height = 2.0f;
	Oyster::Math::Float radius = 0.5f;
	Oyster::Math::Float mass = 40;
	Oyster::Math::Float restitutionCoeff = 0.5f;
	Oyster::Math::Float frictionCoeff_Static = 0.4f;
	Oyster::Math::Float frictionCoeff_Dynamic = 0.3f;

	//create rigid body
	Oyster::Physics::ICustomBody* rigidBody = Oyster::Physics::API::Instance().AddCharacter(	height, radius, Oyster::Math::Float4(0, 0, 0, 1), 
																							centerPosition, mass, restitutionCoeff, 
																							frictionCoeff_Static, frictionCoeff_Dynamic );
	rigidBody->SetAngularFactor(0.0f);
	//create player with this rigid body
	this->player = new Player(rigidBody, Player::PlayerCollision, ObjectSpecialType_Player, 0, 0);
}
Game::PlayerData::PlayerData(int playerID,int teamID)
{
	Oyster::Math::Float3 centerPosition = Oyster::Math::Float3(-50,250,0);
	Oyster::Math::Float height = 2.0f;
	Oyster::Math::Float radius = 0.5f;
	Oyster::Math::Float mass = 40;
	Oyster::Math::Float restitutionCoeff = 0.5f;
	Oyster::Math::Float frictionCoeff_Static = 0.4f;
	Oyster::Math::Float frictionCoeff_Dynamic = 0.3f;

	//create rigid body
	Oyster::Physics::ICustomBody* rigidBody = Oyster::Physics::API::Instance().AddCharacter(	height, radius, Oyster::Math::Float4(0, 0, 0, 1), 
																							centerPosition, mass, restitutionCoeff, 
																							frictionCoeff_Static, frictionCoeff_Dynamic );
	rigidBody->SetAngularFactor(0.0f);
	//create player with this rigid body
	this->player = new Player(rigidBody, Player::PlayerCollision, ObjectSpecialType_Player,playerID,teamID);
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
void Game::PlayerData::Rotate(const Oyster::Math3D::Float3& lookDir, const Oyster::Math3D::Float3& right)
{
	this->player->Rotate(lookDir, right);
}
void Game::PlayerData::TurnLeft(Oyster::Math3D::Float deltaLeftRadians )
{
	this->player->TurnLeft(deltaLeftRadians);
}