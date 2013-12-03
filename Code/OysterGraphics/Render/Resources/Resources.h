#pragma once

#ifndef Reources_h
#define Reources_h

#include <map>
#include "../OysterGraphics/Core/Core.h"

namespace Oyster
{
	namespace Graphics
	{
		namespace Render
		{
			class Resources
			{
			public:
				static Core::ShaderManager::ShaderEffect obj;
				static Core::Buffer ModelData;
				static Core::Buffer VPData;

				static Core::Init::State Init();
				static void Clean();
			};
		}
	}
}

#endif