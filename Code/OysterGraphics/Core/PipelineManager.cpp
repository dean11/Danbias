#include "Core.h"
#include <fstream>
#include <map>
#include "../FileLoader/GeneralLoader.h"

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
			std::vector<Core::PipelineManager::ShaderData> VData;
			std::map<std::wstring,int> VSMap;

			Core::PipelineManager::RenderPass Current;
		}

		#pragma region Init
		bool Core::PipelineManager::Init(std::wstring filename, ShaderType type, std::wstring name)
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
			case Oyster::Graphics::Core::PipelineManager::Vertex:
				if(!VSMap.count(name) || ForceReload)
				{
					data = Core::loader.LoadResource(filename.c_str(),Loading::LoadShaderV, Loading::UnloadShaderV, -1, ForceReload);
					if(data)
					{
						if(ForceReload && VSMap.count(name))
						{
							VS[VSMap[name]] = (ID3D11VertexShader*)data;
						}
						else
						{
							VSMap[name] = (int)VS.size();
							VS.push_back((ID3D11VertexShader*)data);
						}
					}
				}
				break;
			case Oyster::Graphics::Core::PipelineManager::Hull:
				data = Core::loader.LoadResource(filename.c_str(),Loading::LoadShaderH, Loading::UnloadShaderH, -1, ForceReload);
				if(!HSMap.count(name) || ForceReload)
				{
					if(data!=0)
					{
						if(ForceReload && HSMap.count(name))
						{
							HS[HSMap[name]] = (ID3D11HullShader*)data;
						}
						else
						{
							HSMap[name] = (int)HS.size();
							HS.push_back((ID3D11HullShader*)data);
						}
						
					}
				}
				break;
			case Oyster::Graphics::Core::PipelineManager::Domain:
				data = Core::loader.LoadResource(filename.c_str(),Loading::LoadShaderD, Loading::UnloadShaderD, -1, ForceReload);
				if(!DSMap.count(name) || ForceReload)
				{
					if(data!=0)
					{
						if(ForceReload && DSMap.count(name))
						{
							DS[DSMap[name]] = (ID3D11DomainShader*)data;
						}
						else
						{
							DSMap[name] = (int)DS.size();
							DS.push_back((ID3D11DomainShader*)data);
						}
					}
				}
				break;
			case Oyster::Graphics::Core::PipelineManager::Geometry:
				data = Core::loader.LoadResource(filename.c_str(),Loading::LoadShaderG, Loading::UnloadShaderG, -1, ForceReload);
				if(!GSMap.count(name) || ForceReload)
				{
					if(data!=0)
					{
						if(ForceReload && GSMap.count(name))
						{
							GS[GSMap[name]] = (ID3D11GeometryShader*)data;
						}
						else
						{
							GSMap[name] = (int)GS.size();
							GS.push_back((ID3D11GeometryShader*)data);
						}
					}
				}
				break;
			case Oyster::Graphics::Core::PipelineManager::Pixel:
				data = Core::loader.LoadResource(filename.c_str(),Loading::LoadShaderP, Loading::UnloadShaderP, -1, ForceReload);
				if(!PSMap.count(name) || ForceReload)
				{
					if(data!=0)
					{
						if(ForceReload && PSMap.count(name))
						{
							PS[PSMap[name]] = (ID3D11PixelShader*)data;
						}
						else
						{
							PSMap[name] = (int)PS.size();
							PS.push_back((ID3D11PixelShader*)data);
						}
					}
				}
				break;
			case Oyster::Graphics::Core::PipelineManager::Compute:
				data = Core::loader.LoadResource(filename.c_str(),Loading::LoadShaderC, Loading::UnloadShaderC, -1, ForceReload);
				if(!CSMap.count(name) || ForceReload)
				{
					if(data!=0)
					{
						if(ForceReload && CSMap.count(name))
						{
							CS[CSMap[name]] = (ID3D11ComputeShader*)data;
						}
						else
						{
							CSMap[name] = (int)CS.size();
							CS.push_back((ID3D11ComputeShader*)data);
						}
						
					}
				}
				break;
			default:
				break;
			}
			return true;
		}

		void* Core::PipelineManager::CreateShader(Core::PipelineManager::ShaderData data, Core::PipelineManager::ShaderType type)
		{
			
			HRESULT hr;
			switch (type)
			{
			case Oyster::Graphics::Core::PipelineManager::Vertex:
				ID3D11VertexShader* vs;
				hr = Core::device->CreateVertexShader(data.data,data.size,NULL,&vs);
				if(hr == S_OK)
					VData.push_back(data);
				return vs;
				break;
			case Oyster::Graphics::Core::PipelineManager::Hull:
				ID3D11HullShader* hs;
				Core::device->CreateHullShader(data.data,data.size,NULL,&hs);
				delete[] data.data;
				return hs;
				break;
			case Oyster::Graphics::Core::PipelineManager::Domain:
				ID3D11DomainShader* ds;
				Core::device->CreateDomainShader(data.data,data.size,NULL,&ds);
				delete[] data.data;
				return ds;
				break;
			case Oyster::Graphics::Core::PipelineManager::Geometry:
				ID3D11GeometryShader* gs;
				Core::device->CreateGeometryShader(data.data,data.size,NULL,&gs);
				delete[] data.data;
				return gs;
				break;
			case Oyster::Graphics::Core::PipelineManager::Pixel:
				ID3D11PixelShader* ps;
				Core::device->CreatePixelShader(data.data,data.size,NULL,&ps);
				delete[] data.data;
				return ps;
				break;
			case Oyster::Graphics::Core::PipelineManager::Compute:
				ID3D11ComputeShader* cs;
				Core::device->CreateComputeShader(data.data,data.size,NULL,&cs);
				delete[] data.data;
				return cs;
				break;
			}
			return NULL;
		}
	#pragma endregion

		void Core::PipelineManager::CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC *desc, int ElementCount,int VertexIndex,ID3D11InputLayout *&Layout)
		{
			if(VertexIndex==-1)
			{
				Layout=0;
				return;
			}
			Core::device->CreateInputLayout(desc,ElementCount,VData[VertexIndex].data,VData[VertexIndex].size,&Layout);
		}

		#pragma region Get
		int Core::PipelineManager::Get::Pixel(std::wstring Name)
		{
			if(PSMap.count(Name))
				return PSMap[Name];
			return -1;
		}

		int Core::PipelineManager::Get::Vertex(std::wstring Name)
		{
			if(VSMap.count(Name))
				return VSMap[Name];
			return -1;
		}
		int Core::PipelineManager::Get::Geometry(std::wstring Name)
		{
			if(GSMap.count(Name))
				return GSMap[Name];
			return -1;
		}

		int Core::PipelineManager::Get::Compute(std::wstring Name)
		{
			if(CSMap.count(Name))
				return CSMap[Name];
			return -1;
		}
		int Core::PipelineManager::Get::Hull(std::wstring Name)
		{
			return -1;
		}
		int Core::PipelineManager::Get::Domain(std::wstring Name)
		{
			return -1;
		}
	#pragma endregion

		#pragma region Set
		void Core::PipelineManager::Set::Pixel(int Index)
		{
			if(Current.Shaders.Pixel != Index)
			{
				if(Index==-1)
				{
					Core::deviceContext->PSSetShader( NULL,NULL,0);
				}
				else
				{
					Core::deviceContext->PSSetShader( PS[Index],NULL,0);
				}
				Current.Shaders.Pixel=Index;
			}
		}

		void Core::PipelineManager::Set::Vertex(int Index)
		{
			if(Current.Shaders.Vertex != Index)
			{
				if(Index==-1)
				{
					Core::deviceContext->VSSetShader( NULL,NULL,0);
				}
				else
				{
					Core::deviceContext->VSSetShader( VS[Index],NULL,0);
				}
				Current.Shaders.Vertex = Index;
			}
		}

		void Core::PipelineManager::Set::Geometry(int Index)
		{
			if(Current.Shaders.Geometry != Index)
			{
				if(Index==-1)
				{
					Core::deviceContext->GSSetShader( NULL,NULL,0);
				}
				else
				{
					Core::deviceContext->GSSetShader( GS[Index],NULL,0);
				}
				Current.Shaders.Geometry = Index;
			}
		}

		void Core::PipelineManager::Set::Compute(int Index)
		{
			if(Current.Shaders.Compute != Index)
			{
				if(Index==-1)
				{
					Core::deviceContext->CSSetShader( NULL,NULL,0);
				}
				else
				{
					Core::deviceContext->CSSetShader( CS[Index],NULL,0);
				}
				Current.Shaders.Compute = Index;
			}
		}
		/// \todo set Hull
		void Core::PipelineManager::Set::Hull(int Index)
		{
			return;
		}
		/// \todo set Domain
		void Core::PipelineManager::Set::Domain(int Index)
		{
			return;
		}
	#pragma endregion

		/// \todo smart Set ie. not resetting the shader
		/// \todo research states
		/// \todo smart buffer set
		void Core::PipelineManager::SetRenderPass(RenderPass se)
		{
			CleanPipeline();
			Set::Pixel(se.Shaders.Pixel);
			Set::Vertex(se.Shaders.Vertex);
			Set::Geometry(se.Shaders.Geometry);
			Set::Compute(se.Shaders.Compute);
			Core::deviceContext->IASetInputLayout(se.IAStage.Layout);
			Core::deviceContext->IASetPrimitiveTopology(se.IAStage.Topology);
			if(se.CBuffers.Vertex.size())
			{
				deviceContext->VSSetConstantBuffers(0,(int)se.CBuffers.Vertex.size(),&se.CBuffers.Vertex[0]);
			}
			if(se.CBuffers.Pixel.size())
			{
				deviceContext->PSSetConstantBuffers(0,(int)se.CBuffers.Pixel.size(),&se.CBuffers.Pixel[0]);
			}
			if(se.CBuffers.Geometry.size())
			{
				deviceContext->GSSetConstantBuffers(0,(int)se.CBuffers.Geometry.size(),&se.CBuffers.Geometry[0]);
			}
			if(se.CBuffers.Compute.size())
			{
				deviceContext->CSSetConstantBuffers(0,(int)se.CBuffers.Compute.size(),&se.CBuffers.Compute[0]);
			}
			Core::deviceContext->RSSetState(se.RenderStates.Rasterizer);
			Core::deviceContext->PSSetSamplers(0,se.RenderStates.SampleCount,se.RenderStates.SampleState);
			Core::deviceContext->CSSetSamplers(0,se.RenderStates.SampleCount,se.RenderStates.SampleState);
			Core::deviceContext->OMSetDepthStencilState(se.RenderStates.DepthStencil,0);
			float test[4] = {0};
			Core::deviceContext->OMSetBlendState(se.RenderStates.BlendState,test,-1);

			if(se.SRV.Vertex.size())
			{
				Core::deviceContext->VSSetShaderResources(0,(int)se.SRV.Vertex.size(),&se.SRV.Vertex[0]);
			}
			if(se.SRV.Geometry.size())
			{
				Core::deviceContext->GSSetShaderResources(0,(int)se.SRV.Geometry.size(),&se.SRV.Geometry[0]);
			}
			if(se.SRV.Pixel.size())
			{
				Core::deviceContext->PSSetShaderResources(0,(int)se.SRV.Pixel.size(),&se.SRV.Pixel[0]);
			}
			if(se.SRV.Compute.size())
			{
				Core::deviceContext->CSSetShaderResources(0,(int)se.SRV.Compute.size(),&se.SRV.Compute[0]);
			}

			if(se.RTV.size())
			{
				if(se.UAV.Pixel.size())
				{
					deviceContext->OMSetRenderTargetsAndUnorderedAccessViews((int)se.RTV.size(),&se.RTV[0],se.depth,(int)se.RTV.size(),(int)se.UAV.Pixel.size(),&se.UAV.Pixel[0],0);
				}
				else
				{
					deviceContext->OMSetRenderTargets((int)se.RTV.size(),&se.RTV[0],se.depth);
				}
			}
			else
			{
				if(se.UAV.Pixel.size())
				{
					deviceContext->OMSetRenderTargetsAndUnorderedAccessViews(0,NULL,se.depth,0,(int)se.UAV.Pixel.size(),&se.UAV.Pixel[0],0);
				}
			}

			if(se.UAV.Compute.size())
			{
				deviceContext->CSSetUnorderedAccessViews(0,(int)se.UAV.Compute.size(),&se.UAV.Compute[0],0);
			}
		}

		void Core::PipelineManager::Clean()
		{
			for(int i = 0; i < (int)VData.size(); ++i)
			{
				delete[] VData[i].data;
			}
		}

		void Core::PipelineManager::CleanPipeline()
		{
			//deviceContext->VSSetShaderResources(0,16,Core::srvNULL);
			//deviceContext->GSSetShaderResources(0,16,Core::srvNULL);
			//deviceContext->PSSetShaderResources(0,16,Core::srvNULL);
			deviceContext->CSSetShaderResources(0,16,Core::srvNULL);
			deviceContext->CSSetUnorderedAccessViews(0,8,Core::uavNULL,NULL);
			deviceContext->OMSetRenderTargets(8,Core::rtvNULL, NULL);
		}
	}
}