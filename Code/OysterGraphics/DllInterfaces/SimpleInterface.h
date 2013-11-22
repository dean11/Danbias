#pragma once
#include "..\Model\Model.h"


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
	static void NewFrame(float View[16], float Projection[16]);
	static void RenderScene(Oyster::Graphics::Render::Model* models, int count);
	static void EndFrame();

	static Oyster::Graphics::Render::Model* CreateModel();

	static State SetOptions(Option);
	
};
