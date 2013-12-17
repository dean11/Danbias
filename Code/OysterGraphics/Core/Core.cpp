#include "Core.h"
#include <vld.h>

using namespace Oyster::Graphics;
using std::string;

//GPU
ID3D11Device *Core::device = NULL;

//API
ID3D11DeviceContext *Core::deviceContext = NULL;

//SwapChain
IDXGISwapChain* Core::swapChain = NULL;

std::stringstream Core::log;

ID3D11RenderTargetView* Core::backBufferRTV = NULL;

ID3D11UnorderedAccessView* Core::backBufferUAV = NULL;

ID3D11DepthStencilView* Core::depthStencil = NULL;
ID3D11ShaderResourceView* Core::depthStencilUAV = NULL;

D3D11_VIEWPORT* Core::viewPort = NULL;

Oyster::Math::Float2 Core::resolution = Oyster::Math::Float2::null;

ID3D11ShaderResourceView* Core::srvNULL[16] = {0};
ID3D11RenderTargetView* Core::rtvNULL[8] = {0};
ID3D11UnorderedAccessView* Core::uavNULL[8] = {0};