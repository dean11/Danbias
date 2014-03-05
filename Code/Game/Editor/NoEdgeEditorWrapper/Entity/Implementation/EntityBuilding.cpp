#include "..\Header\EntityBuilding.h"


using namespace Oyster::Math;

EntityBuilding::EntityBuilding( NoEdgeType_Buildings gameType, Float3 position )
	:	Entity(EntityType_Building, gameType)
{
	Entity::EntityInitDesc d;
	d.mesh.modelPath = L"";
	d.rigidBody.cgfFile = L"";
	d.general.position = position;
	d.general.rotation = Quaternion::identity;
	d.general.scale = Float3(1.0f);

	switch (gameType)
	{
		case NoEdgeType_Buildings_Building1:
			d.mesh.modelPath = L"structure_corporation.dan";
			d.rigidBody.cgfFile = L"structure_corporation.cgf";
		break;
		case NoEdgeType_Buildings_Building2:
			d.mesh.modelPath = L"structure_platform.dan";
			d.rigidBody.cgfFile = L"structure_platform.cgf";
		break;

		default:
			d.mesh.modelPath = L"";
		break;
	}

	if(!EntityInitialize(d))
	{
		printf("Failed to initate building!\n");
	}
}
EntityBuilding::~EntityBuilding()
{
}



