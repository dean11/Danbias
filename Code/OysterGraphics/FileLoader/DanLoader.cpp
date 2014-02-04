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
	unsigned int numBones;
	
	///
	SkeletonHeader(char* data)
	{
		memcpy(&numBones, data, sizeof(unsigned int));
	}
};

///
struct AnimationHeader
{
	unsigned int numAnims;


	AnimationHeader(char* data)
	{
		memcpy(&numAnims, data, sizeof(unsigned int));
	}
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
	if(info->Animated)
	{
		//clean animation
		delete[] info->bones;
		for(int a = 0; a < info->AnimationCount; ++a)
		{
			for(int x = 0; x < info->Animations[a].Bones; ++x)
			{
				delete[] info->Animations[a].Keyframes[x];
			}
			delete[] info->Animations[a].Frames;
			delete[] info->Animations[a].Keyframes;
		}
		delete[] info->Animations;
	}
	for(int i =0;i<info->Material.size();++i)
	{
		Core::loader.ReleaseResource(info->Material[i]);
	}
	delete info;
}

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

///
void* Oyster::Graphics::Loading::LoadDAN(const wchar_t filename[])
{
	// 
	Oyster::Graphics::Model::ModelInfo* modelInfo = new Oyster::Graphics::Model::ModelInfo();
	modelInfo->Indexed = false;
	modelInfo->Animated = false;
	// Open file in binary mode
	std::ifstream danFile;
	danFile.open(filename, std::ios::binary);
	if (!danFile.is_open())
		return NULL;

	// Read file header
	char* buffer = new char[sizeof(FileHeader)];
	danFile.read(buffer, sizeof(FileHeader));
	FileHeader fileHeader(buffer);
	delete[] buffer; // ( note: may crash here.)

	// If problem with compatability then close file and return from method
	if (fileHeader.versionMajor != DANFILEVERSIONMAJOR)
	{
		danFile.close();
		return NULL;
	}

	// Read the .dan-file
	while (!danFile.eof())
	{
		// read header type
		unsigned int headerType;
		ReadData(&headerType,danFile,4);

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
				unsigned int bufferSize = VERTEXSIZE * vertexHeader.numVertices;
				buffer = new char[bufferSize];
				danFile.read(buffer, bufferSize);

				// Do the deed
				Oyster::Graphics::Core::Buffer*						vertexBuffer = new Oyster::Graphics::Core::Buffer();
				Oyster::Graphics::Core::Buffer::BUFFER_INIT_DESC	bufferInitDesc;
				bufferInitDesc.ElementSize	= sizeof(Vertex);
				bufferInitDesc.InitData		= buffer;
				bufferInitDesc.NumElements	= vertexHeader.numVertices;
				bufferInitDesc.Type			= Oyster::Graphics::Core::Buffer::BUFFER_TYPE::VERTEX_BUFFER;
				bufferInitDesc.Usage		= Oyster::Graphics::Core::Buffer::BUFFER_USAGE::BUFFER_DEFAULT;
				vertexBuffer->Init(bufferInitDesc);
				modelInfo->VertexCount = vertexHeader.numVertices;
				modelInfo->Vertices = vertexBuffer;

				
				delete[] buffer; // ( note: may crash here.)

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

				
				ReadData(indices,danFile,bufferSize);

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

				//read difuse map name length
				ReadData(&materialHeader.diffuseMapPathLength,danFile,4);
				
				//read diffuse map name
				materialHeader.diffuseMapPath = new char[materialHeader.diffuseMapPathLength+1];
				ReadData(materialHeader.diffuseMapPath,danFile,materialHeader.diffuseMapPathLength);
				//null terminate
				materialHeader.diffuseMapPath[materialHeader.diffuseMapPathLength] = 0;
				
				//read normal map name length
				ReadData(&materialHeader.normalMapPathLength,danFile,4);

				//read difuse map name
				materialHeader.normalMapPath = new char[materialHeader.normalMapPathLength + 1];
				ReadData(materialHeader.normalMapPath,danFile,materialHeader.normalMapPathLength);
				materialHeader.normalMapPath[materialHeader.normalMapPathLength] = 0;

				//load diffuse map 
				wchar_t* path = charToWChar(materialHeader.diffuseMapPath);
				ID3D11ShaderResourceView* diffuseMap = (ID3D11ShaderResourceView*)Core::loader.LoadResource((Core::texturePath + path).c_str(), Oyster::Graphics::Loading::LoadTexture, Oyster::Graphics::Loading::UnloadTexture);
				delete[] path;

				//load normal map
				path = charToWChar(materialHeader.normalMapPath);
				ID3D11ShaderResourceView* normalMap  = (ID3D11ShaderResourceView*)Core::loader.LoadResource((Core::texturePath + path).c_str(),  Oyster::Graphics::Loading::LoadTexture, Oyster::Graphics::Loading::UnloadTexture);
				delete[] path;

				//add to model
				modelInfo->Material.push_back(diffuseMap);
				modelInfo->Material.push_back(normalMap);

				//clean up
				delete[] materialHeader.diffuseMapPath;
				delete[] materialHeader.normalMapPath;

				break;
			}

			// skeleton header
			case HeaderType::SKELETONHEADER:
			{
				// Fetch Skeleton header, number of Bones
				buffer = new char[4];
				danFile.read(buffer, 4);
				SkeletonHeader skeletonHeader(buffer);
				delete[] buffer; // ( note: may crash here.)

				//array for bone data
				Oyster::Graphics::Model::Bone* bones = new Oyster::Graphics::Model::Bone[skeletonHeader.numBones];
				
				//read bones
				ReadData(bones,danFile,skeletonHeader.numBones * sizeof(Oyster::Graphics::Model::Bone));


				//read skeleton Hiarchy

				modelInfo->BoneCount = skeletonHeader.numBones;
				modelInfo->bones = bones;

				break;
			}
			// animation header
			case HeaderType::ANIMATIONHEADER:
			{
				//get num anims
				buffer = new char[4];
				danFile.read(buffer, 4);
				AnimationHeader animationHeader(buffer);
				delete[] buffer;

				Oyster::Graphics::Model::Animation* anims = new Oyster::Graphics::Model::Animation[animationHeader.numAnims];

				for(int a = 0; a < animationHeader.numAnims; ++a)
				{
					//read name of animation
					int nameLength;

					
					ReadData(&nameLength,danFile,4);

					char* name = new char[nameLength + 1];
					ReadData(name,danFile,nameLength);
					name[nameLength] = 0;

					wchar_t* wName = charToWChar(name);
					anims[a].name = std::wstring(wName);
					delete[] wName;
					delete name;

					//read nr of bones in animation
					ReadData(&anims[a].Bones,danFile,4);

					//read duration
					ReadData(&anims[a].duration,danFile,8);

					//create Frame array and Bone part of KeyFrameArray;
					anims[a].Frames = new int[anims[a].Bones];
					anims[a].Keyframes = new Oyster::Graphics::Model::Frame*[anims[a].Bones];
					
					//loop per bone and gather data
					for(int b = 0; b < anims[a].Bones; ++b)
					{
						
						//read bone index
						int boneIndex;
						ReadData(&boneIndex,danFile,4);

						//read nr of frames per bone
						ReadData(&anims[a].Frames[b],danFile,4);

						//create frame matrix
						anims[a].Keyframes[b] = new Oyster::Graphics::Model::Frame[anims[a].Frames[b]];


						for(int f = 0; f < anims[a].Frames[b]; ++f)
						{
							//write index of bone
							anims[a].Keyframes[b][f].bone.Parent = boneIndex;

							//read bone transform
							ReadData(&anims[a].Keyframes[b][f].bone.Relative, danFile, sizeof(Math::Matrix));
							
							ReadData(&anims[a].Keyframes[b][f].time,danFile,sizeof(double));
						}
					}
				}
				modelInfo->AnimationCount = animationHeader.numAnims;
				modelInfo->Animations = anims;
				modelInfo->Animated = true;

				break;
			}
		}
	}

	// close file
	danFile.close();

	// Set modelinfo as output data
	return modelInfo;
}