#pragma once

#include <map>
#include "..\Core\Core.h"

namespace Oyster
{
	namespace Graphics
	{
		namespace Render
		{
			static class Resources
			{
				const Core::ShaderManager::ShaderEffect basic;
				const Buffer ModelData;

				Core::Init::State Init();
			};
		}
	}
}