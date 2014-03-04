#include "..\Header\EntityLight.h"
#include "DllInterfaces\GFXAPI.h"

using namespace Oyster;

EntityLight::EntityLight(NoEdgeType_Light type)
	: Entity( EntityType_Light, type )
{
	EntityInitDesc d;
	d.general.position = 0.0f;
	d.general.rotation = Oyster::Math::Quaternion::identity;
	d.general.scale = 0.2f;

	switch (type)
	{
		case NoEdgeType_Light_PointLight:
		{
			this->lightData.pointLight.Bright = 1.0f;
			this->lightData.pointLight.Color = 1.0f;
			this->lightData.pointLight.Pos = 0.0f;
			this->lightData.pointLight.Radius = 1000.0f;
			Graphics::API::AddLight(&this->lightData.pointLight);

			d.mesh.modelPath = L"generic_sphere.dan";
		}
		break;
	}

	if(!Entity::EntityInitialize(d))
	{
		printf("Failed to initiate light\n");
	}
}
EntityLight::~EntityLight()
{
	
}
void EntityLight::SetLight(LightData& light)
{
	this->lightData = light;
}

void EntityLight::SetPosition(Oyster::Math::Float3 position)
{
	this->position = position;
	switch (this->entitySubType.et_light)
	{
		case NoEdgeType_Light_PointLight:
			this->lightData.pointLight.Pos = this->position;
		break;
	}
}
void EntityLight::SetRotation(Oyster::Math::Quaternion roation)
{
	this->rotation = rotation;
	switch (this->entitySubType.et_light)
	{
		case NoEdgeType_Light_PointLight: return;
	}
}
void EntityLight::SetScale(Oyster::Math::Float3 scale)
{
	this->scale = scale;
	switch (this->entitySubType.et_light)
	{
		case NoEdgeType_Light_PointLight: 
			//Scale radius?
		break;
	}
}


