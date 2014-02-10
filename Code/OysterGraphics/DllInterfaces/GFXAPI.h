#pragma once
#include <string>
#include "../Definitions/GraphicalDefinition.h"
#include "..\Model\Model.h"
#include "OysterMath.h"
#include <Windows.h>
//#include <vld.h>

#ifdef GFX_DLL_EXPORT
	#define GFX_DLL_USAGE __declspec(dllexport)
#else
	#define GFX_DLL_USAGE
#endif

namespace Oyster
{
	namespace Graphics
	{
		class GFX_DLL_USAGE API
		{
		public:
			enum State
			{
				Sucsess,
				Fail
			};
			struct Option
			{
				std::wstring modelPath, texturePath;
				int BytesUsed;
			};
			typedef void* Texture;

			static State Init(HWND Window, bool MSAA_Quality, bool Fullscreen, Oyster::Math::Float2 StartResulotion);
#ifdef _DEBUG
			static State ReloadShaders();
#endif
			
			//! @todo Memory Leaks

			//! @brief Clean Resources and release all memory
			static void Clean();

			//! @brief Sets the view matrix to use next frame
			static void SetView(Oyster::Math::Float4x4& View);
			//! @brief Sets the projection matrix to use next frame
			static void SetProjection(Oyster::Math::Float4x4& Projection);

			//! @brief will internally use last values from SetView and SetProjection
			static void NewFrame();
			//! @brief Renders a list of models
			static void RenderScene(Oyster::Graphics::Model::Model models[], int count);
			//! @brief Renders a single model
			static void RenderModel(Oyster::Graphics::Model::Model& model);

			//! @brief Configures Renderer to process 2D graphics, data will be passed in to EndFrame()
			static void StartGuiRender();

			//! @brief Renders a single GUI element using the texture provided and the Pos in the center, %based system
			static void RenderGuiElement(Texture, Math::Float2 Pos, Math::Float2 Size);

			//! @brief Performs light calculations, post effects and presents the scene
			static void EndFrame();

			//! @brief Creates a model from the supplied file, note: do not include .obj
			static Oyster::Graphics::Model::Model* CreateModel(std::wstring filename);
			//! @brief deletes a model and relases the models resources
			static void DeleteModel(Oyster::Graphics::Model::Model* model);

			static Texture CreateTexture(std::wstring filename);
			static void DeleteTexture(Texture);

			//! @brief adds a light to the scene
			static void AddLight(Definitions::Pointlight* light);
			//! @brief removes all lights from the scene
			static void ClearLights();

			//! @brief Sets Options to the graphics
			static State SetOptions(Option);

			//! @brief Gets Options from the graphics
			static Option GetOption();

			//! @brief Starts an animation and returns the time of the animation
			static float PlayAnimation(Model::Model* model, std::wstring name, bool looping = false);

			//! @brief Moves all animating models forward the specified time;
			static void Update(float deltaTime);
		};
	}
}
