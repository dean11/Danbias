#include "DefaultRenderer.h"
#include "Resources.h"
#include "../Definitions/GraphicalDefinition.h"
#include "../Model/ModelInfo.h"
#include "../DllInterfaces/GFXAPI.h"
#include <map>
#include <vector>

namespace Oyster
{
	namespace Graphics
	{
		namespace Render
		{
				Definitions::Pointlight pl;

				void DefaultRenderer::NewFrame(Oyster::Math::Float4x4 View, Oyster::Math::Float4x4 Projection, Definitions::Pointlight* Lights, int numLights)
				{
					Preparations::Basic::ClearBackBuffer(Oyster::Math::Float4(1,0,0,1));
					Preparations::Basic::ClearRTV(Resources::GBufferRTV,Resources::GBufferSize,Math::Float4(0,0,0,0));
					Core::PipelineManager::SetRenderPass(Graphics::Render::Resources::Gather::Pass);
					Lights[1];

					void* data;

					Definitions::LightConstants lc;
					lc.InvProj =  Projection.GetInverse();
					lc.Pixels = Core::resolution;
					lc.Lights = numLights;
					lc.View = View;
					lc.Proj = Projection;
					lc.SSAORadius = 3;

					data = Resources::Light::LightConstantsData.Map();
					memcpy(data, &lc, sizeof(Definitions::LightConstants));
					Resources::Light::LightConstantsData.Unmap();

					data = Resources::Light::PointLightsData.Map();
					memcpy(data, Lights, sizeof(Definitions::Pointlight) * numLights);
					Resources::Light::PointLightsData.Unmap();

					Definitions::PostData pd;
					pd.x = (int)lc.Pixels.x;
					pd.y = (int)lc.Pixels.y;

					data = Resources::Post::Data.Map();
					memcpy(data, &pd, sizeof(Definitions::PostData));
					Resources::Post::Data.Unmap();
				}

				void DefaultRenderer::RenderScene(Model::Model* models, int count, Math::Matrix View, Math::Matrix Projection, float deltaTime)
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

							Model::ModelInfo* info = models[i].info;
							
							Definitions::AnimationData am;	//final
							if(info->Animated && models[i].Animation.AnimationPlaying != NULL)
							{
								models[i].Animation.AnimationTime += deltaTime;
								////store inverse absolut transform
								Math::Matrix SkinTransform[100];
								Math::Matrix BoneAnimated[100];
								Math::Matrix BoneAbsAnimated[100];

								

								for(int b = 0; b <info->BoneCount; ++b)
								{
									Model::Bone Bone = info->bones[b];
									SkinTransform[b] = Bone.Absolute.GetInverse();
									BoneAnimated[b] = Bone.Relative;
									BoneAbsAnimated[b] = Bone.Absolute;
								}
								int b = 0;
								Model::Animation A = *models[i].Animation.AnimationPlaying;
								while(models[i].Animation.AnimationTime>A.duration && models[i].Animation.LoopAnimation)
									models[i].Animation.AnimationTime -= (float)A.duration;
									
								float position = models[i].Animation.AnimationTime;
								for(int b = 0; b < A.Bones;++b)
								{
									//find current frame
									int nrOfFrames = A.Frames[b];
									Model::Frame PFrame = A.Keyframes[b][nrOfFrames-1];
									Model::Frame NFrame = A.Keyframes[b][nrOfFrames-1];
									bool FrameFound = false;
									for (int i = 0; i < nrOfFrames; i++)
									{
										if(position < A.Keyframes[b][i].time)
										{
											PFrame = A.Keyframes[b][i-1];
											NFrame = A.Keyframes[b][i];
											break;
										}
									}
									float denominator = (float)(NFrame.time - PFrame.time);
									if(denominator == 0)
									{
										BoneAnimated[PFrame.bone.Parent] = PFrame.bone.Relative;
										continue;
									}
									float inter = (float)((position - PFrame.time) / denominator);
									Math3D::InterpolateOrientation_UsingNonRigidNlerp(PFrame.bone.Relative,NFrame.bone.Relative,inter, BoneAnimated[PFrame.bone.Parent]);
								}

								////calculate Absolute Animation Transform
								for(int b = 0; b < info->BoneCount; ++b)
								{
									BoneAbsAnimated[b] = BoneAbsAnimated[info->bones[b].Parent] * BoneAnimated[b];
								}

								//write data to am
								for(int b = 0; b < info->BoneCount; ++b)
								{
									am.AnimatedData[b] =  (BoneAbsAnimated[b] * SkinTransform[b]);
								}

								
								void *data = Resources::Gather::AnimationData.Map();
								memcpy(data,&am,sizeof(Definitions::AnimationData));
								Resources::Gather::AnimationData.Unmap();

								pm.Animated = 1;
							}
							else
								pm.Animated = 0;

							void* data  = Resources::Gather::ModelData.Map();
							memcpy(data,&(pm),sizeof(pm));
							Resources::Gather::ModelData.Unmap();

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


				void DefaultRenderer::EndFrame()
				{
					Core::PipelineManager::SetRenderPass(Resources::Light::Pass);

					Core::deviceContext->Dispatch((UINT)((Core::resolution.x + 15U) / 16U), (UINT)((Core::resolution.y + 15U) / 16U), 1);

					//Core::PipelineManager::SetRenderPass(Resources::Blur::HorPass);
					//Core::deviceContext->Dispatch((UINT)((Core::resolution.x + 15U) / 16U), (UINT)((Core::resolution.y + 15U) / 16U), 1);

					//Core::PipelineManager::SetRenderPass(Resources::Blur::VertPass);
					//Core::deviceContext->Dispatch((UINT)((Core::resolution.x + 15U) / 16U), (UINT)((Core::resolution.y + 15U) / 16U), 1);

					Core::PipelineManager::SetRenderPass(Resources::Post::Pass);

					Core::deviceContext->Dispatch((UINT)((Core::resolution.x + 15U) / 16U), (UINT)((Core::resolution.y + 15U) / 16U), 1);

					Core::swapChain->Present(0,0);
				}
		}
	}
}