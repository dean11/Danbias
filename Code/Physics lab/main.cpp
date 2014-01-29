#define NOMINMAX
#include "WinTimer.h"
#include "WindowShell.h"
#include "GFXAPI.h"
#include "PhysicsAPI.h"

#include <cstdio>

using namespace ::Utility;
using namespace ::Oyster;
using namespace ::Oyster::Math3D;

struct Object
{
	Graphics::Model::Model *gfx;
	Physics::ICustomBody *phys;

	Object()
	{
		this->gfx = nullptr;
		this->phys = nullptr;
	}

	~Object()
	{
		DynamicMemory::SafeDeleteInstance( this->gfx );
	}
};

// typedef LRESULT (CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK MessagePipe( HWND, UINT, WPARAM, LPARAM );

void RedirectIOToConsole(); // copy pasted from http://www.cplusplus.com/forum/windows/58206/

void OnCollisionResponse( const Physics::ICustomBody *proto, const Physics::ICustomBody *deuter, Float kineticEnergyLoss );

int WINAPI WinMain( HINSTANCE thisInstance, HINSTANCE prevInst, PSTR cmdLine, int cmdShow)
{
	// NON-GRAPHICAL TESTS /////////////////////////////////////////////

	Float4x4 rotA, rotB;
	Float4 normal = Float4( 0.0f, 1.0f, 0.0f, 0.0f ).Normalize();
	RotationMatrix_AxisX( Value::Radian(45.0f), rotA );

	for( Float t = 0.0f; t < 1.05f; t += 0.1f )
	{
		rotB = rotA;
		InterpolateAxisYToNormal_UsingNlerp( rotB, normal, t );
		const char* breakpoint = "";
	}

	// END NON-GRAPHICAL TESTS /////////////////////////////////////////

	if( SetDllDirectoryW(L"..\\DLL") == FALSE )
	{
		return 0;
	}

	WinTimer timer;
	WindowShell screen;
	{
		WindowShell::WINDOW_INIT_DESC desc;
		desc.parent				= nullptr;
		desc.hInstance			= thisInstance;
		desc.windowProcCallback	= MessagePipe;
		desc.windowName			= L"Physics Testing Enviroment";
		desc.windowSize.x		= 1280;
		desc.windowSize.y		= 720;
		desc.windowPosition.x	= 0;
		desc.windowPosition.y	= 0;
		desc.windowClassStyle	= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		desc.windowStyle		= WS_POPUPWINDOW|WS_SYSMENU|WS_CAPTION;
		desc.icon				= LoadIcon(0, IDI_APPLICATION);
		desc.cursor				= LoadCursor(NULL, IDC_ARROW);
		desc.background			= (HBRUSH)GetStockObject(BLACK_BRUSH);

		if( !screen.CreateWin(desc) )
		{
			return 0;
		}
	}

	RedirectIOToConsole();
	
	const Float graphicsPeriodicy = 1.0f / 60.0f;
	Float graphicsTimeAccumulation = 0.0f;
	if( Graphics::API::Init(screen.GetHWND(), false, false, Float2(1280.0f, 720.0f)) == Graphics::API::Fail )
	{
		return 0;
	}
	Graphics::API::Option settings;
	//settings.
	Graphics::API::SetOptions( settings );
	Graphics::API::SetProjection( ProjectionMatrix_Perspective( Value::Radian(100.0f), 1280.0f/720.0f, 0.01f, 1000.0f) );

	Graphics::Definitions::Pointlight light;
	{
		light.Pos = Float3(0.0f, 5.0f, 5.0f);
		light.Radius = 100.0f;
		light.Color = Float3( 1.0f );
		light.Bright = 5.0f;
	}
	Graphics::API::AddLight( light );

	const Float physicsPeriodicy = 1.0f / 120.0f;
	Float physicsTimeAccumulation = 0.0f;
	Physics::API::Instance().Init( 27, 1, Float3(1024.0f) );
	Physics::API::Instance().SetFrameTimeLength( physicsPeriodicy );

	//Object sphere;
	Object crate[2];
	{
		//sphere.gfx = Graphics::API::CreateModel( L"worldDummy" );
		//sphere.phys = Physics::API::Instance().CreateRigidBody( Physics::API::SphericalBodyDescription() ).Release();
		//Physics::API::Instance().AddObject( sphere.phys );

		Physics::API::SimpleBodyDescription descCrate;

//		descCrate.centerPosition = Float4( -2.5f, 0.0f, 0.0f, 1.0f );
		descCrate.centerPosition = Float4( 0.9f, 1.5f, 0.9f, 1.0f );
		crate[0].phys = Physics::API::Instance().CreateRigidBody( descCrate ).Release();

//		descCrate.centerPosition = Float4( 2.5, -0.5f, -0.5f, 1.0f );
		descCrate.centerPosition = Float4::standard_unit_w;
		descCrate.subscription_onCollisionResponse = OnCollisionResponse;
		crate[1].phys = Physics::API::Instance().CreateRigidBody( descCrate ).Release();

		for( unsigned int i = 0; i < StaticArray::NumElementsOf(crate); ++i )
		{
			crate[i].gfx = Graphics::API::CreateModel( L"box.dan" );
			Physics::API::Instance().AddObject( crate[i].phys );
		}

		Physics::ICustomBody::State state;
//		sphere.phys->GetState( state );
//		state.SetMass_KeepMomentum( 10.0f );
//		state.SetMomentOfInertia_KeepMomentum( Physics::Formula::MomentOfInertia::CreateSphereMatrix(10.0f, 0.5f) );
//		sphere.phys->SetState( state );

		//crate[0].phys->GetState( state );
		//state.SetLinearMomentum( Float4(5.0f, 0.0f, 0.0f, 0.0f) );
		//crate[0].phys->SetState( state );

		crate[1].phys->GetState( state );
		state.SetLinearMomentum( Float3(0.0f, 5.0f, 0.0f) );
		crate[1].phys->SetState( state );
	}


	Graphics::Definitions::Pointlight interpolationLight;
	Graphics::Model::Model* interpolationBox;
	Float4x4 interpolationStart;
	Float4 interpolationNormal = Float4( 1.0f, 0.0f, 1.0f, 0.0f ).Normalize();
	Float interpolationStep = 0.0f;
	{ // interpolation stuff
		interpolationBox = Graphics::API::CreateModel( L"box.dan" );
	
		OrientationMatrix( Rotation(Value::Radian(0.0f), Float4(1.0f, 0.0f, 0.0f, 0.0f).Normalize().xyz), Float3(200.0f, 0.0f, 0.0f), interpolationStart );

		interpolationLight.Pos = Float3( 200.0f, 0.0f, 2.0f );
		interpolationLight.Radius = 100.0f;
		interpolationLight.Color = Float3( 1.0f );
		interpolationLight.Bright = 5.0f;
		Graphics::API::AddLight( interpolationLight );
	}

	timer.reset();
	while( true )
	{
		if( screen.Frame() )
		{
			Float deltaTime = (Float)timer.getElapsedSeconds();
			graphicsTimeAccumulation += deltaTime;
			physicsTimeAccumulation += deltaTime;
			timer.reset();

			while( physicsTimeAccumulation >= physicsPeriodicy )
			{
				Physics::API::Instance().Update();
				physicsTimeAccumulation -= physicsPeriodicy;
			}

			//Physics::ICustomBody::State state;
			//crate[0].phys->GetState( state );
			//state.SetLinearMomentum( Float4(5.0f, 0.0f, 0.0f, 0.0f) );
			//crate[0].phys->SetState( state );

			// update interpolation test
			if( interpolationStep < 1.00001f )
			{
				interpolationStep += 0.1f * deltaTime;
				interpolationBox->WorldMatrix = interpolationStart;
				InterpolateAxisYToNormal_UsingNlerp( interpolationBox->WorldMatrix, interpolationNormal, interpolationStep );				
			}
			// end update interpolation test

			if( graphicsTimeAccumulation >= graphicsPeriodicy )
			{
				Graphics::API::SetView( ViewMatrix_LookAtPos(Float3::null, Float3::standard_unit_y, light.Pos) ); // for collision response test
				//Graphics::API::SetView( ViewMatrix_LookAtPos(Float3(interpolationLight.Pos.x, 0.0f, 0.0f), Float3::standard_unit_y, interpolationLight.Pos) ); // for interpolation test
				Graphics::API::NewFrame();
				{
					// render collision response test

					//sphere.gfx->WorldMatrix = sphere.phys->GetState().GetOrientation();
					//Graphics::API::RenderModel( *sphere.gfx );

					for( unsigned int i = 0; i < StaticArray::NumElementsOf(crate); ++i )
					{
						crate[i].gfx->WorldMatrix = crate[i].phys->GetState().GetOrientation();
						Graphics::API::RenderModel( *crate[i].gfx );
					}

					// end render collision response test

					// render interpolation test
					Graphics::API::RenderModel( *interpolationBox );
					// end render interpolation test
				}
				Graphics::API::EndFrame();
				graphicsTimeAccumulation = 0.0f;
			}
		}
		else break;
	}

	return 0;
}

LRESULT CALLBACK MessagePipe( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message) 
	{
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void OnCollisionResponse( const Physics::ICustomBody *proto, const Physics::ICustomBody *deuter, Float kineticEnergyLoss )
{
	static int count = 0;
	++count;
	printf( "OnCollisionResponse::Count %d\n", count );
	
	Physics::ICustomBody::State state;
	proto->GetState( state );
	printf( "\tproto::Momentum(Linear, Angular)  %f %f\n", state.GetLinearMomentum().GetMagnitude(), state.GetAngularMomentum().GetMagnitude() );
	Float4 w = Float4::null;
	state.GetMomentOfInertia().CalculateAngularVelocity( Rotation(state.GetAngularAxis()), state.GetAngularMomentum(), w.xyz );	
	printf( "\tproto::Velocity(Linear, Angular)  %f %f\n", state.GetLinearMomentum().GetMagnitude() / state.GetMass(), w.GetMagnitude() );
	
	Float4 sumLinear = Float4( state.GetLinearMomentum(), 0.0f ),
		   sumAngular = Float4( state.GetAngularMomentum(), 0.0f );

	deuter->GetState( state );
	printf( "\tdeuter::Momentum(Linear, Angular) %f %f\n", state.GetLinearMomentum().GetMagnitude(), state.GetAngularMomentum().GetMagnitude() );
	state.GetMomentOfInertia().CalculateAngularVelocity( Rotation(state.GetAngularAxis()), state.GetAngularMomentum(), w.xyz );	
	printf( "\tdeuter::Velocity(Linear, Angular) %f %f\n", state.GetLinearMomentum().GetMagnitude() / state.GetMass(), w.GetMagnitude() );

	sumLinear += Float4( state.GetLinearMomentum(), 0.0f );
	sumAngular -= Float4( state.GetAngularMomentum(), 0.0f );

	printf( "\tSumMomentum(Linear, Angular) %f %f\n", sumLinear.GetMagnitude(), sumAngular.GetMagnitude() );
}

// copy pasted from http://www.cplusplus.com/forum/windows/58206/
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
void RedirectIOToConsole()
{
	// maximum mumber of lines the output console should have
	static const WORD MAX_CONSOLE_LINES = 500;

    int hConHandle;
    long lStdHandle;
    CONSOLE_SCREEN_BUFFER_INFO coninfo;
    FILE *fp;

	// allocate a console for this app
    AllocConsole();

	// set the screen buffer to be big enough to let us scroll text
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&coninfo);
    coninfo.dwSize.Y = MAX_CONSOLE_LINES;
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE),coninfo.dwSize);

	// redirect unbuffered STDOUT to the console
    lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

    fp = _fdopen( hConHandle, "w" );

    *stdout = *fp;

    setvbuf( stdout, NULL, _IONBF, 0 );

	// redirect unbuffered STDIN to the console

    lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

    fp = _fdopen( hConHandle, "r" );
    *stdin = *fp;
    setvbuf( stdin, NULL, _IONBF, 0 );

	// redirect unbuffered STDERR to the console
    lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

    fp = _fdopen( hConHandle, "w" );

    *stderr = *fp;

    setvbuf( stderr, NULL, _IONBF, 0 );

	// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
	// point to console as well
    ::std::ios::sync_with_stdio();
}