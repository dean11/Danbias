#pragma once
#include "..\..\Misc\Resource\OysterResource.h"
namespace Oyster
{
	namespace Graphics
	{
		namespace Loading
		{
			void UnloadTexture(void* loadedData);
			void LoadTexture(const wchar_t filename[], Oyster::Resource::CustomData& out);

			void UnloadShaderP(void* loadedData);
			void LoadShaderP(const wchar_t filename[], Oyster::Resource::CustomData& out);

			void UnloadShaderG(void* loadedData);
			void LoadShaderG(const wchar_t filename[], Oyster::Resource::CustomData& out);

			void UnloadShaderC(void* loadedData);
			void LoadShaderC(const wchar_t filename[], Oyster::Resource::CustomData& out);

			void UnloadShaderV(void* loadedData);
			void LoadShaderV(const wchar_t filename[], Oyster::Resource::CustomData& out);

			void UnloadShaderH(void* loadedData);
			void LoadShaderH(const wchar_t filename[], Oyster::Resource::CustomData& out);

			void UnloadShaderD(void* loadedData);
			void LoadShaderD(const wchar_t filename[], Oyster::Resource::CustomData& out);

			void LoadShader(const wchar_t filename[], Oyster::Resource::CustomData& out, int type);
		}
	}
}