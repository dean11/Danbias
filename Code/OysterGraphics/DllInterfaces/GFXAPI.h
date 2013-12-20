#pragma once
#include <string>
#include "../Definitions/GraphicalDefinition.h"
#include "..\Model\Model.h"
#include "OysterMath.h"
#include <Windows.h>
//#include <vld.h>

#ifdef GFX_DLL_EXPORT
	#define GFX_DLL_USAGE __declspec(dllexport)
#else
	#define GFX_DLL_USAGE
#endif

namespace Oyster
{
	namespace Graphics
	{
		class GFX_DLL_USAGE API
		{
		public:
			enum State
			{
				Sucsess,
				Fail
			};
			struct Option
			{
			};

			static State Init(HWND Window, bool MSAA_Quality, bool Fullscreen, Oyster::Math::Float2 StartResulotion);
			static void Clean();

			static void SetView(Oyster::Math::Float4x4& View);
			static void SetProjection(Oyster::Math::Float4x4& Projection);

			//! @brief will internally use last values from SetView and SetProjection
			static void NewFrame();
			static void RenderScene(Oyster::Graphics::Model::Model models[], int count);
			static void RenderModel(Oyster::Graphics::Model::Model& model);
			static void EndFrame();

			static Oyster::Graphics::Model::Model* CreateModel(std::wstring filename);
			static void DeleteModel(Oyster::Graphics::Model::Model* model);

			static void AddLight(Definitions::Pointlight light);
			static void ClearLights();

			static State SetOptions(Option);
		};
	}
}
