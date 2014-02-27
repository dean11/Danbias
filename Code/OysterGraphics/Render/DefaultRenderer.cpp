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

				void DefaultRenderer::NewFrame(Oyster::Math::Float4x4 View, Oyster::Math::Float4x4 Projection, Definitions::Pointlight* Lights, int numLights, float Fov)
				{
					Preparations::Basic::ClearBackBuffer(Oyster::Math::Float4(0,0,0,0));
					Preparations::Basic::ClearDepthStencil(Resources::Gui::depth);
					Preparations::Basic::ClearRTV(Resources::GBufferRTV,Resources::GBufferSize,Math::Float4(0,0,0,0));
					Lights[1];

					void* data;

					Definitions::LightConstants lc;
					lc.InvProj =  Projection.GetInverse();
					lc.FoV = Fov;
					lc.Lights = numLights;
					lc.View = View;
					lc.Proj = Projection;
					lc.SSAORadius = 3;

					data = Resources::Light::LightConstantsData.Map();
					memcpy(data, &lc, sizeof(Definitions::LightConstants));
					Resources::Light::LightConstantsData.Unmap();

					data = Resources::Light::PointLightsData.Map();
					Definitions::Pointlight* plData = (Definitions::Pointlight*)data;
					for(int i=0; i < numLights; ++i)
					{
						plData[i].Pos =  (View * Math::Float4(Lights[i].Pos,1)).xyz;
						plData[i].Radius = Lights[i].Radius;
						plData[i].Color = Lights[i].Color;
						plData[i].Bright = Lights[i].Bright;
					}
					//memcpy(data, Lights, sizeof(Definitions::Pointlight) * numLights);
					Resources::Light::PointLightsData.Unmap();

					for(auto i = Render::Resources::RenderData.begin(); i != Render::Resources::RenderData.end(); i++ )
					{
						(*i).second->Models=0;
					}

					Core::PipelineManager::SetRenderPass(Resources::Gather::AnimatedPass);
				}

				void DefaultRenderer::RenderScene(Model::Model* models, int count, Math::Matrix View, Math::Matrix Projection, float deltaTime)
				{
					for(int i = 0; i < count; ++i)
					{
						if(&models[i] == NULL || !models[i].Visible)
							continue;

						Model::ModelInfo* info = models[i].info;
						if(!info->Animated && models[i].Instanced)
						{
							Definitions::RenderInstanceData rid;
							Math::Float3x3 normalTransform;
							normalTransform = Math::Float3x3(models[i].WorldMatrix.v[0].xyz, models[i].WorldMatrix.v[1].xyz, models[i].WorldMatrix.v[2].xyz);
							normalTransform.Transpose().Invert();
							Math::Matrix m = Math::Matrix(Math::Vector4(normalTransform.v[0],0.0f), Math::Vector4(normalTransform.v[1],0.0f), Math::Vector4(normalTransform.v[2],0.0f), Math::Vector4(0.0f));
							rid.WV = View * m;
							rid.WVP = Projection * View * models[i].WorldMatrix;

							rid.Tint = models[i].Tint;
							rid.GTint = models[i].GlowTint;

							Resources::RenderData[info]->rid[Resources::RenderData[info]->Models++] = rid;
						}
						else
						{
							Definitions::PerModel pm;
							Math::Float3x3 normalTransform;
							normalTransform = Math::Float3x3(models[i].WorldMatrix.v[0].xyz, models[i].WorldMatrix.v[1].xyz, models[i].WorldMatrix.v[2].xyz);
							normalTransform.Transpose().Invert();
							Math::Matrix m = Math::Matrix(Math::Vector4(normalTransform.v[0],0.0f), Math::Vector4(normalTransform.v[1],0.0f), Math::Vector4(normalTransform.v[2],0.0f), Math::Vector4(0.0f));
							pm.WV = View * m;
							pm.WVP = Projection * View * models[i].WorldMatrix;

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

							Definitions::TintData td;
							td.GlowTint = models[i].GlowTint;
							td.Tint = models[i].Tint;
							td.PAD = 0;
							td.PAD2 = 0;
							int s = sizeof(Definitions::TintData);

							data = Render::Resources::Color.Map();
							memcpy(data,&td,sizeof(Definitions::TintData));
							Render::Resources::Color.Unmap(); 

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

				void BlurGlow()
				{
					Definitions::BlurrData bd;
					bd.BlurMask = Math::Float4(1,1,1,1);
					bd.StopX = (UINT)Core::resolution.x/2;
					bd.StopY = (UINT)Core::resolution.y;
					bd.StartX = 0;
					bd.StartY = (UINT)Core::resolution.y/2;
					
					void* data = Resources::Blur::Data.Map();
					memcpy(data,&bd,sizeof(Definitions::BlurrData));
					Resources::Blur::Data.Unmap();

					Core::PipelineManager::SetRenderPass(Resources::Blur::HorPass);
					Core::deviceContext->Dispatch((UINT)((Core::resolution.x/2 + 127U) / 128U), (UINT)(Core::resolution.y/2), 1);

					Core::PipelineManager::SetRenderPass(Resources::Blur::VertPass);
					Core::deviceContext->Dispatch((UINT)(Core::resolution.x/2), (UINT)((Core::resolution.y/2 + 127U) / 128U), 1);
				}

				void BlurSSAO()
				{
					Definitions::BlurrData bd;
					bd.BlurMask = Math::Float4(0,0,0,1);
					bd.StopX = (UINT)Core::resolution.x/2;
					bd.StopY = (UINT)Core::resolution.y/2;
					bd.StartX = 0;
					bd.StartY = 0;
					
					void* data = Resources::Blur::Data.Map();
					memcpy(data,&bd,sizeof(Definitions::BlurrData));
					Resources::Blur::Data.Unmap();

					Core::PipelineManager::SetRenderPass(Resources::Blur::HorPass);
					Core::deviceContext->Dispatch((UINT)((Core::resolution.x/2 + 127U) / 128U), (UINT)(Core::resolution.y/2), 1);

					Core::PipelineManager::SetRenderPass(Resources::Blur::VertPass);
					Core::deviceContext->Dispatch((UINT)(Core::resolution.x/2), (UINT)((Core::resolution.y/2 + 127U) / 128U), 1);
				}

				void RenderModel(Model::ModelInfo* info, Definitions::RenderInstanceData* rid , int count)
				{
					if(count < 1)
						return;
					if(info->Material.size())
					{
						Core::deviceContext->PSSetShaderResources(0,(UINT)info->Material.size(),&(info->Material[0]));
					}
					info->Vertices->Apply();
					if(info->Indexed)
					{
						info->Indecies->Apply();
					}

					void* data = Resources::Gather::InstancedData.Map();
					memcpy(data, rid, sizeof(Definitions::RenderInstanceData)*count);
					Resources::Gather::InstancedData.Unmap();

					if(info->Indexed)
					{
						Core::deviceContext->DrawIndexedInstanced(info->IndexCount,count,0,0,0);
						//Core::deviceContext->DrawIndexed(info->IndexCount,0,0);
					}
					else
					{
						Core::deviceContext->DrawInstanced(info->VertexCount,count,0,0);
						//Core::deviceContext->Draw(info->VertexCount,0);
					}
				}


				void DefaultRenderer::EndFrame()
				{
					Core::PipelineManager::SetRenderPass(Graphics::Render::Resources::Gather::InstancedPass);
					Resources::Gather::InstancedData.Apply(1);

					for(auto i = Render::Resources::RenderData.begin(); i != Render::Resources::RenderData.end(); i++ )
					{
						RenderModel((*i).first,(*i).second->rid, (*i).second->Models);
					}

					Core::PipelineManager::SetRenderPass(Resources::Light::Pass);

					Core::deviceContext->Dispatch((UINT)((Core::resolution.x + 15U) / 16U), (UINT)((Core::resolution.y + 15U) / 16U), 1);

					BlurGlow();

					BlurSSAO();

					Core::PipelineManager::SetRenderPass(Resources::Post::Pass);

					Core::deviceContext->Dispatch((UINT)((Core::resolution.x + 15U) / 16U), (UINT)((Core::resolution.y + 15U) / 16U), 1);

					Core::swapChain->Present(0,0);
				}
		}
	}
}