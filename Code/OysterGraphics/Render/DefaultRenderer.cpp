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

			/********************************************************
			 * Private Prototype Methods
			 ********************************************************/
			void AnimateRelativeBones( const Model::ModelInfo &info, Model::AnimationData &anim, Math::Matrix relativeBuffer[] );
			void MergeAnimatedBones
			(
				const Model::Bone  raw[], int numBones, Math::Float interpolation, // comparable raw bone data and interpolation value
				const Math::Matrix animatedBoneSourceA[], // relative bone animations
					  Math::Matrix animatedBoneSourceB_Target[] // relative bone animations and targetbuffer
			);
			int AnimateAbsoluteBones
			(
				const Model::ModelInfo &info, Math::Float deltaTime,
				Model::AnimationData &anim,
				Math::Matrix SkinTransformBuffer[],
				Math::Matrix BoneAnimationBuffer_Relative[],
				Math::Matrix BoneAnimationBuffer_Absolute[]
			);
			int AnimateAbsoluteBones
			(
				const Model::ModelInfo &info, Math::Float deltaTime,
				Model::AnimationData anim[], int numAnimations,
				Math::Matrix SkinTransformBuffer[],
				Math::Matrix BoneAnimationBuffer_Relative[],
				Math::Matrix BoneAnimationBuffer_Absolute[]
			);

			/********************************************************
			 * Public Method Implementations
			 ********************************************************/

			void DefaultRenderer::NewFrame(Oyster::Math::Float4x4 View, Oyster::Math::Float4x4 Projection, Definitions::Pointlight** Lights, int numLights)
			{
				Preparations::Basic::ClearBackBuffer(Oyster::Math::Float4(0,0,0,0));
				Preparations::Basic::ClearDepthStencil(Resources::Gui::depth);
				Preparations::Basic::ClearDepthStencil(Resources::Gather::NoDepthView);
				Preparations::Basic::ClearRTV(Resources::GBufferRTV,Resources::GBufferSize,Math::Float4(0,0,0,0));
				Lights[1];

				void* data;

				Definitions::LightConstants lc;
				lc.InvProj =  Projection.GetInverse();
				lc.Lights = numLights;
				lc.View = View;
				lc.Proj = Projection;
				lc.AmbFactor = Core::amb;
				lc.SSAORadius = 3;

				data = Resources::Light::LightConstantsData.Map();
				memcpy(data, &lc, sizeof(Definitions::LightConstants));
				Resources::Light::LightConstantsData.Unmap();

				data = Resources::Light::PointLightsData.Map();
					Definitions::Pointlight* plData = (Definitions::Pointlight*)data;
					for(int i=0; i < numLights; ++i)
					{
						plData[i].Pos =  (View * Math::Float4(Lights[i]->Pos,1)).xyz;
						plData[i].Radius = Lights[i]->Radius;
						plData[i].Color = Lights[i]->Color;
						plData[i].Bright = Lights[i]->Bright;
					}
					//memcpy(data, Lights, sizeof(Definitions::Pointlight) * numLights);
				Resources::Light::PointLightsData.Unmap();

				for(auto i = Render::Resources::RenderData.begin(); i != Render::Resources::RenderData.end(); i++ )
				{
					(*i).second->Models=0;
				}
				for(auto i = Render::Resources::NoDepthData.begin(); i != Render::Resources::NoDepthData.end(); i++ )
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
						normalTransform.Invert().Transpose();
						Math::Matrix m = Math::Matrix(Math::Vector4(normalTransform.v[0],0.0f), Math::Vector4(normalTransform.v[1],0.0f), Math::Vector4(normalTransform.v[2],0.0f), Math::Vector4(0.0f, 0.0f, 0.0f, 1.0f));
						rid.WV = View * m;
						rid.WVP = Projection * View * models[i].WorldMatrix;

						rid.Tint = models[i].Tint;
						rid.GTint = models[i].GlowTint;
						if(models[i].IgnoreDepth)
						{
							Resources::NoDepthData[info]->rid[Resources::NoDepthData[info]->Models++] = rid;
						}
						else
						{
							Resources::RenderData[info]->rid[Resources::RenderData[info]->Models++] = rid;
						}
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
							
						// Bone animation buffers
						Math::Matrix SkinTransform[100];
						Math::Matrix BoneAnimated[100];
						Math::Matrix BoneAbsAnimated[100];

						pm.Animated = AnimateAbsoluteBones
						(
							*info, deltaTime,
							models[i].Animation,
							models[i].numOccupiedAnimationSlots,
							SkinTransform, BoneAnimated, BoneAbsAnimated
						);

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

				Core::deviceContext->OMSetRenderTargets(2,&Resources::Gather::InstancedPass.RTV[0],Resources::Gather::NoDepthView);

				for(auto i = Render::Resources::NoDepthData.begin(); i != Render::Resources::NoDepthData.end(); i++ )
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

			/********************************************************
			 * Private Prototype Method Implementations
			 ********************************************************/

			void AnimateRelativeBones( const Model::ModelInfo &info, Model::AnimationData &anim, Math::Matrix relativeBuffer[] )
			{
				for( int i = 0; i < info.BoneCount; ++i )
				{
					Model::Bone Bone = info.bones[i];
					relativeBuffer[i] = Bone.Relative;
				}

				const Model::Animation &A = *anim.AnimationPlaying;

				while( anim.AnimationTime > A.duration && anim.LoopAnimation )
					anim.AnimationTime -= (float)A.duration;
									
				float position = anim.AnimationTime;
				for( int i = 0; i < A.Bones; ++i )
				{
					//find current frame
					int nrOfFrames = A.Frames[i];
					Model::Frame PFrame = A.Keyframes[i][nrOfFrames-1];
					Model::Frame NFrame = A.Keyframes[i][nrOfFrames-1];
					bool FrameFound = false;

					for (int j = 0; j < nrOfFrames; j++)
					{
						if(position < A.Keyframes[i][j].time)
						{
							PFrame = A.Keyframes[i][j-1];
							NFrame = A.Keyframes[i][j];
							break;
						}
					}

					float denominator = (float)(NFrame.time - PFrame.time);
					
					if( denominator != 0.0f )
					{
						float inter = (float)((position - PFrame.time) / denominator);
						Math3D::InterpolateOrientation_UsingNonRigidNlerp( PFrame.bone.Relative,NFrame.bone.Relative, inter, relativeBuffer[PFrame.bone.Parent] );
					}
					else
					{
						relativeBuffer[PFrame.bone.Parent] = PFrame.bone.Relative;
					}
				}
			}

			enum Conflict
			{
				Conflict_detected,
				Conflict_useA,
				Conflict_useB
			};

			Conflict DetectBoneAnimationConflict( const Model::Bone &raw, const Math::Matrix &animBoneA, const Math::Matrix &animBoneB );

			void MergeAnimatedBones( const Model::Bone raw[], int numBones, Math::Float interpolation, const Math::Matrix *animatedBoneSourceA, Math::Matrix animatedBoneSourceB_Target[] )
			{
				for( int i = 0; i < numBones; ++i )
				{
					switch( DetectBoneAnimationConflict(raw[i], animatedBoneSourceA[i], animatedBoneSourceB_Target[i]) )
					{
					case Conflict_detected:
						Math3D::InterpolateOrientation_UsingNonRigidNlerp( animatedBoneSourceA[i], animatedBoneSourceB_Target[i], interpolation, animatedBoneSourceB_Target[i] );
						break;
					case Conflict_useA:
						animatedBoneSourceB_Target[i] = animatedBoneSourceA[i];
						break;
					default: case Conflict_useB: break;
					}
				}
			}

			Conflict DetectBoneAnimationConflict( const Model::Bone &raw, const Math::Matrix &animBoneA, const Math::Matrix &animBoneB )
			{
				if( animBoneA == raw.Relative )
					return Conflict_useB;

				if( animBoneB == raw.Relative )
					return Conflict_useA;

				return Conflict_detected;
			}

			int AnimateAbsoluteBones( const Model::ModelInfo &info, Math::Float deltaTime, Model::AnimationData &anim, Math::Matrix SkinTransformBuffer[], Math::Matrix BoneAnimationBuffer_Relative[], Math::Matrix BoneAnimationBuffer_Absolute[] )
			{
				if( !info.Animated || (anim.AnimationPlaying == nullptr) )
				{ // no animation
					return 0;
				}

				anim.AnimationTime += deltaTime;
				AnimateRelativeBones( info, anim, BoneAnimationBuffer_Relative );

				for( int i = 0; i < info.BoneCount; ++i )
				{
					Model::Bone Bone = info.bones[i];
					SkinTransformBuffer[i] = Bone.Absolute.GetInverse();
					BoneAnimationBuffer_Absolute[i] = Bone.Absolute;
				}

				Definitions::AnimationData am;

				for( int i = 0; i < info.BoneCount; ++i )
				{
					//calculate Absolute Animation Transform
					BoneAnimationBuffer_Absolute[i] = BoneAnimationBuffer_Absolute[info.bones[i].Parent] * BoneAnimationBuffer_Relative[i];

					//write data to am
					am.AnimatedData[i] =  (BoneAnimationBuffer_Absolute[i] * SkinTransformBuffer[i]);
				}
					
				void *data = Resources::Gather::AnimationData.Map();
				memcpy( data, &am, sizeof(Definitions::AnimationData) );
				Resources::Gather::AnimationData.Unmap();

				return 1;
			}

			int AnimateAbsoluteBones( const Model::ModelInfo &info, Math::Float deltaTime, Model::AnimationData anim[], int numAnimations, Math::Matrix SkinTransformBuffer[], Math::Matrix BoneAnimationBuffer_Relative[], Math::Matrix BoneAnimationBuffer_Absolute[] )
			{
				if( !info.Animated || (numAnimations < 1) )
				{ // no animation
					return 0;
				}
				int isAnimated = 0;

				// for each animation
				for( int i = 0; i < numAnimations; ++i )
				{
					if( anim[i].AnimationPlaying != nullptr )
					{
						anim[i].AnimationTime += deltaTime;
						if( isAnimated )
						{
							AnimateRelativeBones( info, anim[i], BoneAnimationBuffer_Absolute ); // Borrowing BoneAnimationBuffer_Absolute as interim buffer
							MergeAnimatedBones( info.bones, info.BoneCount, 0.5f, BoneAnimationBuffer_Absolute, BoneAnimationBuffer_Relative );
						}
						else
						{
							isAnimated = 1;
							AnimateRelativeBones( info, anim[i], BoneAnimationBuffer_Relative );
						}
					}
				}

				if( isAnimated )
				{
					for( int i = 0; i < info.BoneCount; ++i )
					{
						Model::Bone Bone = info.bones[i];
						SkinTransformBuffer[i] = Bone.Absolute.GetInverse();
						BoneAnimationBuffer_Absolute[i] = Bone.Absolute;
					}

					Definitions::AnimationData am;

					for( int i = 0; i < info.BoneCount; ++i )
					{
						//calculate Absolute Animation Transform
						BoneAnimationBuffer_Absolute[i] = BoneAnimationBuffer_Absolute[info.bones[i].Parent] * BoneAnimationBuffer_Relative[i];

						//write data to am
						am.AnimatedData[i] =  (BoneAnimationBuffer_Absolute[i] * SkinTransformBuffer[i]);
					}

					void *data = Resources::Gather::AnimationData.Map();
					memcpy( data, &am, sizeof(Definitions::AnimationData) );
					Resources::Gather::AnimationData.Unmap();
				}

				return isAnimated;
			}
		}
	}
}