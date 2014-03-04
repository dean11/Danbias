#include "GraphicalDefinition.h"
#include <fstream>
#include <string>

#define DANFILEVERSIONMAJOR 2
#define DANFILEVERSIONMINOR 1

#define FILEHEADERSIZE		8
#define VERTEXHEADERSIZE	4



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

static wchar_t* charToWChar(const char* text)
{
	// Convert to a wchar_t*
	size_t origsize = strlen(text) + 1;
	size_t convertedChars = 0;
	wchar_t* wcstring = new wchar_t[origsize];
	mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);
   return wcstring;
}

static void ReadData(void* Destination, std::ifstream& file,  int size)
{
	char* buffer = new char[size];
	file.read(buffer,size);
	memcpy(Destination,buffer,size);
	delete[] buffer;
}

void UnloadDAN(ModelInfo& info)
{
	delete[] info.vertices;
	delete[] info.indices;
	info.indices = 0;
	info.vertices = 0;
}

///
ModelInfo LoadDAN(const char filename[])
{
	ModelInfo model;
	model.numIndex = model.numVertex = 0;

	// Open file in binary mode
	std::ifstream danFile;
	danFile.open(filename, std::ios::binary);
	if (!danFile.is_open())
	{
		return model;
	}

	// Read file header
	char* buffer = new char[sizeof(FileHeader)];
	danFile.read(buffer, sizeof(FileHeader));
	FileHeader fileHeader(buffer);
	delete[] buffer; // ( note: may crash here.)

	// If problem with compatability then close file and return from method
	if (fileHeader.versionMajor != DANFILEVERSIONMAJOR)
	{
		danFile.close();
		return model;
	}

	// Read the .dan-file
	
	// read header for vertex
	unsigned int headerType;
	ReadData(&headerType,danFile,4);

		// Fetch vertex header, number of vertices
		buffer = new char[4];
		danFile.read(buffer, 4);
		VertexHeader vertexHeader(buffer);
		delete[] buffer; // ( note: may crash here.)

		Vertex* buf = new Vertex[vertexHeader.numVertices];
		ReadData(buf, danFile, vertexHeader.numVertices * sizeof(Vertex));
		model.numVertex = vertexHeader.numVertices;

		//Copy over only the vertex positions
		model.vertices = new FinalVertex[vertexHeader.numVertices];
		for(int i = 0; i < vertexHeader.numVertices; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				model.vertices[i].pos[j] = buf[i].data[j];
			}
		}

	//Read header for index
	ReadData(&headerType,danFile,4);


		// Fetch vertex header, number of vertices
		buffer = new char[4];
		danFile.read(buffer, 4);
		IndexHeader indexHeader(buffer);
		delete[] buffer; // ( note: may crash here.)

		// Fetch all indices
		unsigned int* indices = new unsigned int[indexHeader.numIndices];
		unsigned int bufferSize = sizeof(unsigned int) * indexHeader.numIndices;

		ReadData(indices,danFile,bufferSize);

		model.numIndex = indexHeader.numIndices;
		model.indices = indices;


	// close file
	danFile.close();

	// Set modelinfo as output data
	return model;
}