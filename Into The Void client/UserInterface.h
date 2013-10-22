// Wrapper for User Interface System

#pragma once
#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "Engine.h"
#include "Ship.h"
#include "TailCamera.h"
#include "NetworkUpdateStructs.h"

class UserInterface
{
public:
	enum InitResult { Success, Failure };
	static InitResult init( );

	enum UpdateResult { ShutDown, Updated, Idle };
	static UpdateResult MessageLoop( );

	static void Update(TailCamera &T, float DeltaTime, Network::ClientToServerUpdateData& data);

	static LRESULT CALLBACK wndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

	static bool isMovingForward( );
	static bool isMovingBackward( );
	static bool isStrafingLeft( );
	static bool isStrafingRight( );
	static bool isStrafingUp( );
	static bool isStrafingDown( );
	static bool isPitchingUp( );
	static bool isPitchingDown( );
	static void ResetPitch();
	static bool isYawingLeft( );
	static bool isYawingRight( );
	static bool isRollingLeft( );
	static bool isRollingRight( );

private:
	static ::Oyster::Math::Float elapsedUpdatePeriod;

	static void ResolvePitchAndYaw(TailCamera &T, float DeltaTime, Network::ClientToServerUpdateData& data);
};

#endif