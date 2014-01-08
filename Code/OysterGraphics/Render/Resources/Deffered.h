#pragma once

#include "../../Core/Core.h"

namespace Oyster
{
	namespace Graphics
	{
		namespace Render
		{
			namespace Resources
			{
				class Deffered
				{
				public:
					
					static const int GBufferSize = 2;
					static const int LBufferSize = 3;
					static const int MaxLightSize = 100;
					//! GBuffers
					//! 0 = Diffuse + SpecKoeff
					//! 1 = Normal + Glow
					static ID3D11RenderTargetView* GBufferRTV[GBufferSize];
					static ID3D11ShaderResourceView* GBufferSRV[GBufferSize];

					
					static ID3D11UnorderedAccessView* LBufferUAV[LBufferSize];
					static ID3D11ShaderResourceView* LBufferSRV[LBufferSize];

					static Core::PipelineManager::RenderPass GeometryPass;
					static Core::PipelineManager::RenderPass LightPass;
					static Core::PipelineManager::RenderPass PostPass;


					static Core::Buffer ModelData;
					static Core::Buffer VPData;

					static Core::Buffer LightConstantsData;

					static Core::Buffer PointLightsData;
					static ID3D11ShaderResourceView* PointLightView;

					static ID3D11ShaderResourceView* SSAOKernel;

					static ID3D11ShaderResourceView* SSAORandom;

					static Core::Init::State Init();
					static Core::Init::State InitShaders();
					static void Clean();
				};
			}
		}
	}
}