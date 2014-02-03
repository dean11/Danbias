#include "Render.h"
#include "../Resources/Deffered.h"
#include "../../Definitions/GraphicalDefinition.h"
#include "../../Model/ModelInfo.h"
#include <map>
#include <vector>

namespace Oyster
{
	namespace Graphics
	{
		namespace Render
		{
			namespace Rendering
			{
				Definitions::Pointlight pl;

				void Basic::NewFrame(Oyster::Math::Float4x4 View, Oyster::Math::Float4x4 Projection, Definitions::Pointlight* Lights, int numLights)
				{
					Preparations::Basic::ClearBackBuffer(Oyster::Math::Float4(1,0,0,1));
					Preparations::Basic::ClearRTV(Resources::Deffered::GBufferRTV,Resources::Deffered::GBufferSize,Math::Float4(0,0,0,1));
					Core::PipelineManager::SetRenderPass(Graphics::Render::Resources::Deffered::GeometryPass);

					void* data;

					Definitions::LightConstants lc;
					lc.InvProj =  Projection.GetInverse();
					lc.Pixels = Core::resolution;
					lc.Lights = numLights;
					lc.View = View;
					lc.Proj = Projection;
					lc.SSAORadius = 3;

					data = Resources::Deffered::LightConstantsData.Map();
					memcpy(data, &lc, sizeof(Definitions::LightConstants));
					Resources::Deffered::LightConstantsData.Unmap();

					data = Resources::Deffered::PointLightsData.Map();
					memcpy(data, Lights, sizeof(Definitions::Pointlight) * numLights);
					Resources::Deffered::PointLightsData.Unmap();
				}

				Math::Matrix RecursiveBindPos(int index, Model::ModelInfo* mi)
				{
					if(mi->bones[index].Parent == index)
						return mi->bones[index].Transform;

					return mi->bones[index].Transform.GetInverse() * RecursiveBindPos(mi->bones[index].Parent,mi);
				}

				void Basic::RenderScene(Model::Model* models, int count, Math::Matrix View, Math::Matrix Projection)
				{
					for(int i = 0; i < count; ++i)
					{
						if(&models[i] == NULL)
							continue;
						if(models[i].Visible)
						{
							Definitions::PerModel pm;
							pm.WV = View * models[i].WorldMatrix;
							pm.WVP = Projection * pm.WV;

							void* data  = Resources::Deffered::ModelData.Map();
							memcpy(data,&(pm),sizeof(pm));
							Resources::Deffered::ModelData.Unmap();
							
							Model::ModelInfo* info = (Model::ModelInfo*)models[i].info;
							
							
							Definitions::AnimationData am;	//final
							if(info->Animated && models[i].AnimationPlaying != -1)
							{
								for(int b = 0; b <info->BoneCount; ++b)
								{
									am.animatedData[b] = RecursiveBindPos(b,info);
								}
								am.Animated = 1;
							}
							else
								am.Animated = 0;

							data = Resources::Deffered::AnimationData.Map();
							memcpy(data,&am,sizeof(Definitions::AnimationData));
							Resources::Deffered::AnimationData.Unmap();

							if(info->Material.size())
							{
								Core::deviceContext->PSSetShaderResources(0,(UINT)info->Material.size(),&(info->Material[0]));
							}


							info->Vertices->Apply();
							if(info->Indexed)
							{
								info->Indecies->Apply();
								Oyster::Graphics::Core::deviceContext->DrawIndexed(info->IndexCount,0,0);
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
					Core::PipelineManager::SetRenderPass(Resources::Deffered::LightPass);

					Core::deviceContext->Dispatch((UINT)((Core::resolution.x + 15U) / 16U), (UINT)((Core::resolution.y + 15U) / 16U), 1);

					Core::PipelineManager::SetRenderPass(Resources::Deffered::PostPass);

					Core::deviceContext->Dispatch((UINT)((Core::resolution.x + 15U) / 16U), (UINT)((Core::resolution.y + 15U) / 16U), 1);

					Core::swapChain->Present(0,0);
				}
			}
		}
	}
}