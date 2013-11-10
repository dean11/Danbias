#pragma once

#ifndef PipeLineResources_H
#define PipeLineResources_H

#include "..\EngineIncludes.h"

namespace Oyster
{
	namespace Resources
	{
		struct PipeLineResourses
		{
			//0 = Diffuse
			//1 = Specular
			//2 = Glow
			//3 = Pos
			//4 = Normal
			static ID3D11ShaderResourceView* GeometryOut[5];
			static ID3D11RenderTargetView* GeometryTarget[5];


			//0 = TileBuffer
			//1 = PointList
			//2 = Random
			//3 = Sphere
			static ID3D11ShaderResourceView* ComputeResources[4];
			static Oyster::Buffer* Resources[2];


			//0 = Diffuse
			//1 = Specular
			//2 = Glow
			//3 = SSAO
			static ID3D11ShaderResourceView* LightOut[4];
			static ID3D11UnorderedAccessView* LightTarget[4];

			//0 = BlurTempStorage
			static ID3D11UnorderedAccessView* TempUav;
			static ID3D11ShaderResourceView* TempSrv;

			static ID3D11RenderTargetView* RtvNulls[16];
			static ID3D11ShaderResourceView* SrvNulls[16];
			static ID3D11UnorderedAccessView* uavNULL[16];

			static Oyster::Collision3D::Frustrum* SubFrustrums;
			static int FrustrumSize;
			static LinearAlgebra::Vector3<unsigned int> FrustrumDimensions;

			static Oyster::Resources::BufferDefinitions::LightStructureBuffer LightData;

			static void Init(int sizeX, int sizeY);

			static void InitGeometry(int sizeX, int sizeY);

			static void InitSSAOData();
			static void InitSubFrustrums(int sizeX, int sizeY);
			static void InitPointLights();
			static void InitLightData(); 
			
			static void InitLighting(int sizeX, int sizeY);
		};
	}
}

#endif