#include "..\Header\EntityPickup.h"

using namespace Oyster::Math;

EntityPickup::EntityPickup( NoEdgeType_Pickup gameType )
	:	Entity(EntityType_Pickup, gameType)
{
	Entity::EntityInitDesc d;
	d.mesh.modelPath = L"";
	d.general.position = Float3(0.0f);
	d.general.rotation = Quaternion::identity;
	d.general.scale = Float3(1.0f);

	switch (gameType)
	{
		case NoEdgeType_Pickup_HealthPackSmal:
			d.mesh.modelPath = L"generic_cube.dan";
		break;
		case NoEdgeType_Pickup_HealthPackMedium:
			d.mesh.modelPath = L"generic_cube.dan";
		break;
		case NoEdgeType_Pickup_HealthPackLarge:
			d.mesh.modelPath = L"generic_cube.dan";
		break;

		default:
			d.mesh.modelPath = L"";
		break;
	}

	if(!EntityInitialize(d))
	{
		printf("Failed to initate \"%s\"\n", d.mesh.modelPath.c_str());
	}
}
EntityPickup::~EntityPickup()
{
}
