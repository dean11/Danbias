#pragma once

#include "../EngineIncludes.h"

namespace Oyster
{
	namespace Resources
	{
		struct Buffers
		{
			static Buffer V2DSprites;

			static Buffer CbufferVS;

			static Buffer CBufferGs;

			static Buffer CBufferPipelineCs;

			static void Init();
		};
	}
}