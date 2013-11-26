#pragma once
#include <string>
#include "..\Model\Model.h"
#include "OysterMath.h"
#include <Windows.h>


namespace Oyster
{
	namespace Graphics
	{
		class API
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

			State Init(HWND Window, bool MSAA_Quality, bool Fullscreen);
			//! @brief from Oyster::Math Float4x4, expects corect methods
			static void NewFrame(Oyster::Math::Float4x4 View, Oyster::Math::Float4x4 Projection);
			static void RenderScene(Oyster::Graphics::Model::Model* models, int count);
			static void EndFrame();

			static Oyster::Graphics::Model::Model* CreateModel(std::wstring filename);
			static void DeleteModel(Oyster::Graphics::Model::Model* model);

			static State SetOptions(Option);
		};
	}
}
