#include "UserInterface.h"
#include "IntoTheVoid.h"
#include "Player.h"
#include "Input\InputController.h"
float MoveSpeed = 2.0f;
::Oyster::Math::Float UserInterface::elapsedUpdatePeriod = 0.0f;

UserInterface::InitResult UserInterface::init( )
{ 
	Oyster::Input::Controller::RestrictMouse();
	Oyster::Input::Controller::OrigoCenter();
	return UserInterface::Success; 
}

UserInterface::UpdateResult UserInterface::MessageLoop( )
{
	MSG message;

	if( PeekMessage( &message, NULL, NULL, NULL, PM_REMOVE ) )
	{
		TranslateMessage( &message );
		DispatchMessage( &message );

		if( message.message == WM_QUIT )
			return UserInterface::ShutDown;

		return UserInterface::Updated;
	}

	return UserInterface::Idle;
}

void UserInterface::Update(TailCamera &T, float DeltaTime, Network::ClientToServerUpdateData& update)
{
	if(Oyster::Input::Controller::isMouseBtnDown(MK_LBUTTON))
		update.firePrim=true;

	if( UserInterface::isMovingForward() )
	{
		T.panBackward( 0.5f * MoveSpeed * DeltaTime );
		++update.forward;
	}
	else if( UserInterface::isMovingBackward() )
	{
		T.panForward( 0.5f * MoveSpeed * DeltaTime );
		--update.forward;
	}
	else T.stabilisePanZ( 0.5f * MoveSpeed * DeltaTime );

	if( UserInterface::isStrafingLeft() )
	{
		T.panRight( 0.5f * MoveSpeed * DeltaTime );
		--update.straferight;
	}
	else if( UserInterface::isStrafingRight() )
	{
		T.panLeft( 0.5f * MoveSpeed * DeltaTime );
		++update.straferight;
	}
	else 	T.stabilisePanX( 0.5f * MoveSpeed * DeltaTime );

	if( UserInterface::isStrafingUp() )
	{
		T.panDown( 0.5f * MoveSpeed * DeltaTime );
		++update.strafeup;
	}
	else if( UserInterface::isStrafingDown() )
	{
		T.panUp( 0.5f * MoveSpeed * DeltaTime );
		--update.strafeup;
	}
	else T.stabilisePanY( 0.5f * MoveSpeed * DeltaTime );

	if( UserInterface::isRollingLeft() )
	{
		T.revolveRollRight( 0.5f * DeltaTime );
		++update.roll;
	}
	else if( UserInterface::isRollingRight() )
	{
		T.revolveRollLeft( 0.5f * DeltaTime );
		--update.roll;
	}
	else T.stabiliseRoll( 0.5f * DeltaTime );

	if(Oyster::Input::Controller::isKeyDown(' '))
	{
		update.braking=true;
	}
	else
	{
	}
	ResolvePitchAndYaw(T,Oyster::Game::MoveAble::getDiscreteTimeSlice(),update);
}

LRESULT CALLBACK UserInterface::wndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	/* TODO */

	if( message == WM_DESTROY )
	{
		PostQuitMessage( 0 );
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool UserInterface::isMovingForward( )	{return Oyster::Input::Controller::isKeyDown('W');}
bool UserInterface::isMovingBackward( )	{return Oyster::Input::Controller::isKeyDown('S');}
bool UserInterface::isStrafingLeft( )	{return Oyster::Input::Controller::isKeyDown('A');}
bool UserInterface::isStrafingRight( )	{return Oyster::Input::Controller::isKeyDown('D');}
bool UserInterface::isStrafingUp( )		{return Oyster::Input::Controller::isKeyDown('R');}
bool UserInterface::isStrafingDown( )	{return Oyster::Input::Controller::isKeyDown('F');}
bool UserInterface::isPitchingUp( )		{return Oyster::Input::Controller::GetAnalogY()<-0.05f;}
bool UserInterface::isPitchingDown( )	{return Oyster::Input::Controller::GetAnalogY()>0.05f;}
bool UserInterface::isYawingLeft( )		{return Oyster::Input::Controller::GetAnalogX()<-0.05f;}
bool UserInterface::isYawingRight( )	{return Oyster::Input::Controller::GetAnalogX()>0.05f;}
bool UserInterface::isRollingLeft( )	{return Oyster::Input::Controller::isKeyDown('Q');}
bool UserInterface::isRollingRight( )	{return Oyster::Input::Controller::isKeyDown('E');}

void  UserInterface::ResolvePitchAndYaw(TailCamera &T, float DeltaTime, Network::ClientToServerUpdateData& data)
{
	
	T.setVerticalRevolution(-Oyster::Input::Controller::GetAnalogY() * 0.3f);
	if( UserInterface::isPitchingUp() || UserInterface::isPitchingDown())
	{
		
		data.TurnVer=Oyster::Input::Controller::GetAnalogY();
		//::Oyster::Math::Float baseAcceleration = s.rotationProperty.acceleration.pitch;
		//s.rotationProperty.acceleration.pitch *= -Oyster::Input::Controller::GetAnalogY() / ::Oyster::Game::MoveAble::getDiscreteTimeSlice();
		//s.pitchUp( );
		//s.disableRotationReduction();
		//s.rotationProperty.acceleration.pitch = baseAcceleration;

	}
	/*else if( UserInterface::isPitchingDown() )
	{
		::Oyster::Math::Float baseAcceleration = s.rotationProperty.acceleration.pitch;
		s.rotationProperty.acceleration.pitch *= Oyster::Input::Controller::GetAnalogY() / ::Oyster::Game::MoveAble::getDiscreteTimeSlice();
		s.pitchDown( );
		s.disableRotationReduction();
		s.rotationProperty.acceleration.pitch = baseAcceleration;

	}*/
	
	
	T.setHorizontalRevolution(-Oyster::Input::Controller::GetAnalogX()*0.3f);
	if( UserInterface::isYawingLeft() || UserInterface::isYawingRight() )
	{
		data.TurnHor= Oyster::Input::Controller::GetAnalogX();
		/*::Oyster::Math::Float baseAcceleration = s.rotationProperty.acceleration.yaw;
		s.rotationProperty.acceleration.yaw *= -Oyster::Input::Controller::GetAnalogX() / ::Oyster::Game::MoveAble::getDiscreteTimeSlice();
		s.yawLeft( );
		s.disableRotationReduction();
		s.rotationProperty.acceleration.yaw = baseAcceleration;*/

	}
	/*else if( UserInterface::isYawingRight() )
	{
		::Oyster::Math::Float baseAcceleration = s.rotationProperty.acceleration.yaw;
		s.rotationProperty.acceleration.yaw *= Oyster::Input::Controller::GetAnalogX() / ::Oyster::Game::MoveAble::getDiscreteTimeSlice();
		s.yawRight( );
		s.disableRotationReduction();
		s.rotationProperty.acceleration.yaw = baseAcceleration;

	}*/
}
