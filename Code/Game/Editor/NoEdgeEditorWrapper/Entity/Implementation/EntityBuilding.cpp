#include "..\Header\EntityBuilding.h"


using namespace Oyster::Math;

EntityBuilding::EntityBuilding( NoEdgeType_Buildings gameType, Float3 position )
	:	Entity(EntityType_Building, gameType)
{
	Entity::EntityInitDesc d;
	d.mesh.modelPath = L"";
	d.general.position = position;
	d.general.rotation = Quaternion::identity;
	d.general.scale = Float3(1.0f);

	switch (gameType)
	{
		case NoEdgeType_Buildings_Building:
			d.mesh.modelPath = L"generic_sphere.dan";
		break;
		case NoEdgeType_Buildings_CrystalFormation:
			d.mesh.modelPath = L"generic_sphere.dan";
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



