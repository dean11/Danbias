#include "GeneralLoader.h"
#include "../Definitions/GraphicalDefinition.h"
#include "../Model/ModelInfo.h"
#include "../Core/Core.h"
#include <fstream>
#include <string>

#define DANFILEVERSIONMAJOR 1
#define DANFILEVERSIONMINOR 1

#define FILEHEADERSIZE		8
#define VERTEXHEADERSIZE	4
#define VERTEXSIZE			88



/// 
enum HeaderType
{
	VERTEXHEADER	= 0,	///< 
	INDEXHEADER		= 1,	///<
	MATERIALHEADER	= 2,	///<
	SKELETONHEADER	= 3,	///<
	ANIMATIONHEADER	= 4		///<
};

/// 
struct FileHeader
{
	unsigned int versionMajor;	///< 
	unsigned int versionMinor;	///< 

	///
	FileHeader(char* data)
	{
		char* memPos = data; //

		memcpy(&versionMajor, memPos, sizeof(unsigned int));
		memPos += 4;
		memcpy(&versionMinor, memPos, sizeof(unsigned int));
	}
};

///
struct VertexHeader
{
	unsigned int numVertices;
	
	///
	VertexHeader(char* data)
	{
		memcpy(&numVertices, data, sizeof(unsigned int));
	}
};

///
typedef Oyster::Graphics::Definitions::FinalVertex Vertex;

///
struct IndexHeader
{
	unsigned int numIndices;
	
	///
	IndexHeader(char* data)
	{
		memcpy(&numIndices, data, sizeof(unsigned int));
	}
};

///
struct MaterialHeader
{
	unsigned int	diffuseMapPathLength;
	char*			diffuseMapPath;

	unsigned int	normalMapPathLength;
	char*			normalMapPath;
	/*
	///
	MaterialHeader(char* data)
	{
		char* memPos = data; //

		memcpy(&diffuseMapPathLength, memPos, sizeof(unsigned int));
		memPos += 4;

		diffuseMapPath = new char[diffuseMapPathLength];
		memcpy(diffuseMapPath, memPos, diffuseMapPathLength);
		memPos += diffuseMapPathLength;

		memcpy(&normalMapPathLength, memPos, sizeof(unsigned int));
		memPos += 4;

		normalMapPath = new char[normalMapPathLength];
		memcpy(normalMapPath, memPos, normalMapPathLength);
		memPos += normalMapPathLength;
	}
	*/
};

///
struct SkeletonHeader
{
	// do this...
};

///
struct AnimationHeader
{
	// do this...
};

struct Frame
{
	// do this...
};

///
void Oyster::Graphics::Loading::UnloadDAN(void* data)
{
	Model::ModelInfo* info = (Model::ModelInfo*) data;
	SAFE_DELETE(info->Vertices);
	if(info->Indexed)
	{
		SAFE_DELETE(info->Indecies);
	}
	for(int i =0;i<info->Material.size();++i)
	{
		Oyster::Resource::OysterResource::ReleaseResource(info->Material[i]);
	}
	delete info;
}

static std::wstring charToWChar(const char* text)
{
    // Convert to a wchar_t*
    size_t origsize = strlen(text) + 1;
    size_t convertedChars = 0;
	//wchar_t* wcstring = new wchar_t[origsize];
	std::wstring wcstring; wcstring.resize(origsize);
    mbstowcs_s(&convertedChars, &wcstring[0], origsize, text, _TRUNCATE);
   return wcstring;
}

///
void Oyster::Graphics::Loading::LoadDAN(const wchar_t filename[], Oyster::Resource::CustomData& out)
{
	// 
	Oyster::Graphics::Model::ModelInfo* modelInfo = new Oyster::Graphics::Model::ModelInfo();
	modelInfo->Indexed = false;
	// Open file in binary mode
	std::ifstream danFile;
	danFile.open(filename, std::ios::binary);
	if (!danFile.is_open())
		return;

	// Read file header
	char* buffer = new char[sizeof(FileHeader)];
	danFile.read(buffer, sizeof(FileHeader));
	FileHeader fileHeader(buffer);
	delete[] buffer; // ( note: may crash here.)

	// If problem with compatability then close file and return from method
	if (fileHeader.versionMajor != DANFILEVERSIONMAJOR)
	{
		danFile.close();
		return;
	}

	// Read the .dan-file
	while (!danFile.eof())
	{
		// read header type
		unsigned int headerType;
		buffer = new char[4];
		danFile.read(buffer, 4);
		memcpy(&headerType, buffer, 4);
		delete[] buffer; // ( note: may crash here.)

		// handle header type
		switch ((HeaderType)headerType)
		{
			// vertex header
			case HeaderType::VERTEXHEADER:
			{
				// Fetch vertex header, number of vertices
				buffer = new char[4];
				danFile.read(buffer, 4);
				VertexHeader vertexHeader(buffer);
				delete[] buffer; // ( note: may crash here.)

				// Fetch all vertices
				Vertex* vertices = new Vertex[vertexHeader.numVertices];
				unsigned int bufferSize = VERTEXSIZE * vertexHeader.numVertices;
				buffer = new char[bufferSize];
				danFile.read(buffer, bufferSize);
				memcpy(vertices, buffer, bufferSize);
				delete[] buffer; // ( note: may crash here.)

				// Do the deed
				Oyster::Graphics::Core::Buffer*						vertexBuffer = new Oyster::Graphics::Core::Buffer();
				Oyster::Graphics::Core::Buffer::BUFFER_INIT_DESC	bufferInitDesc;
				bufferInitDesc.ElementSize	= sizeof(Vertex);
				bufferInitDesc.InitData		= vertices;
				bufferInitDesc.NumElements	= vertexHeader.numVertices;
				bufferInitDesc.Type			= Oyster::Graphics::Core::Buffer::BUFFER_TYPE::VERTEX_BUFFER;
				bufferInitDesc.Usage		= Oyster::Graphics::Core::Buffer::BUFFER_USAGE::BUFFER_DEFAULT;
				vertexBuffer->Init(bufferInitDesc);
				modelInfo->VertexCount = vertexHeader.numVertices;
				modelInfo->Vertices = vertexBuffer;

				delete[] vertices; // ( note: may crash here.)

				break;
			}
			case HeaderType::INDEXHEADER:
			{
				// Fetch vertex header, number of vertices
				buffer = new char[4];
				danFile.read(buffer, 4);
				IndexHeader indexHeader(buffer);
				delete[] buffer; // ( note: may crash here.)

				// Fetch all indices
				unsigned int* indices = new unsigned int[indexHeader.numIndices];
				unsigned int bufferSize = sizeof(unsigned int) * indexHeader.numIndices;
				buffer = new char[bufferSize];
				danFile.read(buffer, bufferSize);
				memcpy(indices, buffer, bufferSize);
				delete[] buffer; // ( note: may crash here.)

				// Do the deed
				Oyster::Graphics::Core::Buffer*						indexBuffer = new Oyster::Graphics::Core::Buffer();
				Oyster::Graphics::Core::Buffer::BUFFER_INIT_DESC	bufferInitDesc;
				bufferInitDesc.ElementSize	= sizeof(unsigned int);
				bufferInitDesc.InitData		= indices;
				bufferInitDesc.NumElements	= indexHeader.numIndices;
				bufferInitDesc.Type			= Oyster::Graphics::Core::Buffer::BUFFER_TYPE::INDEX_BUFFER;
				bufferInitDesc.Usage		= Oyster::Graphics::Core::Buffer::BUFFER_USAGE::BUFFER_DEFAULT;
				indexBuffer->Init(bufferInitDesc);
				modelInfo->IndexCount = indexHeader.numIndices;
				modelInfo->Indecies = indexBuffer;
				modelInfo->Indexed = true;

				delete[] indices; // ( note: may crash here.)

				break;
			}
			// material header
			case HeaderType::MATERIALHEADER:
			{
				// Fetch material header, 2 texture path strings
				MaterialHeader materialHeader;
				buffer = new char[4];
				danFile.read(buffer, 4);
				memcpy(&materialHeader.diffuseMapPathLength, buffer, 4);
				delete[] buffer; // ( note: may crash here.)

				buffer = new char[materialHeader.diffuseMapPathLength];
				danFile.read(buffer, materialHeader.diffuseMapPathLength);
				materialHeader.diffuseMapPath = new char[materialHeader.diffuseMapPathLength+1];
				memcpy(materialHeader.diffuseMapPath, buffer, materialHeader.diffuseMapPathLength);
				materialHeader.diffuseMapPath[materialHeader.diffuseMapPathLength] = 0;
				delete[] buffer; // ( note: may crash here.)
				
				buffer = new char[4];
				danFile.read(buffer, 4);
				memcpy(&materialHeader.normalMapPathLength, buffer, 4);
				delete[] buffer; // ( note: may crash here.)

				buffer = new char[materialHeader.normalMapPathLength];
				danFile.read(buffer, materialHeader.normalMapPathLength);
				materialHeader.normalMapPath = new char[materialHeader.normalMapPathLength + 1];
				memcpy(materialHeader.normalMapPath, buffer, materialHeader.normalMapPathLength);
				materialHeader.normalMapPath[materialHeader.normalMapPathLength] = 0;
				delete[] buffer; // ( note: may crash here.)

				// 
				ID3D11ShaderResourceView* diffuseMap = (ID3D11ShaderResourceView*)Oyster::Resource::OysterResource::LoadResource(charToWChar(materialHeader.diffuseMapPath).c_str(), Oyster::Graphics::Loading::LoadTexture);
				ID3D11ShaderResourceView* normalMap  = (ID3D11ShaderResourceView*)Oyster::Resource::OysterResource::LoadResource(charToWChar(materialHeader.normalMapPath).c_str(),  Oyster::Graphics::Loading::LoadTexture);
				modelInfo->Material.push_back(diffuseMap);
				modelInfo->Material.push_back(normalMap);

				delete materialHeader.normalMapPath;
				delete materialHeader.diffuseMapPath;

				break;
			}
			// skeleton header
			case HeaderType::SKELETONHEADER:
			{
				// not implemented...
				break;
			}
			// animation header
			case HeaderType::ANIMATIONHEADER:
			{
				// not implemented...
				break;
			}
		}
	}


	// close file
	danFile.close();

	// Set modelinfo as output data
	out.loadedData = modelInfo;
	out.resourceUnloadFnc = Oyster::Graphics::Loading::UnloadDAN;
}