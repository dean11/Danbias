#pragma once

#include "../Core/Core.h"
#include "../Model/ModelInfo.h"
#include "../Definitions/GraphicalDefinition.h"

namespace Oyster
{
	namespace Graphics
	{
		namespace Render
		{
				class Resources
				{
				public:
					struct ModelDataWrapper
					{
						Definitions::RenderInstanceData* rid;
						int Models;
					};

					
					static std::map<Model::ModelInfo*, ModelDataWrapper*> RenderData;
					static std::map<Model::ModelInfo*, ModelDataWrapper*> NoDepthData;
					
					static const int GBufferSize = 3;
					static const int LBufferSize = 3;
					static const int MaxLightSize = 1024;

					//! GBuffers
					//! 0 = Diffuse + Glow
					//! 1 = Normal + Spec
					//! 2 = GUI
					static ID3D11RenderTargetView* GBufferRTV[GBufferSize];
					static ID3D11ShaderResourceView* GBufferSRV[GBufferSize];

					//! LBuffer
					//! 0 = Diffuse
					//! 1 = Specular
					//! 2 = SSAO
					static ID3D11UnorderedAccessView* LBufferUAV[LBufferSize];
					static ID3D11ShaderResourceView* LBufferSRV[LBufferSize];

					
					static Core::Buffer Color;


					struct RenderStates
					{
						static ID3D11RasterizerState* rs;
						static ID3D11SamplerState** ss;
						static ID3D11DepthStencilState* dsState;
						static ID3D11BlendState* bs;
					};

					struct Gather
					{
						static Core::PipelineManager::RenderPass AnimatedPass;
						static Core::PipelineManager::RenderPass InstancedPass;
						static Core::Buffer ModelData;
						static Core::Buffer AnimationData;
						static Core::Buffer InstancedData;
						static ID3D11DepthStencilView* NoDepthView;
					};

					struct Light
					{
						static Core::PipelineManager::RenderPass Pass;
						static Core::Buffer LightConstantsData;
						static Core::Buffer PointLightsData;
						static ID3D11ShaderResourceView* PointLightView;

						static ID3D11ShaderResourceView* SSAOKernel;
						static ID3D11ShaderResourceView* SSAORandom;

						static ID3D11ShaderResourceView* Up;
						static ID3D11ShaderResourceView* Down;
						static ID3D11ShaderResourceView* Left;
						static ID3D11ShaderResourceView* Right;
						static ID3D11ShaderResourceView* Front;
						static ID3D11ShaderResourceView* Back;

						static ID3D11ShaderResourceView* NoDepth;
					};

					struct Gui
					{
						static Core::PipelineManager::RenderPass Pass;
						static Core::Buffer Data;
						static ID3D11DepthStencilView* depth;
						struct Text
						{
							static Core::PipelineManager::RenderPass Pass;
							static Core::Buffer Vertex;
							static ID3D11ShaderResourceView* Font;
						};
					};

					struct Blur
					{
						static Core::PipelineManager::RenderPass HorPass;
						static Core::PipelineManager::RenderPass VertPass;

						
						//Blur UAV and SRV
						static ID3D11UnorderedAccessView* BufferUAV;
						static ID3D11ShaderResourceView* BufferSRV;

						static Core::Buffer Data;
					};

					struct Post
					{
						static Core::PipelineManager::RenderPass Pass;
						static Core::Buffer Data;
					};

					

					static Core::Init::State Init();
					static Core::Init::State InitShaders();
					static Core::Init::State ReInitViews(Math::Float2);
					static Core::Init::State InitRenderStates();
					static Core::Init::State InitBuffers();
					static Core::Init::State InitViews();
					static Core::Init::State InitPasses();
					static void Clean();
				};
		}
	}
}