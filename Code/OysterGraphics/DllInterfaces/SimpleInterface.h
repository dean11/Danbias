#pragma once
#include <string>
#include "..\Model\Model.h"
#include "OysterMath.h"


class SimpleInterface
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
	static void RenderScene(Oyster::Graphics::Render::Model* models, int count);
	static void EndFrame();

	static Oyster::Graphics::Render::Model* CreateModel(std::wstring filename);

	static State SetOptions(Option);
	
};
