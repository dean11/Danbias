#pragma once

#ifndef Core_h
#define Core_h


#include "CoreIncludes.h"
#include <sstream>
namespace Oyster
{
	class Core
	{
	public:

		static ID3D11Device* Device;

		static ID3D11DeviceContext* DeviceContext;

		static IDXGISwapChain* SwapChain;

		static std::stringstream* AccesLog();

		static bool Init(bool SingleThreaded,bool Reference,bool ForceDX11);

		static bool CreateSwapChain(HWND Window, int NrofBuffers,bool MSAA_Quality,bool Fullscreen);
	};
}


#endif