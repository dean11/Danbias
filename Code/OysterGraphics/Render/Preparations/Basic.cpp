#include "Preparations.h"

namespace Oyster
{
	namespace Graphics
	{
		namespace Render
		{
			namespace Preparations
			{
				void Basic::BindBackBufferRTV(bool DepthStencil)
				{
					if(DepthStencil)
					{
						Core::deviceContext->OMSetRenderTargets(1,&Core::backBufferRTV,Core::depthStencil);
					}
					else
					{
						Core::deviceContext->OMSetRenderTargets(1,&Core::backBufferRTV,NULL);
					}
				}

				void Basic::BindBackBufferRTV(ID3D11DepthStencilView& depthStencil)
				{
					Core::deviceContext->OMSetRenderTargets(1,&Core::backBufferRTV,&depthStencil);
				}

				void Basic::BindBackBufferUAV()
				{
					Core::deviceContext->CSSetUnorderedAccessViews(0,1,&Core::backBufferUAV,0);
				}

				void Basic::BindRTV(ID3D11RenderTargetView* RTVs[], int size, bool UseDepthStencil)
				{
					if(UseDepthStencil)
					{
						BindRTV(RTVs, size, Core::depthStencil);
					}
					else
					{
						Core::deviceContext->OMSetRenderTargets(size,RTVs,NULL);
					}
				}

				void Basic::BindRTV(ID3D11RenderTargetView* RTVs[], int size,ID3D11DepthStencilView& depthStencil)
				{
					Core::deviceContext->OMSetRenderTargets(size,RTVs,&depthStencil);
				}

				void Basic::BindUAV(ID3D11UnorderedAccessView* UAVs[], int size)
				{
					Core::deviceContext->CSSetUnorderedAccessViews(0,size,UAVs,0);
				}

				void Basic::ClearBackBuffer(Oyster::Math::Float4 Color, bool ClearDefaultDepthStencil)
				{
					Core::deviceContext->ClearRenderTargetView(Core::backBufferRTV,Color);
					if(ClearDefaultDepthStencil)
					{
						Core::deviceContext->ClearDepthStencilView(Core::depthStencil,1,1,0);
					}
				}

				void Basic::ClearRTV(ID3D11RenderTargetView* RTVs[], int size,Oyster::Math::Float4 Color)
				{
					for(int i = 0; i < size; ++i)
					{
						Core::deviceContext->ClearRenderTargetView(RTVs[i],Color);
					}
				}

				void Basic::ClearDepthStencil(ID3D11DepthStencilView &depthStencil)
				{
					Core::deviceContext->ClearDepthStencilView(&depthStencil,1,1,0);
				}

				void Basic::SetViewPort()
				{
					Core::deviceContext->RSSetViewports(1,Core::viewPort);
				}
			}
		}
	}
}