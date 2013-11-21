#pragma once

#ifndef Reources_h
#define Reources_h

#include <map>
#include "..\Core\Core.h"

namespace Oyster
{
	namespace Graphics
	{
		namespace Render
		{
			class Resources
			{
				const Core::ShaderManager::ShaderEffect basic;
				const Buffer ModelData;

				Core::Init::State Init();
			};
		}
	}
}

#endif