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
								//store inverse absolut transform
								Math::Matrix* SkinTransform = new Math::Matrix[info->BoneCount];
								Math::Matrix* BoneAnimated = new Math::Matrix[info->BoneCount];
								Math::Matrix* BoneAbsAnimated = new Math::Matrix[info->BoneCount];

								Math::Matrix Scale = Math::Matrix::identity;
								Scale.m[1][1] = 0.1f;
								Scale.m[2][2] = 0.1f;
								Scale.m[3][3] = 2;

								for(int b = 0; b <info->BoneCount; ++b)
								{
									Model::Bone Bone = info->bones[b];
									SkinTransform[b] = Bone.Absolute.GetInverse();
									BoneAnimated[b] = Bone.Relative;
									BoneAbsAnimated[b] = Bone.Absolute;
								}
								//for each bone in animation 
								//HACK use first bone
								int b = 0;
								Model::Animation A = info->Animations[models[i].AnimationPlaying];
								//for(int b = 0; b < A.Bones;++b)
								{
									//for each frame on bone Write current relative data
									//HACK use first frame
									int f = 0;
									//for(int f = 0; f < A.Frames[b]; ++b)
									{
										//find right frame
										//HACK accept first
										Model::Frame Current = A.Keyframes[b][f];

										//calculate new matrix
										Model::Bone CBone = Current.bone;
										BoneAnimated[CBone.Parent] = CBone.Relative;
									}
								}

								//calculate Absolute Animation Transform
								for(int b = 0; b < info->BoneCount; ++b)
								{
									BoneAbsAnimated[b] = BoneAbsAnimated[info->bones[b].Parent] * BoneAnimated[b];
									cube->WorldMatrix = BoneAbsAnimated[b] * Scale;
									cube->WorldMatrix.v[3] = BoneAbsAnimated[b].v[3];
									//Basic::RenderScene(cube,1,View,Projection);
								}

								//write data to am
								for(int b = 0; b < info->BoneCount; ++b)
								{
									am.animatedData[b] = BoneAbsAnimated[b] * SkinTransform[b];
									cube2->WorldMatrix = Scale;
									cube2->WorldMatrix.v[3] = info->bones[b].Absolute.v[3];
									Basic::RenderScene(cube2,1,View,Projection);
								}

								//retore to draw animated model
								Definitions::PerModel pm;
								pm.WV = View * models[i].WorldMatrix;
								pm.WVP = Projection * pm.WV;

								void* data  = Resources::Deffered::ModelData.Map();
								memcpy(data,&(pm),sizeof(pm));
								Resources::Deffered::ModelData.Unmap();

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