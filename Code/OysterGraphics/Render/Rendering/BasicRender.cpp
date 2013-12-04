#include "Render.h"
#include "../Resources/Resources.h"
#include "../../Definitions/GraphicalDefinition.h"
#include "../../Model/ModelInfo.h"

namespace Oyster
{
	namespace Graphics
	{
		namespace Render
		{
			namespace Rendering
			{

				void Basic::NewFrame(Oyster::Math::Float4x4 View, Oyster::Math::Float4x4 Projection)
				{
					Preparations::Basic::ClearBackBuffer(Oyster::Math::Float4(1,0,0,1));
					Core::ShaderManager::SetShaderEffect(Graphics::Render::Resources::obj);
					Preparations::Basic::BindBackBufferRTV();

					Definitions::VP vp;
					vp.V = View;
					vp.P = Projection;

					void* data = Resources::VPData.Map();
					memcpy(data, &vp, sizeof(Definitions::VP));
					Resources::VPData.Unmap();

					Resources::VPData.Apply();
				}

				void Basic::RenderScene(Model::Model* models, int count)
				{
					for(int i = 0; i < count; ++i)
					{
						if(models[i].Visible)
						{
							void* data  = Resources::ModelData.Map();
							memcpy(data,&(models[i].WorldMatrix),sizeof(Math::Float4x4));
							Resources::ModelData.Unmap();

							
							Model::ModelInfo* info = (Model::ModelInfo*)models[i].info;

							Core::deviceContext->PSSetShaderResources(0,info->Material.size(),&(info->Material[0]));


							info->Vertices->Apply();
							if(info->Indexed)
							{
								info->Indecies->Apply();
								Oyster::Graphics::Core::deviceContext->DrawIndexed(info->VertexCount,0,0);
							}
							else
							{
								Oyster::Graphics::Core::deviceContext->Draw(info->VertexCount,0);
							}
						}
					}				
				}
				void Basic::EndFrame()
				{
					IDXGISwapChain* chain = Core::swapChain;
					chain->Present(0,0);
				}
			}
		}
	}
}