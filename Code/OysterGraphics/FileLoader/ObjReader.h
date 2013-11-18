#pragma once
#include "..\Core\CoreIncludes.h"
#include "..\Math\OysterMath.h"

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
				Oyster::Math::Float3 Normal;
				Oyster::Math::Float2 UV;
			};

			static ObjReader *LoadFile(std::string fileName, Oyster::Math::Float4x4 transform = Oyster::Math::Float4x4::identity);

			ObjReader(void);
			~ObjReader(void);
			
			void GetVertexData(Vertex **vertex,int &numVertex, std::map<std::string, ID3D11ShaderResourceView *> &textures);

		private:
			Vertex *vertices;
			size_t numVertices;
			std::map<std::string, ID3D11ShaderResourceView *> materials;

			void ParseFile(std::string fileName, Oyster::Math::Float4x4 transform = Oyster::Math::Float4x4::identity);

			Oyster::Math::Float3 extract(std::string d);
			Oyster::Math::Float3 readVertex(int offset,std::string s);
			Oyster::Math::Float2 readUV(int offset,std::string s);
			Oyster::Math::Float3 readNormal(int offset,std::string s);
			void readFace(int offset,std::string s, Oyster::Math::Float3 face[3]);

			std::map<std::string, ID3D11ShaderResourceView *> GetMaterials(std::string fileName);
		};
	}
}