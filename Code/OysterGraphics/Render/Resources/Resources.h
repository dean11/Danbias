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
				static Buffer ModelData;
				static Buffer VPData;

				static Core::Init::State Init();
			};
		}
	}
}

#endif