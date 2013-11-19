#pragma once
#ifndef CORE_INCLUDE
#define CORE_INCLUDE

#define NOMINMAX // Because I hate Microsoft now. ~Angry Dan.
// http://lolengine.net/blog/2011/3/4/fuck-you-microsoft-near-far-macros
#include <windows.h>
#include <D3D11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")


#include <string>
#include <vector>
#include <map>
#include <fstream>

#define SAFE_RELEASE(x) if( x ) { (x)->Release(); (x) = NULL; }
#define SAFE_DELETE(x) if( x ) { delete(x); (x) = NULL; }
#define SAFE_DELETE_ARRAY(x) if( x ) { delete[](x); (x) = NULL; }
#define PI (3.14159265358979323846f)

#endif