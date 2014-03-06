#pragma once

#ifndef Core_h
#define Core_h


#include "Dx11Includes.h"
#include <sstream>
#include "OysterMath.h"
#include "Resource/ResourceManager.h"
//#include <vld.h>

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

			static Resource::ResourceManager loader;


			//BackBufferRTV
			static ID3D11RenderTargetView* backBufferRTV;
			//BackBufferUAV
			static ID3D11UnorderedAccessView* backBufferUAV;
			//DepthStencil
			static ID3D11DepthStencilView* depthStencil;
			static ID3D11ShaderResourceView* depthStencilUAV;
			//ViewPort
			static D3D11_VIEWPORT* viewPort;


			static ID3D11ShaderResourceView* srvNULL[16];
			static ID3D11RenderTargetView* rtvNULL[8];
			static ID3D11UnorderedAccessView* uavNULL[8];

			static std::map<ID3D11ShaderResourceView*,int> TexSize;

			static int UsedMem;
			static HWND window;
			static Oyster::Math::Float2 resolution;
			static float amb;
			static Math::Float3 gTint;
			static Math::Float3 gGTint;
			static bool fullscreen;
			static std::wstring modelPath;
			static std::wstring texturePath;

			class Buffer
			{
				public:
				enum BUFFER_TYPE
				{
					VERTEX_BUFFER,
					INDEX_BUFFER,
					CONSTANT_BUFFER_VS,
					CONSTANT_BUFFER_GS,
					CONSTANT_BUFFER_PS,
					CONSTANT_BUFFER_CS,
					STRUCTURED_BUFFER,
					BUFFER_TYPE_COUNT
				};

				enum BUFFER_USAGE
				{
					BUFFER_DEFAULT,
					BUFFER_STREAM_OUT_TARGET,
					BUFFER_CPU_WRITE,
					BUFFER_CPU_WRITE_DISCARD,
					BUFFER_CPU_READ,
					BUFFER_USAGE_COUNT,
					BUFFER_USAGE_IMMUTABLE
				};

				struct BUFFER_INIT_DESC
				{
					BUFFER_TYPE		Type;
					UINT32			NumElements;
					UINT32			ElementSize;
					BUFFER_USAGE	Usage;
					void*			InitData;

					BUFFER_INIT_DESC()
					{
						InitData = NULL;
						Usage = BUFFER_DEFAULT;
					}
				};
			protected:
				ID3D11Buffer*	mBuffer;
				BUFFER_TYPE		mType;
				BUFFER_USAGE	mUsage;

				UINT32			mElementSize;
				UINT32			mElementCount;
			public:
				Buffer();
				virtual ~Buffer();

				HRESULT Init(const BUFFER_INIT_DESC& initDesc);

				void* Map();
				void Unmap();

				operator ID3D11Buffer*();
				operator const ID3D11Buffer*() const;

				HRESULT Apply(UINT32 misc = 0) const;

				ID3D11Buffer* GetBufferPointer();
				UINT32 GetVertexSize();
				UINT32 GetElementCount();
			};
		

			class PipelineManager
			{
			public:
				struct RenderPass
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
						std::vector<ID3D11Buffer*> Vertex;
						std::vector<ID3D11Buffer*> Geometry;
						std::vector<ID3D11Buffer*> Pixel;
						std::vector<ID3D11Buffer*> Compute;
					}CBuffers;

					struct
					{
						std::vector<ID3D11ShaderResourceView*> Vertex;
						std::vector<ID3D11ShaderResourceView*> Geometry;
						std::vector<ID3D11ShaderResourceView*> Pixel;
						std::vector<ID3D11ShaderResourceView*> Compute;
					}SRV;
					
					std::vector<ID3D11RenderTargetView*> RTV;

					struct
					{
						std::vector<ID3D11UnorderedAccessView*> Pixel;
						std::vector<ID3D11UnorderedAccessView*> Compute;
					}UAV;

					ID3D11DepthStencilView* depth;

					RenderPass()
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
				struct ShaderData
				{
					size_t size;
					char* data;
				};

				static void SetRenderPass(RenderPass);

				static void CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC *desc, int ElementCount,int VertexIndex,ID3D11InputLayout *&Layout);

				static bool Init(std::wstring filename, ShaderType type, std::wstring name);
				static void* CreateShader(ShaderData data, ShaderType type);

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

				static void Clean();
				static void CleanPipeline();
			};

			//Set resulotion Before Calling Full Init
			class Init
			{
			public:
				enum State
				{
					Success,
					Fail
				};

				static State CreateDeviceAndDeviceContext(bool SingleThreaded = true, bool Reference = false, bool ForceDX11 = true);

				static State CreateSwapChain(HWND Window, int NrofBuffers,bool MSAA_Quality,bool Fullscreen, Oyster::Math::Float2 Size);

				static State CreateDepthStencil(bool MSAA_Quality, Oyster::Math::Float2 Size);

				static State CreateBackBufferViews();

				static State CreateViewPort(Oyster::Math::Float2 Origin, Oyster::Math::Float2 Size);

				static State FullInit(HWND Window, bool MSAA_Quality, bool Fullscreen);

				static State ReInitialize(bool MSAA_Quality, bool Fullscreen, Math::Float2 Size);

				static State CreateLinkedShaderResourceFromTexture(ID3D11RenderTargetView** rtv, ID3D11ShaderResourceView** srv, ID3D11UnorderedAccessView** uav);

				static State CreateLinkedShaderResourceFromStructuredBuffer(Buffer** Structured, ID3D11ShaderResourceView** srv, ID3D11UnorderedAccessView** uav);

				static State CreateLinkedShaderResourceFromStructuredBuffer(void* InitData, int numElem, int ElemSize, Buffer** Structured, ID3D11ShaderResourceView** srv, ID3D11UnorderedAccessView** uav);

			};
		};
	}
}


#endif