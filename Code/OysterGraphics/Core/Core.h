#pragma once

#ifndef Core_h
#define Core_h


#include "CoreIncludes.h"
#include <sstream>
#include "Buffer.h"
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
					std::vector<Oyster::Buffer*> Vertex;
					std::vector<Oyster::Buffer*> Geometry;
					std::vector<Oyster::Buffer*> Pixel;
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
	};
}


#endif