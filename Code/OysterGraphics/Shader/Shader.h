#pragma once

#include "..\Core\Buffer.h"

namespace Oyster
{
	namespace Graphics
	{
		class Shader
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
			static bool InitShaders(const std::string &name = "..\\Shaders\\ShaderConfig.txt");
		
			static void SetShaderEffect(ShaderEffect);

			static void CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC *desc, int ElementCount,int VertexIndex,ID3D11InputLayout *&Layout);

			struct Set
			{
				static void SetPixel(int Index);
				static void SetVertex(int Index);
				static void SetGeometry(int Index);
				static void SetCompute(int Index);
				static void SetHull(int Index);
				static void SetDomain(int Index);
			};

			struct Get
			{
				static int GetPixel(std::string Name);
				static int GetVertex(std::string Name);
				static int GetGeometry(std::string Name);
				static int GetCompute(std::string Name);
				static int GetHull(std::string Name);
				static int GetDomain(std::string Name);
			};

			static std::stringstream* AccesLog();
		};
	}
}