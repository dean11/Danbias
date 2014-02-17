#pragma once
#include "../Model/ModelInfo.h"
#include "OysterMath.h"

namespace Oyster
{
	namespace FileLoaders
	{
		class ObjReader
		{
		public:
			struct Vertex
			{
				Oyster::Math::Float3 Position;
				Oyster::Math::Float2 UV;
				Oyster::Math::Float3 Normal;
			};

			void LoadFile(std::wstring fileName, Oyster::Math::Float4x4 transform = Oyster::Math::Float4x4::identity);

			ObjReader(void);
			~ObjReader(void);
			
			void GetVertexData(Vertex **vertex,int &numVertex, std::map<std::string, ID3D11ShaderResourceView *> &textures);
			void GetVertexData(Vertex **vertex,int &numVertex);

		private:
			Vertex *vertices;
			size_t numVertices;
			std::map<std::wstring, ID3D11ShaderResourceView *> materials;

			void ParseFile(std::wstring fileName, Oyster::Math::Float4x4 transform = Oyster::Math::Float4x4::identity);

			Oyster::Math::Float3 extract(std::string d);
			Oyster::Math::Float3 readVertex(int offset,std::wstring s);
			Oyster::Math::Float2 readUV(int offset,std::wstring s);
			Oyster::Math::Float3 readNormal(int offset,std::wstring s);
			void readFace(int offset,std::wstring s, Oyster::Math::Float3 face[3]);

			std::map<std::string, ID3D11ShaderResourceView *> GetMaterials(std::wstring fileName);
		};
	}
}