#include "Shader.h"
#include "../Core/Core.h"
#include "Utilities.h"
#include <fstream>
#include <sstream>
namespace Oyster
{
	namespace
	{
		std::vector<ID3D11PixelShader*> PS;
		std::map<std::string,int> PSMap;

		std::vector<ID3D11GeometryShader*> GS;
		std::map<std::string,int> GSMap;

		std::vector<ID3D11ComputeShader*> CS;
		std::map<std::string,int> CSMap;

		std::vector<ID3D11VertexShader*> VS;
		std::vector<ID3D10Blob*> VBlob;
		std::map<std::string,int> VSMap;

		std::stringstream log;
	}

	bool Oyster::Shader::InitShaders(const std::string &name)
	{
		std::ifstream input;
		input.open(name.c_str());
		std::string s, file,method;
		std::vector<std::string> line;
		if(!input.is_open())
			return false;
		while(!input.eof())
		{
			getline(input,s);
			line.clear();
			Utility::String::split(line,s,' ');
			if(line.size())
			{
				if(line[0]=="F")
				{
					file = line[1];
				}
				if(line[0]=="P")
				{
					ID3D10Blob *Shader,*Error; 
					if(!PSMap.count(line[2]))
					{
						PSMap[line[2]]=(int)PS.size();
						ID3D11PixelShader* pixel;
						if(FAILED(D3DX11CompileFromFileA(file.c_str(),NULL,NULL,line[1].c_str(),"ps_5_0",0,0,NULL,&Shader,&Error,NULL)))
						{
							std::string fel = (char*)Error->GetBufferPointer();
							PSMap.erase(line[2]);
							Error->Release();
							return false;
						}
						if(FAILED(Oyster::Core::Device->CreatePixelShader(Shader->GetBufferPointer(),Shader->GetBufferSize(),NULL,&pixel)))
						{
							PSMap.erase(line[2]);
							Error->Release();
							Shader->Release();
							return false;
						}
						Shader->Release();
						PS.push_back(pixel);
					}
				}
				if(line[0]=="V")
				{
					ID3D10Blob *Shader,*Error; 
					if(!VSMap.count(line[2]))
					{
						int i = (int)VS.size();
						VSMap[line[2]]= i;
						ID3D11VertexShader* vertex;
						if(FAILED(D3DX11CompileFromFileA(file.c_str(),NULL,NULL,line[1].c_str(),"vs_5_0",0,0,NULL,&Shader,&Error,NULL)))
						{
							log //<< "Shader Compilation Warning(s)/Error(s)\n-----------------------------\n"
						 << (char*) Error->GetBufferPointer();
						// << "-----------------------------\n";
							s = log.str();
							VSMap.erase(line[2]);
							Error->Release();
							return false;
						}
						if(FAILED(Oyster::Core::Device->CreateVertexShader
							(Shader->GetBufferPointer(),
							Shader->GetBufferSize(),
							NULL,
							&vertex)))
						{
							VSMap.erase(line[2]);
							Error->Release();
							Shader->Release();
							return false;
						}
						VS.push_back(vertex);
						VBlob.push_back(Shader);
					}
				}
				if(line[0]=="G")
				{
					ID3D10Blob *Shader,*Error; 
					if(!GSMap.count(line[2]))
					{
						GSMap[line[2]]=(int)GS.size();
						ID3D11GeometryShader* pixel;
						if(FAILED(D3DX11CompileFromFileA(file.c_str(),NULL,NULL,line[1].c_str(),"gs_5_0",0,0,NULL,&Shader,&Error,NULL)))
						{
							std::string fel = (char*)Error->GetBufferPointer();
							GSMap.erase(line[2]);
							Error->Release();
							return false;
						}
						if(FAILED(Oyster::Core::Device->CreateGeometryShader(Shader->GetBufferPointer(),Shader->GetBufferSize(),NULL,&pixel)))
						{
							GSMap.erase(line[2]);
							Error->Release();
							Shader->Release();
							return false;
						}
						Shader->Release();
						GS.push_back(pixel);
					}
				}
				if(line[0]=="C")
				{
					ID3D10Blob *Shader,*Error; 
					if(!CSMap.count(line[2]))
					{
						CSMap[line[2]]=(int)CS.size();
						ID3D11ComputeShader* comp;
						if(FAILED(D3DX11CompileFromFileA(file.c_str(),NULL,NULL,line[1].c_str(),"cs_5_0",0,0,NULL,&Shader,&Error,NULL)))
						{
							std::string fel = (char*)Error->GetBufferPointer();
							CSMap.erase(line[2]);
							Error->Release();
							return false;
						}
						if(FAILED(Oyster::Core::Device->CreateComputeShader(Shader->GetBufferPointer(),Shader->GetBufferSize(),NULL,&comp)))
						{
							CSMap.erase(line[2]);
							Error->Release();
							Shader->Release();
							return false;
						}
						Shader->Release();
						CS.push_back(comp);
					}
				}
			}
		}
		return true;
	}

	void Oyster::Shader::SetShaderEffect(ShaderEffect se)
	{
		Shader::Set::SetPixel(se.Shaders.Pixel);
		Shader::Set::SetVertex(se.Shaders.Vertex);
		Shader::Set::SetGeometry(se.Shaders.Geometry);
		Shader::Set::SetCompute(se.Shaders.Compute);
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
	void Oyster::Shader::Set::SetPixel(int Index)
	{
		if(Index==-1)
			Oyster::Core::DeviceContext->PSSetShader( NULL,NULL,0);
		else
			Oyster::Core::DeviceContext->PSSetShader( PS[Index],NULL,0);
	}
	void Oyster::Shader::Set::SetVertex(int Index)
	{
		if(Index==-1)
			Oyster::Core::DeviceContext->VSSetShader(NULL,NULL,0);
		else
			Oyster::Core::DeviceContext->VSSetShader(VS[Index],NULL,0);
	}
	void Oyster::Shader::Set::SetGeometry(int Index)
	{
		if(Index==-1)
			Oyster::Core::DeviceContext->GSSetShader(NULL,NULL,0);
		else
			Oyster::Core::DeviceContext->GSSetShader(GS[Index],NULL,0);
	}
	void Oyster::Shader::Set::SetCompute(int Index)
	{
		if(Index==-1)
			Oyster::Core::DeviceContext->CSSetShader(NULL,NULL,0);
		else
			Oyster::Core::DeviceContext->CSSetShader(CS[Index],NULL,0);
	}
	void Oyster::Shader::Set::SetHull(int Index)
	{
	}
	void Oyster::Shader::Set::SetDomain(int Index)
	{
	}

	int Oyster::Shader::Get::GetPixel(std::string Name)
	{
		if(PSMap.count(Name))
			return PSMap[Name];
		return -1;
	}
	int Oyster::Shader::Get::GetVertex(std::string Name)
	{
		if(VSMap.count(Name))
			return VSMap[Name];
		return -1;
	}
	int Oyster::Shader::Get::GetGeometry(std::string Name)
	{
		if(GSMap.count(Name))
			return GSMap[Name];
		return -1;
	}
	int Oyster::Shader::Get::GetCompute(std::string Name)
	{
		if(CSMap.count(Name))
			return CSMap[Name];
		return -1;
	}
	int Oyster::Shader::Get::GetHull(std::string Name)
	{
		return -1;
	}
	int Oyster::Shader::Get::GetDomain(std::string Name)
	{
		return -1;
	}
	void Oyster::Shader::CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC *desc, int ElementCount,int VertexIndex,ID3D11InputLayout *&Layout)
	{
		if(VertexIndex==-1)
		{
			Layout=0;
			return;
		}
		Oyster::Core::Device->CreateInputLayout(desc,ElementCount,VBlob[VertexIndex]->GetBufferPointer(),VBlob[VertexIndex]->GetBufferSize(),&Layout);
	}
}