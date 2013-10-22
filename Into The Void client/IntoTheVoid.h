#pragma once

#include "NetworkConnection.h"
#include "GameState.h"
#include "TailCamera.h"
#include "Player.h"
#include "Protocoll.h"
#include "GameEndedMenu.h"

class RenderEngine;

class IntoTheVoid : public GameState
{
public:
	friend class RenderEngine;
	//friend class NetworkConnection;

	// IntoTheVoid takes ownership of the connection.
	IntoTheVoid(NetworkConnection *connection);
	virtual ~IntoTheVoid();

	void Enter(GameStateStack *stateStack);
	void Exit();

	void Suspend();
	void Resume();

	bool WantsBackgroundUpdates() const;

	void Update(float timeDelta);
	void Render() const;

	void SetPlayerPosition( unsigned int id, const Oyster::Math::Float4x4 &position );
	void SetPlayerUpdate( Network::ServerToClientUpdateData& data );
	void RecievedKeyFrame( const KeyFrame &keyframe );

	bool Initialized;

	bool gameRunning;
private:
	float updatePeriod, elapsedUpdatePeriod;
	mutable bool readOnly;

	bool isSuspended;
	bool isFirstUpdate;

	GameStateStack *stateStack;
	NetworkConnection *connection;

	unsigned int playerID;
	
	static const int MAX_PLAYERS = 8;
	Player *players[MAX_PLAYERS]; // the data send from from the server.
	//Player self; // For the second time; SUPERIMPORTANT!! Do not remove! It is the physical data that is manipulated by the player before sending it back to the server. ~Dan

	struct
	{
		unsigned int num;
		::Oyster::Render::Model *model;
		::Oyster::Math::Float4x4 *world;
	} staticObject;

	TailCamera *camera;

	Score scores[MAX_PLAYERS];

	void lockMutex( ) const;
	void releaseMutex( ) const;
	void SetRenderData( const Protocol::RenderData &renderData );
};
