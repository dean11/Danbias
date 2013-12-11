#include "Core.h"

namespace Oyster
{
	namespace Graphics
	{
		Core::Init::State Core::Init::CreateDeviceAndDeviceContext(bool SingleThreaded,bool Reference,bool ForceDX11)
		{
			UINT createDeviceFlags = 0;

			if(Core::deviceContext)
			{
				Core::deviceContext->Release();
				delete Core::deviceContext;
			}
			if(Core::device)
			{
				Core::device->Release();
				delete Core::device;
			}
			

			if( SingleThreaded )
				createDeviceFlags = ::D3D11_CREATE_DEVICE_SINGLETHREADED;

			::D3D_DRIVER_TYPE driverType = ::D3D_DRIVER_TYPE_HARDWARE;
		
			if(Reference)
				driverType = D3D_DRIVER_TYPE_REFERENCE;
	
			#if defined(DEBUG) || defined(_DEBUG)
			log << "DirectX running in debug mode.\n";
				createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
			#endif
			
			createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;

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
												&Core::device, // device
												&initiatedFeatureLevel,
												&Core::deviceContext ) ) ) // context
			{ // if failed
				if( Core::deviceContext ) { Core::deviceContext->Release(); Core::deviceContext = NULL; } // safe cleanup
				if( Core::device ) { Core::device->Release(); Core::device = NULL; } // safe cleanup
			}

			if( driverType == ::D3D_DRIVER_TYPE_HARDWARE )
				log << "D3D_DRIVER_TYPE_HARDWARE support discovered.\n";
			else
				log << "D3D_DRIVER_TYPE_REFERENCE support discovered.\n";

			if( initiatedFeatureLevel == ::D3D_FEATURE_LEVEL_11_0 )
			{
				log << "DirectX Featurelevel 11.0  supported.\n";
			}
			else
			{
				if(ForceDX11)
					return Init::Fail;
				if( initiatedFeatureLevel == ::D3D_FEATURE_LEVEL_10_1 )
				{
					log << "DirectX Featurelevel 10.1  supported.\n";
				}
				else
				{
					if( initiatedFeatureLevel == ::D3D_FEATURE_LEVEL_10_0 )
					{
						log << "DirectX Featurelevel 10.0  supported.\n";
					}
				}
			}
			if(Core::device)
				return Init::Sucsess;

			return Init::Fail;
		}

		Core::Init::State Core::Init::CreateSwapChain(HWND Window, int NrofBuffers,bool MSAA_Quality,bool Fullscreen, Oyster::Math::Float2 Size)
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

			desc.BufferDesc.Height = (UINT)Size.y;
			desc.BufferDesc.Width = (UINT)Size.x;

			if(Core::swapChain)
			{
				Core::swapChain->Release();
				delete Core::swapChain;
			}

	
			//Check and Set multiSampling
			if(MSAA_Quality)
			{
				if(FAILED(Core::device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM,4,&desc.SampleDesc.Quality)))
				{
					log<< "Failed to check multisample quality levels (MSAAQuality).\n";
					return Init::Fail;
				}
				desc.SampleDesc.Count=4;
				--desc.SampleDesc.Quality;
				log << "Supported multisample quality levels (MSAAQuality): " << desc.SampleDesc.Quality+1 << "x\n";
			}
			else
			{
				desc.SampleDesc.Count=1;
				desc.SampleDesc.Quality=0;
			}

			//Get Device Factory
			::IDXGIDevice *dxgiDevice = NULL;
			if( FAILED( Core::device->QueryInterface( __uuidof( IDXGIDevice ), (void**)&dxgiDevice ) ) )
			{
				log << "Failed to Query for the GPU's dxgiDevice.\nFailed to create swapChain for the GPU.\n";
				return Init::Fail;
			}

			::IDXGIAdapter *dxgiAdapter = NULL;
			if( FAILED( dxgiDevice->GetParent( __uuidof( IDXGIAdapter ), (void**)&dxgiAdapter ) ) )
			{
				dxgiDevice->Release();
				log << "Failed to get GPU's parent dxgiAdapter.\nFailed to create swapChain for the GPU.\n";
				return Init::Fail;
			}
			dxgiDevice->Release();

			::IDXGIFactory *dxgiFactory = NULL;
			if( FAILED( dxgiAdapter->GetParent( __uuidof( IDXGIFactory ), (void**)&dxgiFactory ) ) )
			{
				dxgiAdapter->Release();
				log << "Failed to get GPU's parent dxgiFactory.\nFailed to create swapChain for the GPU.\n";
				return Init::Fail;
			}
			dxgiAdapter->Release();
	
			//Create SwapChain
			if( FAILED( dxgiFactory->CreateSwapChain( Core::device, &desc, &Core::swapChain ) ) )
			{
				dxgiFactory->Release();
				log << "Failed to create swapChain for the GPU.\n";
				return Init::Fail;
			}

			dxgiFactory->Release();

			return Init::Sucsess;
		}

		Core::Init::State Core::Init::CreateDepthStencil(bool MSAA_Quality, Oyster::Math::Float2 Size)
		{
			D3D11_TEXTURE2D_DESC desc;
			desc.MipLevels=1;
			desc.ArraySize=1;
			desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			desc.CPUAccessFlags=0;
			desc.MiscFlags=0;
			desc.Height = (UINT)Size.y;
			desc.Width = (UINT)Size.x;

			if(Core::depthStencil)
			{
				Core::depthStencil->Release();
				delete Core::depthStencil;
			}

			//Check and Set multiSampling
			if(MSAA_Quality)
			{
				if(FAILED(Core::device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM,4,&desc.SampleDesc.Quality)))
				{
					log<< "Failed to check multisample quality levels (MSAAQuality).\n";
					return Init::Fail;
				}
				desc.SampleDesc.Count=4;
				--desc.SampleDesc.Quality;
				log << "Supported multisample quality levels (MSAAQuality): " << desc.SampleDesc.Quality+1 << "x\n";
			}
			else
			{
				desc.SampleDesc.Count=1;
				desc.SampleDesc.Quality=0;
			}

			ID3D11Texture2D* depthstencil;

			if(FAILED(Core::device->CreateTexture2D(&desc,0,&depthstencil)))
				return Init::Fail;
			if(FAILED(Core::device->CreateDepthStencilView(depthstencil,0,&Core::depthStencil)))
			{
				depthstencil->Release();
				return Init::Fail;
			}
			depthstencil->Release();
		
			return Init::Sucsess;
		}

		Core::Init::State Core::Init::CreateBackBufferViews()
		{
			D3D11_UNORDERED_ACCESS_VIEW_DESC descView;
			ZeroMemory( &descView, sizeof(descView) );
			descView.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
			descView.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			descView.Texture2D.MipSlice=0;

			ID3D11Texture2D* backBuffer;
			if(FAILED(Core::swapChain->GetBuffer(0,__uuidof(ID3D11Texture2D),reinterpret_cast<void**>(&backBuffer))))
			{
				log << "Failed to get BackBuffer from Swapchain";
				return Init::Fail;
			}
			if(Core::backBufferRTV)
			{
				Core::backBufferRTV->Release();
				delete Core::backBufferRTV;
			}
			if(FAILED(Core::device->CreateRenderTargetView(backBuffer,0,&Core::backBufferRTV)))
			{
				log << "Failed to create RTV for BackBuffer";
				backBuffer->Release();
				return Init::Fail;
			}
			if(Core::backBufferUAV)
			{
				Core::backBufferUAV->Release();
				delete Core::backBufferUAV;
			}
			if(FAILED(Core::device->CreateUnorderedAccessView(backBuffer,0,&Core::backBufferUAV)))
			{
				log << "Failed to create UAV for BackBuffer";
				backBuffer->Release();
				return Init::Fail;
			}

			backBuffer->Release();

			return Init::Sucsess;
		}

		Core::Init::State Core::Init::CreateViewPort(Oyster::Math::Float2 Origin, Oyster::Math::Float2 Size)
		{
			if(Core::viewPort)
				delete Core::viewPort;
			Core::viewPort = new D3D11_VIEWPORT;

			Core::viewPort->TopLeftX = Origin.x;
			Core::viewPort->TopLeftY = Origin.y;
			Core::viewPort->Width = Size.x;
			Core::viewPort->Height = Size.y;
			Core::viewPort->MinDepth = 0.0f;
			Core::viewPort->MaxDepth = 1.0f;

			return Init::Sucsess;
		}

		Core::Init::State Core::Init::FullInit(HWND Window, bool MSAA_Quality, bool Fullscreen)
		{
			if(Init::CreateDeviceAndDeviceContext() == Init::Fail)
			{
				return Init::Fail;
			}

			if(Init::CreateSwapChain(Window, 1, MSAA_Quality, Fullscreen, Core::resolution) == Init::Fail)
			{
				return Init::Fail;
			}

			if(Init::CreateDepthStencil(MSAA_Quality, Core::resolution) == Init::Fail)
			{
				return Init::Fail;
			}

			if(Init::CreateBackBufferViews() == Init::Fail)
			{
				return Init::Fail;
			}

			if(Init::CreateViewPort(Oyster::Math::Float2::null, Core::resolution) == Init::Fail)
			{
				return Init::Fail;
			}

			return Init::Sucsess;
		}

		Core::Init::State Core::Init::ReInitialize(HWND Window, bool MSAA_Quality, bool Fullscreen)
		{
			if(Init::CreateSwapChain(Window, 1, MSAA_Quality, Fullscreen, Core::resolution) == Init::Fail)
			{
				return Init::Fail;
			}

			if(Init::CreateDepthStencil(MSAA_Quality, Core::resolution) == Init::Fail)
			{
				return Init::Fail;
			}

			if(Init::CreateBackBufferViews() == Init::Fail)
			{
				return Init::Fail;
			}

			if(Init::CreateViewPort(Oyster::Math::Float2::null, Core::resolution) == Init::Fail)
			{
				return Init::Fail;
			}

			return Init::Sucsess;
		}
	}
}