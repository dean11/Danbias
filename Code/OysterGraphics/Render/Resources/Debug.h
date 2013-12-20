#pragma once

#ifndef Reources_h
#define Reources_h

#ifdef _DEBUG

#include "../OysterGraphics/Core/Core.h"

namespace Oyster
{
	namespace Graphics
	{
		namespace Render
		{
			namespace Resources
			{
				class Debug
				{
				public:
					//static Core::PipelineManager::RenderPass obj;
					static Core::PipelineManager::RenderPass debugCompute;
					//static Core::Buffer ModelData;
					//static Core::Buffer VPData;

					//static ID3D11ShaderResourceView* srv;
					//static ID3D11RenderTargetView*	rtv;

					static Core::Init::State Init();
					static void Clean();
				};
			}
		}
	}
}

#endif

#endif