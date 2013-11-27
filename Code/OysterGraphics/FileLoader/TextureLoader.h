#pragma once
#include "..\..\Misc\Resource\OysterResource.h"
namespace Oyster
{
	namespace Graphics
	{
		namespace Loading
		{
			void UnloadTexture(void* loadedData);
			const Oyster::Resource::CustomData& LoadTexture(const wchar_t filename[]);
		}
	}
}