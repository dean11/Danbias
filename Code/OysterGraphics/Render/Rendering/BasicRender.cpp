#include "Render.h"
#include "../Resources/Deffered.h"
#include "../../Definitions/GraphicalDefinition.h"
#include "../../Model/ModelInfo.h"
#include "../../DllInterfaces/GFXAPI.h"
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
				Model::Model* Basic::cube = NULL;
				Model::Model* Basic::cube2 = NULL; 

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

				Math::Matrix RecursiveBindPosRotation(int index, Model::ModelInfo* mi)
				{
					if(mi->bones[index].Parent == index)
						return mi->bones[index].Relative;
					
					return mi->bones[index].Relative*mi->bones[mi->bones->Parent].Relative;
				}

				Math::Vector4 RecursiveBindPosPosition(int index, Model::ModelInfo* mi)
				{
					//return Math::Vector4::standard_unit_w;
					if(mi->bones[index].Parent == index)
						return mi->bones[index].Relative.v[3];
					
					return Math::Vector4(RecursiveBindPosPosition(mi->bones->Parent, mi).xyz + (mi->bones[index].Relative.v[3] * RecursiveBindPosRotation(mi->bones->Parent,mi)).xyz,1);
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
								cube->WorldMatrix == Math::Matrix::identity;
								////store inverse absolut transform
								Math::Matrix SkinTransform[100];
								Math::Matrix BoneAnimated[100];
								Math::Matrix BoneAbsAnimated[100];

								Math::Matrix Scale = Math::Matrix::identity;
								Scale.m[0][0] = 1;
								Scale.m[1][1] = 1;
								Scale.m[2][2] = 2;

								

								for(int b = 0; b <info->BoneCount; ++b)
								{
									Model::Bone Bone = info->bones[b];
									SkinTransform[b] = Bone.Absolute.GetInverse();
									BoneAnimated[b] = Bone.Relative;
									BoneAbsAnimated[b] = Bone.Absolute;

									
									cube2->WorldMatrix = Scale;
									cube2->WorldMatrix.v[3] = info->bones[b].Absolute.v[3];
									//Basic::RenderScene(cube2,1, View, Projection);
								}
								//BoneAnimated[8] = Math3D::RotationMatrix(3.14/4, Math::Float3(0, 0, 1)) * info->bones[8].Relative;
								//BoneAnimated[31] = Math3D::RotationMatrix(3.14/4, Math::Float3(0, 0, 1)) * info->bones[31].Relative;
								////for each bone in animation 
								////HACK use first bone
								int b = 0;
								Model::Animation A = info->Animations[models[i].AnimationPlaying];
								while(models[i].AnimationTime>A.duration)
									models[i].AnimationTime -= A.duration;
									
								float position = models[i].AnimationTime;
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
									float denominator = (NFrame.time - PFrame.time);
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
									//SkinTransform[b] = BoneAbsAnimated[b];
									cube->WorldMatrix = Scale;
									cube->WorldMatrix.v[3] = BoneAbsAnimated[b].v[3];
									Basic::RenderScene(cube,1,View,Projection);
								}

								//write data to am
								for(int b = 0; b < info->BoneCount; ++b)
								{
									am.AnimatedData[b] =  (BoneAbsAnimated[b] * SkinTransform[b]);
									am.BindPoseData[b] = info->bones[b].Absolute;//Math3D::ExtractRotationMatrix(am.animatedData[b]);
								}

								//retore to draw animated model
								Definitions::PerModel pm;
								pm.WV = View * models[i].WorldMatrix;
								pm.WVP = Projection * pm.WV;

								void* data  = Resources::Deffered::ModelData.Map();
								memcpy(data,&(pm),sizeof(pm));
								Resources::Deffered::ModelData.Unmap();

								//delete[]SkinTransform;
								//delete[]BoneAbsAnimated;
								//delete[]BoneAnimated;

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