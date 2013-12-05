#include "Core.h"
#include <fstream>
#include <map>
#include "../FileLoader/GeneralLoader.h"
#include "Resource\OysterResource.h"

const char* ShaderFunction = "main";

namespace Oyster
{
	namespace Graphics
	{
		namespace
		{
			std::vector<ID3D11PixelShader*> PS;
			std::map<std::wstring,int> PSMap;

			std::vector<ID3D11GeometryShader*> GS;
			std::map<std::wstring,int> GSMap;

			std::vector<ID3D11ComputeShader*> CS;
			std::map<std::wstring,int> CSMap;

			std::vector<ID3D11DomainShader*> DS;
			std::map<std::wstring,int> DSMap;

			std::vector<ID3D11HullShader*> HS;
			std::map<std::wstring,int> HSMap;

			std::vector<ID3D11VertexShader*> VS;
			std::vector<Core::ShaderManager::ShaderData> VData;
			std::map<std::wstring,int> VSMap;
		}

		#pragma region Init
		bool Core::ShaderManager::Init(std::wstring filename, ShaderType type, std::wstring name)
		{
			void* data;
			bool ForceReload;
#if defined (_DEBUG) | defined (DEBUG)
			ForceReload = true;
#else
			ForceReload = false;
#endif
			switch (type)
			{
			case Oyster::Graphics::Core::ShaderManager::Vertex:
				if(!VSMap.count(name) || ForceReload)
				{
					data = Resource::OysterResource::LoadResource(filename.c_str(),Loading::LoadShaderV, -1, ForceReload);
					if(data)
					{
						VSMap[name] = VS.size();
						VS.push_back((ID3D11VertexShader*)data);
					}
				}
				break;
			case Oyster::Graphics::Core::ShaderManager::Hull:
				data = Resource::OysterResource::LoadResource(filename.c_str(),Loading::LoadShaderH, -1, ForceReload);
				if(!HSMap.count(name) || ForceReload)
				{
					if(data!=0)
					{
						HSMap[name] = HS.size();
						HS.push_back((ID3D11HullShader*)data);
					}
				}
				break;
			case Oyster::Graphics::Core::ShaderManager::Domain:
				data = Resource::OysterResource::LoadResource(filename.c_str(),Loading::LoadShaderD, -1, ForceReload);
				if(!DSMap.count(name) || ForceReload)
				{
					if(data!=0)
					{
						DSMap[name] = DS.size();
						DS.push_back((ID3D11DomainShader*)data);
					}
				}
				break;
			case Oyster::Graphics::Core::ShaderManager::Geometry:
				data = Resource::OysterResource::LoadResource(filename.c_str(),Loading::LoadShaderG, -1, ForceReload);
				if(!GSMap.count(name) || ForceReload)
				{
					if(data!=0)
					{
						GSMap[name] = GS.size();
						GS.push_back((ID3D11GeometryShader*)data);
					}
				}
				break;
			case Oyster::Graphics::Core::ShaderManager::Pixel:
				data = Resource::OysterResource::LoadResource(filename.c_str(),Loading::LoadShaderP, -1, ForceReload);
				if(!PSMap.count(name) || ForceReload)
				{
					if(data!=0)
					{
						PSMap[name] = PS.size();
						PS.push_back((ID3D11PixelShader*)data);
					}
				}
				break;
			case Oyster::Graphics::Core::ShaderManager::Compute:
				data = Resource::OysterResource::LoadResource(filename.c_str(),Loading::LoadShaderC, -1, ForceReload);
				if(!CSMap.count(name) || ForceReload)
				{
					if(data!=0)
					{
						CSMap[name] = CS.size();
						CS.push_back((ID3D11ComputeShader*)data);
					}
				}
				break;
			default:
				break;
			}
			return true;
		}

		void* Core::ShaderManager::CreateShader(Core::ShaderManager::ShaderData data, Core::ShaderManager::ShaderType type)
		{
			
			HRESULT hr;
			switch (type)
			{
			case Oyster::Graphics::Core::ShaderManager::Vertex:
				ID3D11VertexShader* vs;
				hr = Core::device->CreateVertexShader(data.data,data.size,NULL,&vs);
				if(hr == S_OK)
					VData.push_back(data);
				return vs;
				break;
			case Oyster::Graphics::Core::ShaderManager::Hull:
				ID3D11HullShader* hs;
				Core::device->CreateHullShader(data.data,data.size,NULL,&hs);
				delete[] data.data;
				return hs;
				break;
			case Oyster::Graphics::Core::ShaderManager::Domain:
				ID3D11DomainShader* ds;
				Core::device->CreateDomainShader(data.data,data.size,NULL,&ds);
				delete[] data.data;
				return ds;
				break;
			case Oyster::Graphics::Core::ShaderManager::Geometry:
				ID3D11GeometryShader* gs;
				Core::device->CreateGeometryShader(data.data,data.size,NULL,&gs);
				delete[] data.data;
				return gs;
				break;
			case Oyster::Graphics::Core::ShaderManager::Pixel:
				ID3D11PixelShader* ps;
				Core::device->CreatePixelShader(data.data,data.size,NULL,&ps);
				delete[] data.data;
				return ps;
				break;
			case Oyster::Graphics::Core::ShaderManager::Compute:
				ID3D11ComputeShader* cs;
				Core::device->CreateComputeShader(data.data,data.size,NULL,&cs);
				delete[] data.data;
				return cs;
				break;
			}
			return NULL;
		}
	#pragma endregion

		void Core::ShaderManager::CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC *desc, int ElementCount,int VertexIndex,ID3D11InputLayout *&Layout)
		{
			if(VertexIndex==-1)
			{
				Layout=0;
				return;
			}
			Core::device->CreateInputLayout(desc,ElementCount,VData[VertexIndex].data,VData[VertexIndex].size,&Layout);
		}

		#pragma region Get
		int Core::ShaderManager::Get::Pixel(std::wstring Name)
		{
			if(PSMap.count(Name))
				return PSMap[Name];
			return -1;
		}

		int Core::ShaderManager::Get::Vertex(std::wstring Name)
		{
			if(VSMap.count(Name))
				return VSMap[Name];
			return -1;
		}
		int Core::ShaderManager::Get::Geometry(std::wstring Name)
		{
			if(GSMap.count(Name))
				return GSMap[Name];
			return -1;
		}

		int Core::ShaderManager::Get::Compute(std::wstring Name)
		{
			if(CSMap.count(Name))
				return CSMap[Name];
			return -1;
		}
		int Core::ShaderManager::Get::Hull(std::wstring Name)
		{
			return -1;
		}
		int Core::ShaderManager::Get::Domain(std::wstring Name)
		{
			return -1;
		}
	#pragma endregion

		#pragma region Set
		/// \todo smart set
		void Core::ShaderManager::Set::Pixel(int Index)
		{
			if(Index==-1)
				Core::deviceContext->PSSetShader( NULL,NULL,0);
			else
				Core::deviceContext->PSSetShader( PS[Index],NULL,0);
		}
		void Core::ShaderManager::Set::Vertex(int Index)
		{
			if(Index==-1)
				Core::deviceContext->VSSetShader( NULL,NULL,0);
			else
				Core::deviceContext->VSSetShader( VS[Index],NULL,0);
		}
		void Core::ShaderManager::Set::Geometry(int Index)
		{
			if(Index==-1)
				Core::deviceContext->GSSetShader( NULL,NULL,0);
			else
				Core::deviceContext->GSSetShader( GS[Index],NULL,0);
		}
		void Core::ShaderManager::Set::Compute(int Index)
		{
			if(Index==-1)
				Core::deviceContext->CSSetShader( NULL,NULL,0);
			else
				Core::deviceContext->CSSetShader( CS[Index],NULL,0);
		}
		/// \todo set Hull
		void Core::ShaderManager::Set::Hull(int Index)
		{
			return;
		}
		/// \todo set Domain
		void Core::ShaderManager::Set::Domain(int Index)
		{
			return;
		}
	#pragma endregion

		/// \todo smart Set ie. not resetting the shader
		/// \todo research states
		/// \todo smart buffer set
		void Core::ShaderManager::SetShaderEffect(ShaderEffect se)
		{
			Set::Pixel(se.Shaders.Pixel);
			Set::Vertex(se.Shaders.Vertex);
			Set::Geometry(se.Shaders.Geometry);
			Set::Compute(se.Shaders.Compute);
			Core::deviceContext->IASetInputLayout(se.IAStage.Layout);
			Core::deviceContext->IASetPrimitiveTopology(se.IAStage.Topology);
			for(unsigned int i=0;i<se.CBuffers.Vertex.size();++i)
				se.CBuffers.Vertex[i]->Apply(i);
			for(unsigned int i=0;i<se.CBuffers.Geometry.size();++i)
				se.CBuffers.Geometry[i]->Apply(i);
			for(unsigned int i=0;i<se.CBuffers.Pixel.size();++i)
				se.CBuffers.Pixel[i]->Apply(i);
			Core::deviceContext->RSSetState(se.RenderStates.Rasterizer);
			Core::deviceContext->PSSetSamplers(0,se.RenderStates.SampleCount,se.RenderStates.SampleState);
			Core::deviceContext->OMSetDepthStencilState(se.RenderStates.DepthStencil,0);
			float test[4] = {0};
			Core::deviceContext->OMSetBlendState(se.RenderStates.BlendState,test,-1);
		}

		void Core::ShaderManager::Clean()
		{
			for(int i = 0; i < (int)VData.size(); ++i)
			{
				delete[] VData[i].data;
			}
		}
	}
}