// Wrapper class for the Graphic Render System

#pragma once
#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include "Engine.h"
#include "Player.h"
#include "Protocoll.h"
#include "NetworkConnection.h"
#include "Event.h"

class Player;

class RenderEngine
{
public:
	enum Result { Success, Failure, Idle };
	static Result init( HINSTANCE thisInstance, LPSTR cmdLine, int showCommand, WNDPROC wProc );

	static Result update(const Oyster::Math::Float4x4 &View, const Oyster::Math::Float4x4 &Projection, float deltaTime, NetworkConnection* conn);

	static void PrepareRendering();
	static void Present();

	static void RenderGame( const Player* const *p, int nrOfp, int localPlayerID, const ::Oyster::Render::Model* const staticObject , unsigned int numStaticObjects );
	static void RenderText(Oyster::Render::Textbox* t, int nrOft);
	static void RenderScoreboard(const Score scores[], int count, const Player * const * players);
	static void Damaged();

	static Oyster::Buffer *perObject;

private:
	static Oyster::Buffer *camera;
	static Oyster::Buffer *View;
};

#endif