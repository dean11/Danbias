#include "Object.h"
#include "OysterMath.h"
#include "DllInterfaces\GFXAPI.h"


using namespace GameLogic;

using namespace Oyster::Math;
using namespace Oyster::Graphics::Model;

using namespace Utility::DynamicMemory;

Object::Object(void)
{
<<<<<<< HEAD
	model = new Oyster::Graphics::Model::Model();
	model = Oyster::Graphics::API::CreateModel(L"bth.obj");

	model->WorldMatrix *= 0.1f;
	model->WorldMatrix.m44 = 1.0f;
=======
	model = new Model();
	model = Oyster::Graphics::API::CreateModel(L"bth.obj");

	/*struct float4
	{
		float x,y,z,w;
	};

	float4 mesh[] =
	{
		{-1.0f,1.0f,0.0f,1.0f},
		{1.0f,1.0f,0.0f,1.0f},
		{1.0f,-1.0f,0.0f,1.0f},
	};

	Oyster::Graphics::Buffer::BUFFER_INIT_DESC desc;
	desc.ElementSize= sizeof(float4);
	desc.NumElements = 3;
	desc.InitData=mesh;
	desc.Type = Oyster::Graphics::Buffer::BUFFER_TYPE::VERTEX_BUFFER;
	desc.Usage = Oyster::Graphics::Buffer::BUFFER_USAGE::BUFFER_USAGE_IMMUTABLE;

	Oyster::Graphics::Buffer *b = new Oyster::Graphics::Buffer();
	b->Init(desc);

	ModelInfo* modelInfo = new ModelInfo();
	modelInfo->Vertices = *b;

	modelInfo->Indexed = false;
	modelInfo->VertexCount = 3;
	
	
	Float4x4 matrix = Float4x4::identity;

	model->World = &matrix;
	model->info = modelInfo;
	model->Visible = true;*/
>>>>>>> 01515a4d2d309cfebf4e3dab3ad68787f93050e1
}


Object::~Object(void)
{
<<<<<<< HEAD

=======
	//SAFE_DELETE(model->info);

	 Oyster::Graphics::API::DeleteModel(model);
>>>>>>> 01515a4d2d309cfebf4e3dab3ad68787f93050e1
}
Model* Object::Render()
{
<<<<<<< HEAD
	Oyster::Graphics::API::RenderScene(model,1);
}

void Object::Update()
{
	//dummy implementation that will be overloaded if the other class implements it in a different way
=======
	//model->info->Vertices.Apply(0);
	this->rigidBody->GetOrientation(model->WorldMatrix);
	return model;
>>>>>>> 01515a4d2d309cfebf4e3dab3ad68787f93050e1
}

Object::OBJECT_TYPE Object::GetType()
{
	return this->type;
}
