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

							

							Definitions::AnimationData am;
							if(info->Animated && models[i].AnimationPlaying != -1)
							{
								
								Definitions::AnimationData am2;
								//write default data
								for (int b = 0; b < info->BoneCount; b++)
								{
									am2.animatedData[b] = info->bones[b].Transform;
								}
								//loop bones in animation
								am.Animated = 1;
								

								for(int x = 0; x < info->Animations[models[i].AnimationPlaying].Bones; ++x)
								{
									Model::Frame Prev, Next;
									//loop frame per bone
									for(int y = 0; y < info->Animations[models[i].AnimationPlaying].Frames[x]; ++y)
									{
										///TODO replace with binary search?
										Model::Frame f = info->Animations[models[i].AnimationPlaying].Keyframes[x][y];
										
										//if we hit frame
										if(models[i].AnimationTime == f.time)
										{
											Prev = f;
											Next = f;
											break;
										}

										//if time is larger than frame time, store frames
										if(models[i].AnimationTime < f.time)
										{
											Next = f;
											Prev = info->Animations[models[i].AnimationPlaying].Keyframes[x][y-1];
											break;
										}
									}


									//calculate interpolated bone position
									
									//rebase model time to between prev and next
									float interpoation =(models[i].AnimationTime - Prev.time) / (Next.time - Prev.time);

									//interpolate
									Math::Matrix Interpolated = Prev.bone.Transform;

									//write magic to animated data
									am2.animatedData[Prev.bone.Parent] =  Interpolated * am2.animatedData[info->bones[Prev.bone.Parent].Parent];
									//sneaky write do correct data buffer
									am.animatedData[x] = am2.animatedData[Prev.bone.Parent].GetInverse() * info->bones[Prev.bone.Parent].Transform;
								}
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