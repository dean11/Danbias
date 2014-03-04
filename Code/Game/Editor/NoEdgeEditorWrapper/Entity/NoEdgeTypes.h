#ifndef EDITOR_NOEDGETYPE_H
#define EDITOR_NOEDGETYPE_H

enum NoEdgeType_Special
{
	NoEdgeType_Special_Sky,
	NoEdgeType_Special_Planet,
	
	NoEdgeType_Special_Count,
	NoEdgeType_Special_Unknown  = -1
};
enum NoEdgeType_Collision
{
	NoEdgeType_Collision_Box,
	NoEdgeType_Collision_Sphere,
	NoEdgeType_Collision_Cylinder,
		
	NoEdgeType_Collision_Count,
	NoEdgeType_Collision_Unknown  = -1
};
enum NoEdgeType_Projectiles
{
	NoEdgeType_Projectiles_Stone,
	NoEdgeType_Projectiles_StandardBox,
	
	NoEdgeType_Projectiles_SpikeBox,
	NoEdgeType_Projectiles_Spike,
	NoEdgeType_Projectiles_CrystalShard,

	NoEdgeType_Projectiles_Count,
	NoEdgeType_Projectiles_Unknown  = -1
};
enum NoEdgeType_Interactive
{
	NoEdgeType_Interactive_SpawnPoint,
	NoEdgeType_Interactive_JumpPad,
	NoEdgeType_Interactive_Portal,
	NoEdgeType_Interactive_RedExplosiveBox,

	NoEdgeType_Interactive_Count,
	NoEdgeType_Interactive_Unknown  = -1
};
enum NoEdgeType_Buildings
{
	NoEdgeType_Buildings_Building,
	NoEdgeType_Buildings_CrystalFormation,
	
	NoEdgeType_Buildings_Count,
	NoEdgeType_Buildings_Unknown  = -1
};
enum NoEdgeType_Light
{
	NoEdgeType_Light_PointLight,
	
	NoEdgeType_Light_Count,
	NoEdgeType_Light_Unknown  = -1
};

#endif // !EDITOR_NOEDGETYPE_H
