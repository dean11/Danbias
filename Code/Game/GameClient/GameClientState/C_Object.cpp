#include "C_Object.h"
using namespace DanBias::Client;
C_Object::C_Object()
{
	world = Oyster::Math::Float4x4::identity; 
	position = Oyster::Math::Float3::null; 
	rotation = Oyster::Math::Quaternion::identity; 
	scale = Oyster::Math::Float3(1); 

	id = 0;
	model = NULL;

	// RB DEBUG 
	type = RB_Type_None;
	// !RB DEBUG 
}
C_Object::~C_Object()
{
	if( this->model )
		this->Release();
}

bool C_Object::Init(ModelInitData modelInit, int Light)
{
	position = modelInit.position;
	rotation = modelInit.rotation;
	scale = modelInit.scale;
	id = modelInit.id;
	model = Oyster::Graphics::API::CreateModel(modelInit.modelPath);
	if(model == NULL)
		return false;
	model->Visible = modelInit.visible;
	model->Tint = modelInit.tint;
	model->GlowTint = modelInit.gtint;
	updateWorld();
	return true;
}
void C_Object::updateWorld()
{
	Oyster::Math3D::Float4x4 translation = Oyster::Math3D::TranslationMatrix(this->position); 
	Oyster::Math3D::Float4x4 rot = Oyster::Math3D::RotationMatrix(this->rotation);
	Oyster::Math3D::Float4x4 scale = Oyster::Math3D::ScalingMatrix(this->scale);
	world = translation * rot * scale;

	model->WorldMatrix = world;
}
Oyster::Math::Float4x4 C_Object::getWorld() const
{
	Oyster::Math3D::Float4x4 translation = Oyster::Math3D::TranslationMatrix(this->position); 
	Oyster::Math3D::Float4x4 rot = Oyster::Math3D::RotationMatrix(this->rotation);
	Oyster::Math3D::Float4x4 scale = Oyster::Math3D::ScalingMatrix(this->scale);
	Oyster::Math3D::Float4x4 world = translation * rot * scale;

	return world;
}
void C_Object::setPos(Oyster::Math::Float3 newPos)
{
	this->position = newPos;
}
void C_Object::addPos(Oyster::Math::Float3 deltaPos)
{
	this->position += deltaPos;
}
int C_Object::GetLight()
{
	return this->light;
}
void C_Object::SetLight(int i)
{
	this->light = i;
}
Oyster::Math::Float3 C_Object::getPos() const
{
	return this->position;
}
void C_Object::setRot(Oyster::Math::Quaternion newRot)
{
	this->rotation = newRot;
}
Oyster::Math::Quaternion C_Object::getRotation() const
{
	return this->rotation;
}
void C_Object::setScale(Oyster::Math::Float3 newScale)
{
	this->scale = newScale;
}
void C_Object::addScale(Oyster::Math::Float3 deltaScale)
{
	this->scale += deltaScale;
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
	if( this->model )
	{
		if(this->model->Visible)
		{
			Oyster::Graphics::API::RenderModel(model);
		}
	}
}
void C_Object::Release()
{
	if( this->model )
	{
		Oyster::Graphics::API::DeleteModel(model);
		this->model = nullptr;
	}
}

Oyster::Math::Float3 C_Object::GetTint()
{
	return model->Tint;
}

Oyster::Math::Float3 C_Object::GetGlowTint()
{
	return model->GlowTint;
}

void C_Object::SetTint(Oyster::Math::Float3 tint)
{
	model->Tint = tint;
}

void C_Object::SetGlowTint(Oyster::Math::Float3 tint)
{
	model->GlowTint = tint;
}

void C_Object::SetVisible(bool visible)
{
	model->Visible = visible;
}

////////////////////////////////////////////////
// RB DEBUG
////////////////////////////////////////////////
bool C_Object::InitRB(RBInitData RBInit)
{
	RBposition = RBInit.position;
	RBrotation = RBInit.rotation;
	RBscale = RBInit.scale;
	type = RBInit.type;
	return true;
}
void C_Object::updateRBWorld()
{
	Oyster::Math3D::Float4x4 translation = Oyster::Math3D::TranslationMatrix(this->RBposition); 
	Oyster::Math3D::Float4x4 rot = Oyster::Math3D::RotationMatrix(this->RBrotation);
	Oyster::Math3D::Float4x4 scale = Oyster::Math3D::ScalingMatrix(this->RBscale);
	RBworld = translation * rot * scale;
}
Oyster::Math::Float4x4 C_Object::getRBWorld() const
{
	return RBworld;
}
void C_Object::setRBPos(Oyster::Math::Float3 newPos)
{
	this->RBposition = newPos;
}
Oyster::Math::Float3 C_Object::getRBPos() const
{
	return this->RBposition;
}
void C_Object::setRBRot(Oyster::Math::Quaternion newRot)
{
	this->RBrotation = newRot;
}
Oyster::Math::Quaternion C_Object::getRBRotation() const
{
	return this->RBrotation;
}
void C_Object::setRBScale(Oyster::Math::Float3 newScale)
{
	this->RBscale = newScale;
}
Oyster::Math::Float3 C_Object::getRBScale() const
{
	return this->RBscale;
}
RB_Type C_Object::getBRtype()const
{
	return this->type;
}
// !RB DEBUG 