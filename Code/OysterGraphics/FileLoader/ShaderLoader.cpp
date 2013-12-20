#include "GeneralLoader.h"
#include "..\Core\Dx11Includes.h"
#include "..\Core\Core.h"
#include <fstream>


namespace Oyster
{
	namespace Graphics
	{
		namespace Loading
		{
			void LoadShader(const wchar_t filename[], Oyster::Resource::CustomData& out, int type);

			void UnloadShaderP(void* loadedData)
			{
				ID3D11PixelShader* ps = ((ID3D11PixelShader*)loadedData);
				SAFE_RELEASE(ps);
			}

			void UnloadShaderG(void* loadedData)
			{
				ID3D11GeometryShader* gs = ((ID3D11GeometryShader*)loadedData);
				SAFE_RELEASE(gs);
			}

			void UnloadShaderC(void* loadedData)
			{
				ID3D11ComputeShader* ps = ((ID3D11ComputeShader*)loadedData);
				SAFE_RELEASE(ps);
			}

			void UnloadShaderH(void* loadedData)
			{
				ID3D11HullShader* ps = ((ID3D11HullShader*)loadedData);
				SAFE_RELEASE(ps);
			}

			void UnloadShaderD(void* loadedData)
			{
				ID3D11DomainShader* ps = ((ID3D11DomainShader*)loadedData);
				SAFE_RELEASE(ps);
			}

			void UnloadShaderV(void* loadedData)
			{
				ID3D11VertexShader* ps = ((ID3D11VertexShader*)loadedData);
				SAFE_RELEASE(ps);
			}

			void LoadShaderP(const wchar_t filename[], Oyster::Resource::CustomData& out)
			{
				LoadShader(filename,out,Core::PipelineManager::Pixel);
				if(out.loadedData==NULL)
				{
					memset(&out,0,sizeof(out));
					return;
				}
				out.resourceUnloadFnc = UnloadShaderP;
			}

			void LoadShaderG(const wchar_t filename[], Oyster::Resource::CustomData& out)
			{
				
				LoadShader(filename,out,Core::PipelineManager::Geometry);
				if(out.loadedData==NULL)
				{
					memset(&out,0,sizeof(out));
					return;
				}
				out.resourceUnloadFnc = UnloadShaderG;
			}

			void LoadShaderC(const wchar_t filename[], Oyster::Resource::CustomData& out)
			{
				
				LoadShader(filename,out,Core::PipelineManager::Compute);
				if(out.loadedData==NULL)
				{
					memset(&out,0,sizeof(out));
					return;
				}
				out.resourceUnloadFnc = UnloadShaderC;
			}

			void LoadShaderH(const wchar_t filename[], Oyster::Resource::CustomData& out)
			{
				
				LoadShader(filename,out,Core::PipelineManager::Hull);
				if(out.loadedData==NULL)
				{
					memset(&out,0,sizeof(out));
					return;
				}
				out.resourceUnloadFnc = UnloadShaderH;
			}

			void LoadShaderD(const wchar_t filename[], Oyster::Resource::CustomData& out)
			{
				
				LoadShader(filename,out,Core::PipelineManager::Domain);
				if(out.loadedData==NULL)
				{
					memset(&out,0,sizeof(out));
					return;
				}
				out.resourceUnloadFnc = UnloadShaderD;
			}

			void LoadShaderV(const wchar_t filename[], Oyster::Resource::CustomData& out)
			{
				
				LoadShader(filename,out,Core::PipelineManager::Vertex);
				if(out.loadedData==NULL)
				{
					memset(&out,0,sizeof(out));
					return;
				}
				out.resourceUnloadFnc = UnloadShaderV;
			}

			void LoadShader(const wchar_t filename[], Oyster::Resource::CustomData& out, int type)
			{
				Core::PipelineManager::ShaderData data;
#ifdef _DEBUG
				ID3DBlob *Shader=NULL, *Error=NULL;
				HRESULT hr;
				switch (Core::PipelineManager::ShaderType(type))
				{
				case Oyster::Graphics::Core::PipelineManager::Vertex:
					hr = D3DCompileFromFile(filename,NULL,D3D_COMPILE_STANDARD_FILE_INCLUDE,"main","vs_5_0",D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,0,&Shader,&Error);
					break;
				case Oyster::Graphics::Core::PipelineManager::Hull:
					hr = D3DCompileFromFile(filename,NULL,D3D_COMPILE_STANDARD_FILE_INCLUDE,"main","hs_5_0",D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,0,&Shader,&Error);
					break;
				case Oyster::Graphics::Core::PipelineManager::Domain:
					hr = D3DCompileFromFile(filename,NULL,D3D_COMPILE_STANDARD_FILE_INCLUDE,"main","ds_5_0",D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,0,&Shader,&Error);
					break;
				case Oyster::Graphics::Core::PipelineManager::Geometry:
					hr = D3DCompileFromFile(filename,NULL,D3D_COMPILE_STANDARD_FILE_INCLUDE,"main","gs_5_0",D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,0,&Shader,&Error);
					break;
				case Oyster::Graphics::Core::PipelineManager::Pixel:
					hr = D3DCompileFromFile(filename,NULL,D3D_COMPILE_STANDARD_FILE_INCLUDE,"main","ps_5_0",D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,0,&Shader,&Error);
					break;
				case Oyster::Graphics::Core::PipelineManager::Compute:
					hr = D3DCompileFromFile(filename,NULL,D3D_COMPILE_STANDARD_FILE_INCLUDE,"main","cs_5_0",D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,0,&Shader,&Error);
					break;
				default:
					break;
				}

				if(FAILED(hr))
				{
					if(Error)
					{
						Error->Release();
					}
					if(Shader)
					{
						Shader->Release();
					}
					memset(&out,0,sizeof(out));
					return;
				}

				data.size = Shader->GetBufferSize();
				data.data = new char[data.size];
				memcpy(data.data,Shader->GetBufferPointer(),data.size);
#else
				std::ifstream stream;

				stream.open(filename, std::ifstream::in | std::ifstream::binary);
				if(stream.good())
				{
					stream.seekg(0, std::ios::end);
					data.size = size_t(stream.tellg());
					data.data = new char[data.size];
					stream.seekg(0, std::ios::beg);
					stream.read(&data.data[0], data.size);
					stream.close();
				}
				else
				{
					memset(&out,0,sizeof(out));
					return;
				}
#endif
				out.loadedData = Core::PipelineManager::CreateShader(data, Core::PipelineManager::ShaderType(type));
			}
		}
	}
}