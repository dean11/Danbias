#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "RenderEngine.h"
#include "SoundSystem.h"
#include "GameLogic.h"
#include "Engine.h"
#include "NetworkConnection.h"
#include "NetworkUpdateStructs.h"

class Player
{
//	friend class NetworkConnection;
//	friend class IntoTheVoid;
	friend class RenderEngine;

private:
	Oyster::Render::Model* renderAvatar;
	Game::Ship logicAvatar;
	Oyster::Math::Float4x4 *orientationPointer;

	std::string name;
	SoundSource *weaponSound;

public:
	enum Result { Success, Failure };
	static Result init( );

	Player( const std::string &objFile = "", const ::Oyster::Math::Float4x4 &transform = ::Oyster::Math::Float4x4::identity );
	virtual ~Player();

	Game::Ship & accessShip( );
	const Game::Ship & accessShip( ) const;

	void update( const ::Oyster::Math::Float &deltaTime , ClientToServerUpdateData &update);
	void render( ) const;

	SoundSource *getWeaponSound() const;
	const Oyster::Math::Float4x4 *getOrientation();

	// transform is a matrix that is supposed to change the shape of the ship. Scaling as an example
	void setMutation(const Oyster::Math::Float4x4 &transform);

	void copyLogicFrom( const Player &player ); // Not garbage .. grrrrr ~Dan

	std::string getName() const;
	void setName(const std::string &name);

	int HP,Score, Shield;// evul public /Par

	float Speed;

};

#endif