#pragma once
#ifndef CORE_INCLUDE
#define CORE_INCLUDE

#define NOMINMAX // Because I hate Microsoft now. ~Angry Dan.
// http://lolengine.net/blog/2011/3/4/fuck-you-microsoft-near-far-macros
#include <WinSock2.h>
#include <windows.h>
#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX10math.h>
#include <d3dx11effect.h>
#include <d3dCompiler.h>

#include <string>
#include <vector>
#include <map>
#include <fstream>

#define SAFE_RELEASE(x) if( x ) { (x)->Release(); (x) = NULL; }
#define SAFE_DELETE(x) if( x ) { delete(x); (x) = NULL; }
#define SAFE_DELETE_ARRAY(x) if( x ) { delete[](x); (x) = NULL; }
#define PI (3.14159265358979323846f)

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment (lib,"dxerr.lib")

#ifdef _DEBUG
#pragma comment(lib, "d3dx11d.lib")
#pragma comment(lib, "Effects11D.lib")
#pragma comment(lib, "d3dx10d.lib")
#else
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "Effects11.lib")
#pragma comment(lib, "d3dx10.lib")
#endif


#endif