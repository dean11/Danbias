#pragma once
namespace Oyster
{
	namespace Graphics
	{
		namespace Loading
		{
			void UnloadTexture(void* loadedData);
			void* LoadTexture(const wchar_t filename[]);

			void UnloadShaderP(void* loadedData);
			void* LoadShaderP(const wchar_t filename[]);

			void UnloadShaderG(void* loadedData);
			void* LoadShaderG(const wchar_t filename[]);

			void UnloadShaderC(void* loadedData);
			void* LoadShaderC(const wchar_t filename[]);

			void UnloadShaderV(void* loadedData);
			void* LoadShaderV(const wchar_t filename[]);

			void UnloadShaderH(void* loadedData);
			void* LoadShaderH(const wchar_t filename[]);

			void UnloadShaderD(void* loadedData);
			void* LoadShaderD(const wchar_t filename[]);

			void UnloadOBJ(void* loadedData);
			void* LoadOBJ(const wchar_t filename[]);

			void UnloadDAN(void* loadedData);
			void* LoadDAN(const wchar_t filename[]);
		}
	}
}