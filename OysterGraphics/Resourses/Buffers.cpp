#include "Buffers.h"

namespace Oyster
{
	namespace Resources
	{
		Buffer Buffers::V2DSprites = Buffer();
		Buffer Buffers::CbufferVS = Buffer();
		Buffer Buffers::CBufferGs = Buffer();
		Buffer Buffers::CBufferPipelineCs = Buffer();

		void Buffers::Init()
		{
			Buffer::BUFFER_INIT_DESC desc;

			desc.ElementSize=sizeof(Math::Float2);
			desc.NumElements=1;
			desc.Type = Buffer::BUFFER_TYPE::VERTEX_BUFFER;
			desc.Usage = Buffer::BUFFER_USAGE::BUFFER_DEFAULT;
			desc.InitData = &Math::Float2(0,0);

			V2DSprites.Init(desc);

			desc.Type=Buffer::BUFFER_TYPE::CONSTANT_BUFFER_VS;
			desc.Usage = Buffer::BUFFER_USAGE::BUFFER_CPU_WRITE_DISCARD;
			desc.ElementSize=sizeof(Math::Float4x4);
			desc.InitData=0;

			CbufferVS.Init(desc);

			desc.Type = Buffer::BUFFER_TYPE::CONSTANT_BUFFER_GS;

			CBufferGs.Init(desc);

			desc.ElementSize=sizeof(Oyster::Resources::BufferDefinitions::LightStructureBuffer);
			desc.NumElements=1;
			desc.Type = Buffer::BUFFER_TYPE::CONSTANT_BUFFER_CS;
			desc.Usage = Buffer::BUFFER_USAGE::BUFFER_CPU_WRITE_DISCARD;
			desc.InitData = NULL;

			CBufferPipelineCs.Init(desc);
		}
	}
}