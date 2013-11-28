#pragma once
#include "..\..\Misc\Resource\OysterResource.h"
namespace Oyster
{
	namespace Graphics
	{
		namespace Loading
		{
			void UnloadTexture(void* loadedData);
			Oyster::Resource::CustomData& LoadTexture(const wchar_t filename[]);

			void UnloadShader(void* loadedData);
			Oyster::Resource::CustomData& LoadShader(const wchar_t filename[]);
		}
	}
}