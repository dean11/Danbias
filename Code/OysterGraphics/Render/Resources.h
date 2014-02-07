#pragma once

#include "../Core/Core.h"

namespace Oyster
{
	namespace Graphics
	{
		namespace Render
		{
				class Resources
				{
				public:
					
					static const int GBufferSize = 3;
					static const int LBufferSize = 3;
					static const int MaxLightSize = 100;

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


					struct RenderStates
					{
						static ID3D11RasterizerState* rs;
						static ID3D11SamplerState** ss;
						static ID3D11DepthStencilState* dsState;
					};

					struct Gather
					{
						static Core::PipelineManager::RenderPass Pass;
						static Core::Buffer ModelData;
						static Core::Buffer AnimationData;
					};

					struct Light
					{
						static Core::PipelineManager::RenderPass Pass;
						static Core::Buffer LightConstantsData;
						static Core::Buffer PointLightsData;
						static ID3D11ShaderResourceView* PointLightView;

						static ID3D11ShaderResourceView* SSAOKernel;
						static ID3D11ShaderResourceView* SSAORandom;
					};

					struct Gui
					{
						static Core::PipelineManager::RenderPass Pass;
						static Core::Buffer Data;
					};

					struct Blur
					{
						static Core::PipelineManager::RenderPass HorPass;
						static Core::PipelineManager::RenderPass VertPass;

						
						//Blur UAV and SRV
						static ID3D11UnorderedAccessView* BufferUAV;
						static ID3D11ShaderResourceView* BufferSRV;
					};

					
					static Core::PipelineManager::RenderPass PostPass;

					

					static Core::Init::State Init();
					static Core::Init::State InitShaders();
					static Core::Init::State InitRenderStates();
					static Core::Init::State InitBuffers();
					static Core::Init::State InitViews();
					static Core::Init::State InitPasses();
					static void Clean();
				};
		}
	}
}