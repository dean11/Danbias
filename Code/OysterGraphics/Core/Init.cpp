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
			
			//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;

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
				return Init::Success;

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
			desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
			desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			desc.BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
			desc.BufferDesc.RefreshRate.Denominator=1;
			desc.BufferDesc.RefreshRate.Numerator=60;

			desc.BufferDesc.Height = (UINT)Size.y;
			desc.BufferDesc.Width = (UINT)Size.x;

			if(Core::swapChain)
			{
				Core::swapChain->Release();
				Core::UsedMem -= Core::resolution.x * Core::resolution.y * 4;
				//delete Core::swapChain;
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
			Core::UsedMem += desc.BufferDesc.Height * desc.BufferDesc.Width * 4;
			return Init::Success;
		}

		Core::Init::State Core::Init::CreateDepthStencil(bool MSAA_Quality, Oyster::Math::Float2 Size)
		{
			D3D11_TEXTURE2D_DESC desc;
			desc.MipLevels=1;
			desc.ArraySize=1;
			desc.Format = DXGI_FORMAT_R32_TYPELESS;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags=0;
			desc.MiscFlags=0;
			desc.Height = (UINT)Size.y;
			desc.Width = (UINT)Size.x;

			if(Core::depthStencil)
			{
				Core::depthStencil->Release();
				Core::depthStencilUAV->Release();
				Core::UsedMem -= Core::resolution.x * Core::resolution.y * 4;
				//delete Core::depthStencil;
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
			{
				return Init::Fail;
			}
			Core::UsedMem += desc.Height * desc.Width * 4;
			D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
			dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
			dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			dsvDesc.Flags = 0;
			dsvDesc.Texture2D.MipSlice = 0;
			if(Core::device->CreateDepthStencilView(depthstencil,&dsvDesc,&Core::depthStencil) != S_OK)
			{
				depthstencil->Release();
				return Init::Fail;
			}
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;
			srvDesc.Texture2D.MostDetailedMip = 0;
			if(FAILED(Core::device->CreateShaderResourceView(depthstencil,&srvDesc,&Core::depthStencilUAV)))
			{
				Core::depthStencil->Release();
				depthStencil->Release();
				return Init::Fail;
			}
			depthstencil->Release();
		
			return Init::Success;
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
				//delete Core::backBufferRTV;
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
				//delete Core::backBufferUAV;
			}
			if(FAILED(Core::device->CreateUnorderedAccessView(backBuffer,0,&Core::backBufferUAV)))
			{
				log << "Failed to create UAV for BackBuffer";
				backBuffer->Release();
				return Init::Fail;
			}

			backBuffer->Release();

			return Init::Success;
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

			return Init::Success;
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

			
			Core::window = Window;
			Core::fullscreen = Fullscreen;

			return Init::Success;
		}

		Core::Init::State Core::Init::ReInitialize(bool MSAA_Quality, bool Fullscreen, Math::Float2 Size)
		{
			if(Init::CreateSwapChain(Core::window, 1, MSAA_Quality, Fullscreen, Size) == Init::Fail)
			{
				return Init::Fail;
			}

			if(Init::CreateDepthStencil(MSAA_Quality, Size) == Init::Fail)
			{
				return Init::Fail;
			}

			if(Init::CreateBackBufferViews() == Init::Fail)
			{
				return Init::Fail;
			}

			if(Init::CreateDepthStencil(MSAA_Quality, Size) == Init::Fail)
			{
				return Init::Fail;
			}

			if(Init::CreateViewPort(Oyster::Math::Float2::null, Size) == Init::Fail)
			{
				return Init::Fail;
			}

			return Init::Success;
		}

		Core::Init::State Core::Init::CreateLinkedShaderResourceFromTexture(ID3D11RenderTargetView** rtv, ID3D11ShaderResourceView** srv, ID3D11UnorderedAccessView** uav)
		{
			ID3D11Texture2D* tex;
			D3D11_TEXTURE2D_DESC texDesc;
			texDesc.Width = (UINT)Core::resolution.x;
			texDesc.Height = (UINT)Core::resolution.y;
			texDesc.MipLevels = 1;
			texDesc.ArraySize = 1;
			texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			texDesc.SampleDesc.Count = 1;
			texDesc.SampleDesc.Quality = 0;
			texDesc.Usage = D3D11_USAGE_DEFAULT;
			texDesc.CPUAccessFlags = 0;
			texDesc.MiscFlags = 0;
			texDesc.BindFlags = 0;
			if(rtv)
			{
				texDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
			}
			if(srv)
			{
				texDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
			}
			if(uav)
			{
				texDesc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
			}

			if(FAILED(Core::device->CreateTexture2D(&texDesc,NULL,&tex)))
				return State::Fail;

			Core::UsedMem += texDesc.Height*texDesc.Width*16;

			if(rtv)
			{
				D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
				rtvDesc.Format = texDesc.Format;
				rtvDesc.Texture2D.MipSlice = 0;
				rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

				Core::device->CreateRenderTargetView(tex, &rtvDesc, rtv);
			}
			if(srv)
			{
				D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
				srvDesc.Texture2D.MipLevels = 1;
				srvDesc.Texture2D.MostDetailedMip = 0;
				srvDesc.Format = texDesc.Format;
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

				Core::device->CreateShaderResourceView(tex,&srvDesc,srv);
			}
			if(uav)
			{
				D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
				uavDesc.Texture2D.MipSlice = 0;
				uavDesc.Format = texDesc.Format;
				uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;

				Core::device->CreateUnorderedAccessView(tex,&uavDesc,uav);
			}

			SAFE_RELEASE(tex);


			return State::Success;
		}

		Core::Init::State Core::Init::CreateLinkedShaderResourceFromStructuredBuffer(Buffer** Structured, ID3D11ShaderResourceView** srv, ID3D11UnorderedAccessView** uav)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC desc;
			desc.Buffer.FirstElement=0;
			desc.Buffer.ElementWidth = (*Structured)->GetElementCount();
			desc.Format = DXGI_FORMAT_UNKNOWN;
			desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;

			if(Core::device->CreateShaderResourceView(**(Structured),&desc, srv)==S_OK)
			{
				return State::Success;
			}

			return State::Fail;
		}
	}
}