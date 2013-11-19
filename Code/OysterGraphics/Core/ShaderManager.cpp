#include "Core.h"
#include <fstream>

const char* ShaderFunction = "main";

namespace Oyster
{
	bool LoadPrecompiled(std::wstring filename, Core::ShaderManager::ShaderType type, std::wstring name);
	bool LoadCompile(std::wstring filename, Core::ShaderManager::ShaderType type, std::wstring name);
	namespace
	{
		struct ShaderData
		{
			size_t size;
			char* data;
		};
		std::vector<ID3D11PixelShader*> PS;
		std::map<std::wstring,int> PSMap;

		std::vector<ID3D11GeometryShader*> GS;
		std::map<std::wstring,int> GSMap;

		std::vector<ID3D11ComputeShader*> CS;
		std::map<std::wstring,int> CSMap;

		std::vector<ID3D11VertexShader*> VS;
		std::vector<ShaderData> VData;
		std::map<std::wstring,int> VSMap;
	}

#pragma region Init
	bool Core::ShaderManager::Init(std::wstring filename, ShaderType type, std::wstring name, bool Precompiled)
	{
		if(Precompiled)
		{
			return LoadPrecompiled(filename, type, name);
		}
		else
		{
			return LoadCompile(filename, type, name);
		}
		return true;
	}

	bool LoadPrecompiled(std::wstring filename, Core::ShaderManager::ShaderType type, std::wstring name)
	{

		std::ifstream stream;
		ShaderData sd;

	
		//Create Vertex shader and Layout	
		stream.open(filename, std::ifstream::in | std::ifstream::binary);
		if(stream.good())
		{
			stream.seekg(0, std::ios::end);
			sd.size = size_t(stream.tellg());
			sd.data = new char[sd.size];
			stream.seekg(0, std::ios::beg);
			stream.read(&sd.data[0], sd.size);
			stream.close();

			
			ID3D11VertexShader* vertex;
			ID3D11GeometryShader* geometry;
			ID3D11PixelShader* pixel;
			ID3D11ComputeShader* compute;
			
			switch(type)
			{
			case Core::ShaderManager::ShaderType::Vertex:

				if(VSMap.count(name))
					return false;	

				if(FAILED(Core::Device->CreateVertexShader(sd.data, sd.size, 0, &vertex)))
				{
					return false;
				}
				VSMap[name] = VS.size();
				VS.push_back(vertex);
				VData.push_back(sd);
				break;

			case Core::ShaderManager::ShaderType::Hull:
			case Core::ShaderManager::ShaderType::Domain:

				return false;

			case Core::ShaderManager::ShaderType::Geometry:

				if(GSMap.count(name))
					return false;
				if(FAILED(Core::Device->CreateGeometryShader(sd.data, sd.size, 0, &geometry)))
				{
					return false;
				}
				GSMap[name] = GS.size();
				GS.push_back(geometry);
				break;

			case Core::ShaderManager::ShaderType::Pixel:

				if(PSMap.count(name))
					return false;
				if(FAILED(Core::Device->CreatePixelShader(sd.data, sd.size, 0, &pixel)))
				{
					return false;
				}
				PSMap[name] = PS.size();
				PS.push_back(pixel);
				break;

			case Core::ShaderManager::ShaderType::Compute:

				if(CSMap.count(name))
					return false;
				if(FAILED(Core::Device->CreateComputeShader(sd.data, sd.size, 0, &compute)))
				{
					return false;
				}
				CSMap[name] = CS.size();
				CS.push_back(compute);
				break;
			}
		}
		else
		{
			return false;
		}
		return true;
	}

	bool LoadCompile(std::wstring filename, Core::ShaderManager::ShaderType type, std::wstring name)
	{
		/// \todo error reporting
		ID3D10Blob *Shader,*Error;
		switch(type)
		{
		case Core::ShaderManager::ShaderType::Pixel: 

			ID3D11PixelShader* pixel;

			if(FAILED(D3DCompileFromFile(filename.c_str(),NULL,NULL,ShaderFunction,"ps_5_0",0,0,&Shader,&Error)))
			{
				std::string fel = (char*)Error->GetBufferPointer();
				Error->Release();
				return false;
			}
			if(FAILED(Oyster::Core::Device->CreatePixelShader(Shader->GetBufferPointer(),Shader->GetBufferSize(),NULL,&pixel)))
			{
				Shader->Release();
				return false;
			}
			Shader->Release();
			if(!PSMap.count(name))
			{
				PSMap[name] = PS.size();
				PS.push_back(pixel);
			}
			else
			{
				PS[PSMap[name]] = pixel;
			}
			break;

		case Core::ShaderManager::ShaderType::Geometry: 

			ID3D11GeometryShader* geometry;

			if(FAILED(D3DCompileFromFile(filename.c_str(),NULL,NULL,ShaderFunction,"gs_5_0",0,0,&Shader,&Error)))
			{
				std::string fel = (char*)Error->GetBufferPointer();
				Error->Release();
				return false;
			}
			if(FAILED(Oyster::Core::Device->CreateGeometryShader(Shader->GetBufferPointer(),Shader->GetBufferSize(),NULL,&geometry)))
			{
				Error->Release();
				Shader->Release();
				return false;
			}
			Shader->Release();
			if(!GSMap.count(name))
			{
				GSMap[name] = GS.size();
				GS.push_back(geometry);
			}
			else
			{
				GS[GSMap[name]] = geometry;
			}
			break;

		case Core::ShaderManager::ShaderType::Vertex: 

			ID3D11VertexShader* vertex;

			if(FAILED(D3DCompileFromFile(filename.c_str(),NULL,NULL,ShaderFunction,"vs_5_0",0,0,&Shader,&Error)))
			{
				std::string fel = (char*)Error->GetBufferPointer();
				Error->Release();
				return false;
			}
			if(FAILED(Oyster::Core::Device->CreateVertexShader(Shader->GetBufferPointer(),Shader->GetBufferSize(),NULL,&vertex)))
			{
				Shader->Release();
				return false;
			}
			if(!VSMap.count(name))
			{
				VSMap[name] = VS.size();
				VS.push_back(vertex);
				ShaderData sd;
				sd.size = Shader->GetBufferSize();
				sd.data = new char[sd.size];
				memcpy(sd.data,Shader->GetBufferPointer(),sd.size);
				VData.push_back(sd);
			}
			else
			{
				VS[VSMap[name]] = vertex;
				delete[] VData[VSMap[name]].data;
				VData[VSMap[name]].size = Shader->GetBufferSize();
				VData[VSMap[name]].data = new char[VData[VSMap[name]].size];
				memcpy(VData[VSMap[name]].data,Shader->GetBufferPointer(),VData[VSMap[name]].size);
			}
			
			Shader->Release();
			break;
				
		}
		return true;
	}
#pragma endregion

	void Core::ShaderManager::CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC *desc, int ElementCount,int VertexIndex,ID3D11InputLayout *&Layout)
	{
		if(VertexIndex==-1)
		{
			Layout=0;
			return;
		}
		Device->CreateInputLayout(desc,ElementCount,VData[VertexIndex].data,VData[VertexIndex].size,&Layout);
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
			DeviceContext->PSSetShader( NULL,NULL,0);
		else
			DeviceContext->PSSetShader( PS[Index],NULL,0);
	}
	void Core::ShaderManager::Set::Vertex(int Index)
	{
		if(Index==-1)
			DeviceContext->VSSetShader( NULL,NULL,0);
		else
			DeviceContext->VSSetShader( VS[Index],NULL,0);
	}
	void Core::ShaderManager::Set::Geometry(int Index)
	{
		if(Index==-1)
			DeviceContext->GSSetShader( NULL,NULL,0);
		else
			DeviceContext->GSSetShader( GS[Index],NULL,0);
	}
	void Core::ShaderManager::Set::Compute(int Index)
	{
		if(Index==-1)
			DeviceContext->CSSetShader( NULL,NULL,0);
		else
			DeviceContext->CSSetShader( CS[Index],NULL,0);
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
		Oyster::Core::DeviceContext->IASetInputLayout(se.IAStage.Layout);
		Oyster::Core::DeviceContext->IASetPrimitiveTopology(se.IAStage.Topology);
		for(unsigned int i=0;i<se.CBuffers.Vertex.size();++i)
			se.CBuffers.Vertex[i]->Apply(i);
		for(unsigned int i=0;i<se.CBuffers.Geometry.size();++i)
			se.CBuffers.Geometry[i]->Apply(i);
		for(unsigned int i=0;i<se.CBuffers.Pixel.size();++i)
			se.CBuffers.Pixel[i]->Apply(i);
		Oyster::Core::DeviceContext->RSSetState(se.RenderStates.Rasterizer);
		Oyster::Core::DeviceContext->PSSetSamplers(0,se.RenderStates.SampleCount,se.RenderStates.SampleState);
		float test[4] = {0};
		Oyster::Core::DeviceContext->OMSetBlendState(se.RenderStates.BlendState,test,-1);
	}

}