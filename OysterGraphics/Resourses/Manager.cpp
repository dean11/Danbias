#include "Manager.h"

std::unordered_map< std::string, Oyster::Render::ModelInfo*> Oyster::Resources::Manager::loadedModels = std::unordered_map< std::string, Oyster::Render::ModelInfo*>();

Oyster::Render::Model* Oyster::Resources::Manager::LoadModel(std::string Filename, Matrix Scale)
{
	////TODO: Add redundncy sheck, to ensure not recreating model

	////Loop to find filename

	////If found Return Model

	////else Create Model

	//Oyster::FileLoaders::ObjReader *reader = Oyster::FileLoaders::ObjReader::LoadFile(Filename, Scale);
	//Oyster::FileLoaders::ObjReader::Vertex** vertex = new Oyster::FileLoaders::ObjReader::Vertex*[1];
	//int vcount;
	//std::map<std::string,ID3D11ShaderResourceView *> textures;
	//reader->GetVertexData( vertex, vcount, textures );

	//Oyster::Buffer::BUFFER_INIT_DESC desc;
	//desc.ElementSize=sizeof(Oyster::FileLoaders::ObjReader::Vertex);
	//desc.NumElements = vcount;
	//desc.InitData = *vertex;
	//desc.Type = Oyster::Buffer::VERTEX_BUFFER;
	//desc.Usage = Oyster::Buffer::BUFFER_DEFAULT;
	//
	//ID3D11ShaderResourceView *srv = textures["Diffuse"];

	//Oyster::Render::ModelInfo* m = new Oyster::Render::ModelInfo();
	//
	//m->Vertices = *(Oyster::Engine::Init::Buffers::CreateBuffer(desc));
	//m->VertexCount = vcount;
	//m->Material.push_back(srv);
	//srv = textures["Specular"];
	//m->Material.push_back(srv);
	//srv = textures["Glow"];
	//m->Material.push_back(srv);
	//m->Indexed=false;
	//
	//Oyster::Render::Model* model = new Oyster::Render::Model();
	//model->info=m;
	//model->Visible = true;
	//model->World = &Oyster::Math::Float4x4(Oyster::Math::Float4x4::identity);
	return NULL;
}

