#include "ObjReader.h"
#include "Utilities.h"
#include "..\Core\Core.h"
#include <fstream>
#include <map>
#include <vld.h>

using namespace std;
using namespace Oyster::FileLoaders;
using namespace Oyster;
using namespace Oyster::Math;

void ObjReader::LoadFile(std::wstring fileName, Oyster::Math::Float4x4 transform)
{
		this->ParseFile(fileName + L".obj", transform);
}

ObjReader::ObjReader(void)
{
}


ObjReader::~ObjReader(void)
{
	SAFE_DELETE_ARRAY(this->vertices);
}

void ObjReader::ParseFile(std::wstring fileName, Float4x4 transform)
{
	wifstream input;
	input.open(fileName.c_str());

	if(!input.is_open())
	{
		return;
	}

	wstring path;

	std::vector<Vertex> VertexList;
	std::vector<Float3> vList;
	std::vector<Float3> nList;
	std::vector<Float2> uvList;
	Vertex vertex1, vertex2, vertex3;
	Float3 face[3];
	Float3 position, normal;
	Float2 uv;
	wstring s;

	while(!input.eof())
	{
		getline(input,s);
		int offset = (int)s.find(' ');

		if(offset!=-1)
		{
			wstring c = s.substr(0,offset); 

			if(c==L"v")
			{
				position = readVertex(offset,s);
				//position *= 0.001f;
				vList.push_back(position);
			}
			else if(c==L"vt")
			{
				uv = readUV(offset,s);
				uvList.push_back(uv);
			}
			else if(c==L"vn")
			{
				normal = readNormal(offset,s);
				nList.push_back(normal);
			}
			else if(c==L"f")
			{
				readFace(offset, s, face);

				vertex1.Position = vList[(int)face[0].x];
				vertex1.UV = uvList[(int)face[0].y];
				vertex1.Normal = nList[(int)face[0].z];

				vertex2.Position = vList[(int)face[1].x];
				vertex2.UV = uvList[(int)face[1].y];
				vertex2.Normal = nList[(int)face[1].z];

				vertex3.Position = vList[(int)face[2].x];
				vertex3.UV = uvList[(int)face[2].y];
				vertex3.Normal = nList[(int)face[2].z];

				VertexList.push_back(vertex1);
				VertexList.push_back(vertex2);
				VertexList.push_back(vertex3);
			}
			else if(c==L"mtllib")
			{
				//this->materials = GetMaterials(path + s.substr(offset+1));
			}
		}
	}

	input.close();

	this->numVertices = VertexList.size();
	this->vertices = new Vertex[this->numVertices];

	for(size_t i=0;i<this->numVertices;++i)
	{
		vertices[i].Position=Math::Float4(VertexList[i].Position,1);
		vertices[i].Normal=Math::Float4(VertexList[i].Normal,0);
		vertices[i].UV = VertexList[i].UV;
	}
}

void ObjReader::GetVertexData(Vertex **vertex,int &numVertex, std::map<std::string, ID3D11ShaderResourceView *> &Textures)
{
	numVertex=(int)this->numVertices;
	(*vertex)=this->vertices;
	//Textures = this->materials;
}

void ObjReader::GetVertexData(Vertex **vertex,int &numVertex)
{
	numVertex=(int)this->numVertices;
	(*vertex)=this->vertices;
}

Float3 ObjReader::extract(std::string d1)
{
	Float3 data;
	int offset=(int)d1.find('/');
	//string d1 = Utility::String::WStringToString(d,d1);
	data.x=(float)atoi(d1.substr(1,offset).c_str())-1;

	int newOffset = (int)d1.find('/',offset+1);
	string d2=d1.substr(offset+1,newOffset-offset-1);
	data.y=(float)atoi(d2.c_str())-1;
	offset=newOffset;

	newOffset = (int)d1.find('/',offset+1);
	string d3=d1.substr(offset+1,newOffset-offset-1);
	data.z=(float)atoi(d3.c_str())-1;

	return data;
}

Float3 ObjReader::readVertex(int offset,wstring s)
{
	int newOffset = (int)s.find(' ',offset+1);
	Float3 vertex;
	string d;
	Utility::String::WStringToString(s.substr(offset,newOffset-offset), d);
	vertex.x = (float)atof(d.c_str());
	offset=newOffset;
	
	newOffset = (int)s.find(' ',offset+1);
	Utility::String::WStringToString(s.substr(offset,newOffset-offset), d);
	vertex.y = (float)atof(d.c_str());
	offset=newOffset;
	
	newOffset = (int)s.find(' ',offset+1);
	Utility::String::WStringToString(s.substr(offset,newOffset-offset), d);
	vertex.z = (float)atof(d.c_str());
				
	return vertex;
}

Float2 ObjReader::readUV(int offset,wstring s)
{
	int newOffset = (int)s.find(' ',offset+1);
	Float2 uv;
	string d;
	Utility::String::WStringToString(s.substr(offset,newOffset-offset), d);
	uv.x =(float)atof(d.c_str());
	offset=newOffset;

	newOffset = (int)s.find(' ',offset+1);
	Utility::String::WStringToString(s.substr(offset,newOffset-offset), d);
	uv.y =1- (float)atof(d.c_str());
	offset=newOffset;

	return uv;
}

Float3 ObjReader::readNormal(int offset,wstring s)
{
	int newOffset = (int)s.find(' ',offset+1);
	Float3 vertex;
	string d;
	Utility::String::WStringToString(s.substr(offset,newOffset-offset), d);
	vertex.x = (float)atof(d.c_str());
	offset=newOffset;
	
	newOffset = (int)s.find(' ',offset+1);
	Utility::String::WStringToString(s.substr(offset,newOffset-offset), d);
	vertex.y = (float)atof(d.c_str());
	offset=newOffset;
	
	newOffset = (int)s.find(' ',offset+1);
	Utility::String::WStringToString(s.substr(offset,newOffset-offset), d);
	vertex.z = (float)atof(d.c_str());
				
	return vertex;
}

void ObjReader::readFace(int offset,wstring s1, Oyster::Math::Float3 face[3])
{
	string s;
	Utility::String::WStringToString(s1,s);

	int newOffset = (int)s.find(' ',offset+1);
	string point1 = s.substr(offset,newOffset-offset);

	offset = newOffset;
	newOffset = (int)s.find(' ',offset+1);
	string point2 = s.substr(offset,newOffset-offset);

	offset = newOffset;
	newOffset = (int)s.find(' ',offset+1);
	string point3 = s.substr(offset,newOffset-offset);
	
	face[0] = extract(point1);
	face[1] = extract(point2);
	face[2] = extract(point3);
}

std::map<std::string, ID3D11ShaderResourceView *> ObjReader::GetMaterials(std::wstring fileName)
{
	ifstream input;
	input.open(fileName.c_str());

	std::map<std::string, ID3D11ShaderResourceView *> materials;
	string texture;
	string s;
	string path;
	//Utility::String::extractDirPath(path,fileName,'\\');
	if(!input.is_open())
		return materials;

	while(!input.eof())
	{
		getline(input,s);
		int offset = (int)s.find(' ');
		if(offset!=-1)
		{
			string c = s.substr(0,offset); 
			if(c=="map_Kd")
			{
				texture = path+s.substr(offset+1);
				//D3DX11CreateShaderResourceViewFromFile(Oyster::Core::Device,texture.c_str(), NULL, NULL, &srv, NULL);
				//materials["Diffuse"] = srv;
			}
			if(c=="map_G")
			{
				texture = path+s.substr(offset+1);
				//D3DX11CreateShaderResourceViewFromFile(Oyster::Core::Device,texture.c_str(), NULL, NULL, &srv, NULL);
				//materials["Glow"] = srv;
			}
			if(c=="map_Ks")
			{
				texture = path+s.substr(offset+1);
				//D3DX11CreateShaderResourceViewFromFile(Oyster::Core::Device,texture.c_str(), NULL, NULL, &srv, NULL);
				//materials["Specular"] = srv;
			}
		}
	}
	input.close();

	return materials;
}
