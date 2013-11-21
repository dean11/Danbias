#pragma once

#ifndef Core_h
#define Core_h


#include "CoreIncludes.h"
#include <sstream>
#include "Buffer.h"
#include "OysterMath.h";

namespace Oyster
{
	namespace Graphics
	{
		class Core
		{
		public:

			static ID3D11Device* device;

			static ID3D11DeviceContext* deviceContext;

			static IDXGISwapChain* swapChain;

			static std::stringstream log;

			//BackBufferRTV
			static ID3D11RenderTargetView* backBufferRTV;
			//BackBufferUAV
			static ID3D11UnorderedAccessView* backBufferUAV;
			//DepthStencil
			static ID3D11DepthStencilView* depthStencil;
			//ViewPort
			static D3D11_VIEWPORT* viewPort;

			static  Oyster::Math::Float2 resolution;
		

			class ShaderManager
			{
			public:
				struct ShaderEffect
				{
					struct
					{
						int Pixel,Vertex,Geometry,Compute,Hull,Domain;
					}Shaders;

					struct IAStage_
					{
						ID3D11InputLayout* Layout;
						D3D11_PRIMITIVE_TOPOLOGY Topology;
					}IAStage;

					struct RenderStates_
					{
						ID3D11DepthStencilState *DepthStencil;
						ID3D11RasterizerState	*Rasterizer;
						ID3D11SamplerState	**SampleState;
						int SampleCount;
						ID3D11BlendState	*BlendState;
					}RenderStates;

					struct
					{
						std::vector<Buffer*> Vertex;
						std::vector<Buffer*> Geometry;
						std::vector<Buffer*> Pixel;
					}CBuffers;

					ShaderEffect()
					{
						RenderStates.BlendState=NULL;
						RenderStates.DepthStencil=NULL;
						RenderStates.Rasterizer=NULL;
						RenderStates.SampleState=NULL;
						RenderStates.SampleCount=0;
						Shaders.Compute=-1;
						Shaders.Domain=-1;
						Shaders.Geometry=-1;
						Shaders.Hull=-1;
						Shaders.Pixel=-1;
						Shaders.Vertex=-1;
					}
				};
				enum ShaderType
				{
					Vertex,
					Hull,
					Domain,
					Geometry,
					Pixel,
					Compute
				};

				static void SetShaderEffect(ShaderEffect);

				static void CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC *desc, int ElementCount,int VertexIndex,ID3D11InputLayout *&Layout);

				static bool Init(std::wstring filename, ShaderType type, std::wstring name, bool Precompiled = true);

				struct Get
				{
					static int Pixel(std::wstring Name);
					static int Vertex(std::wstring Name);
					static int Geometry(std::wstring Name);
					static int Compute(std::wstring Name);
					static int Hull(std::wstring Name);
					static int Domain(std::wstring Name);
				};

				struct Set
				{
					static void Pixel(int Index);
					static void Vertex(int Index);
					static void Geometry(int Index);
					static void Compute(int Index);
					static void Hull(int Index);
					static void Domain(int Index);
				};
			};

			//Set resulotion Before Calling Full Init
			class Init
			{
			public:
				enum State
				{
					Sucsess,
					Fail
				};

				static State CreateDeviceAndDeviceContext(bool SingleThreaded = true, bool Reference = false, bool ForceDX11 = true);

				static State CreateSwapChain(HWND Window, int NrofBuffers,bool MSAA_Quality,bool Fullscreen, Oyster::Math::Float2 Size);

				static State CreateDepthStencil(bool MSAA_Quality, Oyster::Math::Float2 Size);

				static State CreateBackBufferViews();

				static State CreateViewPort(Oyster::Math::Float2 Origin, Oyster::Math::Float2 Size);

				static State FullInit(HWND Window, bool MSAA_Quality, bool Fullscreen);

				static State ReInitialize(HWND Window, bool MSAA_Quality, bool Fullscreen);
			};
		};
	}
}


#endif