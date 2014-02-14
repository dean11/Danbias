#include "C_Object.h"
using namespace DanBias::Client;
C_Object::C_Object()
{
	world = Oyster::Math::Float4x4::identity; 
	position = Oyster::Math::Float3::null; 
	rotation = Oyster::Math::Quaternion::identity; 
	scale = Oyster::Math::Float3::null; 

	id = 0;
	model = NULL;
}
C_Object::~C_Object()
{

}
bool C_Object::Init(ModelInitData modelInit)
{
	position = modelInit.position;
	rotation = modelInit.rotation;
	scale = modelInit.scale;
	id = modelInit.id;
	model = Oyster::Graphics::API::CreateModel(modelInit.modelPath);
	if(model == NULL)
		return false;
	model->Visible = modelInit.visible;
	updateWorld();
	return true;
}
void C_Object::updateWorld()
{
	Oyster::Math3D::Float4x4 translation = Oyster::Math3D::TranslationMatrix(this->position); 
	Oyster::Math3D::Float4x4 rot = Oyster::Math3D::RotationMatrix(this->rotation);
	//Oyster::Math3D::Float4x4 scale = Oyster::Math3D::;
	Oyster::Math3D::Float4x4 scale = Oyster::Math3D::Matrix::identity;
	scale.v[0].x = this->scale[0];
	scale.v[1].y = this->scale[1];
	scale.v[2].z = this->scale[2];
	world = translation * rot * scale;

	model->WorldMatrix = world;
}
void C_Object::setWorld(Oyster::Math::Float4x4 world)
{
	model->WorldMatrix = world;
}
Oyster::Math::Float4x4 C_Object::getWorld() const
{
	return world;
}
void C_Object::setPos(Oyster::Math::Float3 newPos)
{
	this->position = newPos;
	updateWorld();
}
void C_Object::addPos(Oyster::Math::Float3 deltaPos)
{
	this->position += deltaPos;
	updateWorld();
}
Oyster::Math::Float3 C_Object::getPos() const
{
	return this->position;
}
void C_Object::setRot(Oyster::Math::Quaternion newRot)
{
	this->rotation = newRot;
	updateWorld();
}
void C_Object::addRot(Oyster::Math::Quaternion deltaRot)
{
	this->rotation += deltaRot;
	updateWorld();
}
Oyster::Math::Quaternion C_Object::getRotation() const
{
	return this->rotation;
}
void C_Object::setScale(Oyster::Math::Float3 newScale)
{
	this->scale = newScale;
	updateWorld();
}
void C_Object::addScale(Oyster::Math::Float3 deltaScale)
{
	this->scale += deltaScale;
	updateWorld();
}
Oyster::Math::Float3 C_Object::getScale() const
{
	return this->scale;
}
int C_Object::GetId() const
{
	return id;
}
void C_Object::Render()
{
	Oyster::Graphics::API::RenderModel(model);
}
void C_Object::Release()
{
	Oyster::Graphics::API::DeleteModel(model);
}