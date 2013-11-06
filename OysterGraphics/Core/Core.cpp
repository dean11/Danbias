#include "Core.h"
#include "..\Window\Window.h"

using namespace Oyster;
using std::string;

//GPU
ID3D11Device *Core::Device = NULL;

//API
ID3D11DeviceContext *Core::DeviceContext = NULL;

//SwapChain
IDXGISwapChain* Core::SwapChain = NULL;

std::stringstream Log;

inline std::stringstream* AccesLog(){return &Log;}

bool Core::Init(bool SingleThreaded, bool Reference,bool ForceDX11)
{
	UINT createDeviceFlags = 0;

	if( SingleThreaded )
		createDeviceFlags = ::D3D11_CREATE_DEVICE_SINGLETHREADED;

	::D3D_DRIVER_TYPE driverType = ::D3D_DRIVER_TYPE_HARDWARE;
		
	if(Reference)
		driverType = D3D_DRIVER_TYPE_REFERENCE;
	
	/*#if defined(DEBUG) || defined(_DEBUG)
		Log << "DirectX running in debug mode.\n";
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif*/


	D3D_FEATURE_LEVEL featureLevelsToTry[] = 
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	D3D_FEATURE_LEVEL initiatedFeatureLevel;

	if( FAILED( ::D3D11CreateDevice( NULL, // default adapter
										driverType,
										NULL, // no software device
										createDeviceFlags,
										featureLevelsToTry, 3, // default feature level array. DX11 support assumed
										D3D11_SDK_VERSION,
										&Device, // device
										&initiatedFeatureLevel,
										&DeviceContext ) ) ) // context
	{ // if failed
		if( DeviceContext ) { DeviceContext->Release(); DeviceContext = NULL; } // safe cleanup
		if( Device ) { Device->Release(); Device = NULL; } // safe cleanup
	}

	if( driverType == ::D3D_DRIVER_TYPE_HARDWARE )
		Log << "D3D_DRIVER_TYPE_HARDWARE support discovered.\n";
	else
		Log << "D3D_DRIVER_TYPE_REFERENCE support discovered.\n";

	if( initiatedFeatureLevel == ::D3D_FEATURE_LEVEL_11_0 )
	{
		Log << "DirectX Featurelevel 11.0  supported.\n";
	}
	else
	{
		if(ForceDX11)
			return false;
		if( initiatedFeatureLevel == ::D3D_FEATURE_LEVEL_10_1 )
		{
			Log << "DirectX Featurelevel 10.1  supported.\n";
		}
		else
		{
			if( initiatedFeatureLevel == ::D3D_FEATURE_LEVEL_10_0 )
			{
				Log << "DirectX Featurelevel 10.0  supported.\n";
			}
		}
	}
	if(Device)
		return true;

	return false;
}

bool  Core::CreateSwapChain(HWND Window, int NrofBuffers,bool MSAA_Quality,bool Fullscreen)
{
	//generate static Swapchain Desc
	DXGI_SWAP_CHAIN_DESC desc;
	desc.OutputWindow=Window;
	desc.BufferCount=NrofBuffers;
	desc.Windowed=!Fullscreen;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_UNORDERED_ACCESS;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	desc.Flags=0;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;
	desc.BufferDesc.RefreshRate.Denominator=1;
	desc.BufferDesc.RefreshRate.Numerator=60;
	desc.BufferDesc.Height = Window::Size.bottom;
	desc.BufferDesc.Width = Window::Size.left;
	
	//Check and Set multiSampling
	if(MSAA_Quality)
	{
		if(FAILED(Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM,4,&desc.SampleDesc.Quality)))
		{
			Log<< "Failed to check multisample quality levels (MSAAQuality).\n";
			return false;
		}
		desc.SampleDesc.Count=4;
		--desc.SampleDesc.Quality;
		Log << "Supported multisample quality levels (MSAAQuality): " << desc.SampleDesc.Quality+1 << "x\n";
	}
	else
	{
		desc.SampleDesc.Count=1;
		desc.SampleDesc.Quality=0;
	}

	//Get Device Factory
	::IDXGIDevice *dxgiDevice = NULL;
	if( FAILED( Device->QueryInterface( __uuidof( IDXGIDevice ), (void**)&dxgiDevice ) ) )
	{
		Log << "Failed to Query for the GPU's dxgiDevice.\nFailed to create swapChain for the GPU.\n";
		return false;
	}

	::IDXGIAdapter *dxgiAdapter = NULL;
	if( FAILED( dxgiDevice->GetParent( __uuidof( IDXGIAdapter ), (void**)&dxgiAdapter ) ) )
	{
		dxgiDevice->Release();
		Log << "Failed to get GPU's parent dxgiAdapter.\nFailed to create swapChain for the GPU.\n";
		return false;
	}
	dxgiDevice->Release();

	::IDXGIFactory *dxgiFactory = NULL;
	if( FAILED( dxgiAdapter->GetParent( __uuidof( IDXGIFactory ), (void**)&dxgiFactory ) ) )
	{
		dxgiAdapter->Release();
		Log << "Failed to get GPU's parent dxgiFactory.\nFailed to create swapChain for the GPU.\n";
		return false;
	}
	dxgiAdapter->Release();
	
	//Create SwapChain
	if( FAILED( dxgiFactory->CreateSwapChain( Device, &desc, &SwapChain ) ) )
	{
		dxgiFactory->Release();
		Log << "Failed to create swapChain for the GPU.\n";
		return false;
	}

	dxgiFactory->Release();

	return true;
}