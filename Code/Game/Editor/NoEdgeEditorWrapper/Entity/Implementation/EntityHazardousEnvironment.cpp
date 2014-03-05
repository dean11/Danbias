#include "..\Header\EntityHazardousEnvironment.h"


	using namespace Oyster::Math;

EntityHazardousEnvironment::EntityHazardousEnvironment( NoEdgeType_HazardEnv gameType, Float3 position )
	:	Entity(EntityType_Building, gameType)
{
	Entity::EntityInitDesc d;
	d.mesh.modelPath = L"";
	d.general.position = position;
	d.general.rotation = Quaternion::identity;
	d.general.scale = Float3(1.0f);

	switch (gameType)
	{
		case NoEdgeType_HazardEnv_CrystalFormation1:
			d.mesh.modelPath = L"nature_crystalFormation1.dan";
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
EntityHazardousEnvironment::~EntityHazardousEnvironment()
{
}
