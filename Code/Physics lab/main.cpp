#define NOMINMAX
#include "WinTimer.h"
#include "WindowShell.h"
#include "GFXAPI.h"
#include "PhysicsAPI.h"

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

int WINAPI WinMain( HINSTANCE thisInstance, HINSTANCE prevInst, PSTR cmdLine, int cmdShow)
{
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
		crate[1].phys = Physics::API::Instance().CreateRigidBody( descCrate ).Release();

		for( unsigned int i = 0; i < StaticArray::NumElementsOf(crate); ++i )
		{
			crate[i].gfx = Graphics::API::CreateModel( L"crate" );
			Physics::API::Instance().AddObject( crate[i].phys );
		}

		Physics::ICustomBody::State state;
		sphere.phys->GetState( state );
		state.SetMass_KeepMomentum( 10.0f );
		state.SetMomentOfInertia_KeepMomentum( Physics::Formula::MomentOfInertia::CreateSphereMatrix(10.0f, 0.5f) );
		sphere.phys->SetState( state );

		//crate[0].phys->GetState( state );
		//state.SetLinearMomentum( Float4(5.0f, 0.0f, 0.0f, 0.0f) );
		//crate[0].phys->SetState( state );

		crate[1].phys->GetState( state );
		state.SetLinearMomentum( Float4(0.0f, 5.0f, 0.0f, 0.0f) );
		crate[1].phys->SetState( state );
	}

	timer.reset();
	while( true )
	{
		if( screen.Frame() )
		{
			graphicsTimeAccumulation += (Float)timer.getElapsedSeconds();
			physicsTimeAccumulation += (Float)timer.getElapsedSeconds();
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

			if( graphicsTimeAccumulation >= graphicsPeriodicy )
			{
				Graphics::API::SetView( ViewMatrix_LookAtPos(Float3::null, Float3::standard_unit_y, light.Pos) );
				Graphics::API::NewFrame();
				{
					//sphere.gfx->WorldMatrix = sphere.phys->GetState().GetOrientation();
					//Graphics::API::RenderModel( *sphere.gfx );

					for( unsigned int i = 0; i < StaticArray::NumElementsOf(crate); ++i )
					{
						crate[i].gfx->WorldMatrix = crate[i].phys->GetState().GetOrientation();
						Graphics::API::RenderModel( *crate[i].gfx );
					}
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