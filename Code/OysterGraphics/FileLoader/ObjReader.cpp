#include "ObjReader.h"
#include "Utilities.h"
#include "..\Core\Core.h"
#include <fstream>
#include <map>

using namespace std;
using namespace Oyster::FileLoaders;
using namespace Oyster;
using namespace Oyster::Math;

ObjReader *ObjReader::LoadFile(std::string fileName, Oyster::Math::Float4x4 transform)
{
	static std::map<std::string, ObjReader *> cache;

	ObjReader *reader = NULL;

	if (cache.count(fileName))
	{
		reader = cache[fileName];
	}
	else
	{
		reader = new ObjReader();
		reader->ParseFile(fileName, transform);

		cache[fileName] = reader;
	}

	return reader;
}

ObjReader::ObjReader(void)
{
}


ObjReader::~ObjReader(void)
{
}

void ObjReader::ParseFile(std::string fileName, Float4x4 transform)
{
	ifstream input;
	input.open(fileName.c_str());

	if(!input.is_open())
	{
		return;
	}

	string path;
	Utility::String::extractDirPath(path,fileName,'\\');

	std::vector<Vertex> VertexList;
	std::vector<Float3> vList;
	std::vector<Float3> nList;
	std::vector<Float2> uvList;
	Vertex vertex1, vertex2, vertex3;
	Float3 face[3];
	Float3 position, normal;
	Float2 uv;
	string s;

	while(!input.eof())
	{
		getline(input,s);
		int offset = (int)s.find(' ');

		if(offset!=-1)
		{
			string c = s.substr(0,offset); 

			if(c=="v")
			{
				position = readVertex(offset,s);
				vList.push_back(position);
			}
			else if(c=="vt")
			{
				uv = readUV(offset,s);
				uvList.push_back(uv);
			}
			else if(c=="vn")
			{
				normal = readNormal(offset,s);
				nList.push_back(normal);
			}
			else if(c=="f")
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
				VertexList.push_back(vertex3);
				VertexList.push_back(vertex2);
			}
			else if(c=="mtllib")
			{
				this->materials = GetMaterials(path+s.substr(offset+1));
			}
		}
	}

	input.close();

	this->numVertices = VertexList.size();
	this->vertices = new Vertex[this->numVertices];

	for(size_t i=0;i<this->numVertices;++i)
	{
		vertices[i].Position=Math::transformVector(Math::Float4(VertexList[i].Position,1),transform);
		vertices[i].Normal=Math::transformVector(Math::Float4(VertexList[i].Normal,0),transform);
		vertices[i].UV = VertexList[i].UV;
	}
}

void ObjReader::GetVertexData(Vertex **vertex,int &numVertex, std::map<std::string, ID3D11ShaderResourceView *> &Textures)
{
	numVertex=(int)this->numVertices;
	(*vertex)=this->vertices;
	Textures = this->materials;
}

Float3 ObjReader::extract(std::string d)
{
	Float3 data;
	int offset=(int)d.find('/');
	data.x=(float)atoi(d.substr(1,offset).c_str())-1;

	int newOffset = (int)d.find('/',offset+1);
	string d2=d.substr(offset+1,newOffset-offset-1);
	data.y=(float)atoi(d2.c_str())-1;
	offset=newOffset;

	newOffset = (int)d.find('/',offset+1);
	string d3=d.substr(offset+1,newOffset-offset-1);
	data.z=(float)atoi(d3.c_str())-1;

	return data;
}

Float3 ObjReader::readVertex(int offset,string s)
{
	int newOffset = (int)s.find(' ',offset+1);
	Float3 vertex;
	string d = s.substr(offset,newOffset-offset);
	vertex.x = (float)atof(d.c_str());
	offset=newOffset;
	
	newOffset = (int)s.find(' ',offset+1);
	vertex.y = (float)atof(s.substr(offset,newOffset-offset).c_str());
	offset=newOffset;
	
	newOffset = (int)s.find(' ',offset+1);
	vertex.z = (float)-atof(s.substr(offset,newOffset-offset).c_str());
				
	return vertex;
}

Float2 ObjReader::readUV(int offset,string s)
{
	int newOffset = (int)s.find(' ',offset+1);
	Float2 uv;
	string d = s.substr(offset,newOffset-offset);
	uv.x =(float)atof(d.c_str());
	offset=newOffset;

	newOffset = (int)s.find(' ',offset+1);
	d = s.substr(offset,newOffset-offset);
	uv.y =1- (float)atof(d.c_str());
	offset=newOffset;

	return uv;
}

Float3 ObjReader::readNormal(int offset,string s)
{
	int newOffset = (int)s.find(' ',offset+1);
	Float3 vertex;
	string d = s.substr(offset,newOffset-offset);
	vertex.x = (float)atof(d.c_str());
	offset=newOffset;
	
	newOffset = (int)s.find(' ',offset+1);
	vertex.y = (float)atof(s.substr(offset,newOffset-offset).c_str());
	offset=newOffset;
	
	newOffset = (int)s.find(' ',offset+1);
	vertex.z = (float)-atof(s.substr(offset,newOffset-offset).c_str());
				
	return vertex;
}

void ObjReader::readFace(int offset,string s, Oyster::Math::Float3 face[3])
{
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

std::map<std::string, ID3D11ShaderResourceView *> ObjReader::GetMaterials(std::string fileName)
{
	ifstream input;
	input.open(fileName.c_str());

	std::map<std::string, ID3D11ShaderResourceView *> materials;
	ID3D11ShaderResourceView *srv;
	string texture;
	string s;
	string path;
	Utility::String::extractDirPath(path,fileName,'\\');
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
				D3DX11CreateShaderResourceViewFromFile(Oyster::Core::Device,texture.c_str(), NULL, NULL, &srv, NULL);
				materials["Diffuse"] = srv;
			}
			if(c=="map_G")
			{
				texture = path+s.substr(offset+1);
				D3DX11CreateShaderResourceViewFromFile(Oyster::Core::Device,texture.c_str(), NULL, NULL, &srv, NULL);
				materials["Glow"] = srv;
			}
			if(c=="map_Ks")
			{
				texture = path+s.substr(offset+1);
				D3DX11CreateShaderResourceViewFromFile(Oyster::Core::Device,texture.c_str(), NULL, NULL, &srv, NULL);
				materials["Specular"] = srv;
			}
		}
	}
	input.close();

	return materials;
}
