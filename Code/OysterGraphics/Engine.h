#pragma once

#ifndef Engine_h
#define Engine_h

#define NOMINMAX // Because I hate Microsoft now. ~Angry Dan. http://lolengine.net/blog/2011/3/4/fuck-you-microsoft-near-far-macros

#include "EngineIncludes.h"

namespace Oyster
{
	namespace Graphics
	{
		class Engine
		{
		private:
			Engine();
			~Engine();
		
		public:
			/*class Init
			{
			public:
				struct Setup
				{
					int NrOfBuffers;
					bool MSAA_Quality;
					bool Fullscreen;
					bool SingleThreaded;
					bool Reference;
					bool ForceDX11;
					bool GenerateDepthStencil;
					bool BindDefault;
					HWND window;
					//all but Window params have Default Values
					Setup()
					{
						NrOfBuffers=1;
						MSAA_Quality = false;
						Fullscreen = true;
						SingleThreaded = true;
						Reference = false;
						ForceDX11 = false;
						GenerateDepthStencil = true;
						BindDefault = true;
					}

				};

				static bool IsInstanced();

				//Creates Device and DeviceContext, if not Initialized
				static bool Instance(bool SingleThreaded=true,bool Reference=false,bool ForceDX11=false);
				static bool HasSwapChain();
			
				//Creates Swapchain, if not Aready Created
				static bool CreateSwapChain(HWND Window, int NrofBuffers=1,bool MSAA_Quality=false,bool Fullscreen=true);

				//CreateWindow, if Not Already Created
				static bool InitializeWindow(const LPCSTR appName, const LPCSTR className,const HINSTANCE &hInstance, const int &nCmdShow, WNDPROC wProc, bool HandleLoop = false );
			
				//Performs a full initialization of a rendering pipeline, including a Window
				static bool FullInit(const Setup& setup);
				struct Buffers
				{
					static Buffer* CreateBuffer(const Buffer::BUFFER_INIT_DESC BufferDesc);
				};
			private:
				Init();
				~Init();
			};*/

			class States
			{
			public:
				//SSAO Quality
				static void SetNrOfSSAOSamples(int);
				static int GetNrOfSSAOSamples();

				//SSAO Frequency
				static void SetSSAOSampleSpread(int);
				static int GetSSAOSampleSpread();

				//PointLights
				static void SetMaxPointlights(int);
				static int GetMaxPointlights();


			private:
				States();
				~States();
			};

			class Render
			{
			public:
				/// Render a number of models, setting the Per model data to the included cBuffer
				/// specify NULL if no such data exists
				//static void Geometry(const Oyster::Graphics::Render::Model* models,int count,Buffer* cBufferEveryObject, int slot);
				static void Text(std::string text, Oyster::Math::Float2 size, Oyster::Math::Float3 Pos);
				//static void TextBox(const Oyster::Render::

				//ensure that a compatible 2D shadereffect is applied
				static void ScreenQuad(ID3D11ShaderResourceView* srv, float ZPos=1);

				//ensure that a compatible 2D shadereffect is applied and that pos.z is between 0 and 1
				static void Sprite(ID3D11ShaderResourceView* srv, Oyster::Math::Float2 size, Oyster::Math::Float3 Pos);

				static void PresentScene();

			private:
				Render();
				~Render();
			};

			class PrepareForRendering
			{
			public:
				//Binds several rendertargets and a depthstencil
				static void BindRenderTargets(ID3D11RenderTargetView** RenderTargets,int NrOfTargets,ID3D11DepthStencilView* depth);

				//Binds several Rendertargest and a default depthstencil
				static void BindRenderTargets(ID3D11RenderTargetView** RenderTargets,int NrOfTargets);

				//Binds the backbuffer and a depthstencil
				static void BindBackBuffer(ID3D11DepthStencilView* depth);

				//Binds the backbuffer and a default depthstencil
				static void BindBackBuffer();

				//Binds the backbuffer to the compute shader
				static void BindBackBufferAsUAV();

				//binds several UAV to the computeshader
				static void BindUAV(ID3D11UnorderedAccessView** uav, int NrOfUavs);

				//Clears the backbuffer and default depthstencil
				static void ClearBackBuffer(Math::Float4 color);

				static void Begin2DRender();

				static void Begin2DTextRender();
			};

			class Pipeline
			{
			public:
				class Deffered_Lightning
				{
				public:
					//Basic Setup
					//static void NewFrame(const Float4& Color, const Matrix& View, const Matrix& Projection);

					//Geometry Pass
					static void BeginRenderGeometry();
					//static void RenderGeometry(const Oyster::Graphics::Render::Model* models,int count);
					static void EndRenderGeometry();

					//Lightning Pass
					//static void InputPointLights(Oyster::Resources::BufferDefinitions::PointLightDescription *p, int NrOfPointlights );
					static void RenderLightning();
				};
			};
		};
	}
};

#endif