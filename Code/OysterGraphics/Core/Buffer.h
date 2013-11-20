#pragma once
#ifndef CoreBuffer
#define CoreBuffer

#include "CoreIncludes.h"

namespace Oyster
{
	namespace Graphics
	{
		class Buffer
		{
		public:
			enum BUFFER_TYPE
			{
				VERTEX_BUFFER,
				INDEX_BUFFER,
				CONSTANT_BUFFER_VS,
				CONSTANT_BUFFER_GS,
				CONSTANT_BUFFER_PS,
				CONSTANT_BUFFER_CS,
				STRUCTURED_BUFFER,
				BUFFER_TYPE_COUNT
			};

			enum BUFFER_USAGE
			{
				BUFFER_DEFAULT,
				BUFFER_STREAM_OUT_TARGET,
				BUFFER_CPU_WRITE,
				BUFFER_CPU_WRITE_DISCARD,
				BUFFER_CPU_READ,
				BUFFER_USAGE_COUNT,
				BUFFER_USAGE_IMMUTABLE
			};

			struct BUFFER_INIT_DESC
			{
				BUFFER_TYPE		Type;
				UINT32			NumElements;
				UINT32			ElementSize;
				BUFFER_USAGE	Usage;
				void*			InitData;

				BUFFER_INIT_DESC()
				{
					InitData = NULL;
					Usage = BUFFER_DEFAULT;
				}
			};
		protected:
			ID3D11Buffer*	mBuffer;
			BUFFER_TYPE		mType;
			BUFFER_USAGE	mUsage;

			UINT32			mElementSize;
			UINT32			mElementCount;
		public:
			Buffer();
			virtual ~Buffer();

			HRESULT Init(const BUFFER_INIT_DESC& initDesc);

			void* Map();
			void Unmap();

			operator ID3D11Buffer*();
			operator const ID3D11Buffer*() const;

			HRESULT Apply(UINT32 misc = 0) const;

			ID3D11Buffer* GetBufferPointer();
			UINT32 GetVertexSize();
			UINT32 GetElementCount();
		};
	}
}

#endif