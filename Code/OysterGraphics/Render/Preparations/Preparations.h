#pragma once

#ifndef Preparations_h
#define Preparations_h

#include "..\..\Core\Core.h"

namespace Oyster
{
	namespace Graphics
	{
		namespace Render
		{
			namespace Preparations
			{
				class Basic
				{
				public:
					
					/// @brief Binds the backbuffer as a RenderTargetView with the default DepthStencil
					//! Binds the backbuffer as a RenderTargetView with the default DepthStencil

					static void BindBackBufferRTV();
					/** @brief Binds the backbuffer as a RenderTargetView with the specified DepthStencil*/
					static void BindBackBufferRTV(ID3D11DepthStencilView* depthStencil);
					

					/** @brief Binds the backbuffer as a UnorderedAccessView*/
					static void BindBackBufferUAV();

					/** @brief Binds the specified RenderTargetViews with the default DepthStencil*/
					static void BindRTV(ID3D11RenderTargetView* RTVs[], int size, bool UseDepthStencil = true);
					/** @brief Binds the specified RenderTargetViews with the specified DepthStencil*/
					static void BindRTV(ID3D11RenderTargetView* RTVs[], int size,ID3D11DepthStencilView* depthStencil);

					/** @brief Binds the specified UnorderedAccessViews*/
					static void BindUAV(ID3D11UnorderedAccessView* UAVs[], int size);

					/** @brief Clear the BackBuffer and the default DepthStencil*/
					static void ClearBackBuffer(Oyster::Math::Float4 Color);

					/** @brief Clear the specified RenderTargetViews*/
					static void ClearRTV(ID3D11RenderTargetView* RTVs[], int size,Oyster::Math::Float4 Color);
					/** @brief Clear the specified DepthStencil*/
					static void ClearDepthStencil(ID3D11DepthStencilView* depthStencil);

					/** @brief Binds the default ViewPort*/
					static void SetViewPort();
				};
			}
		}
	}
}

#endif