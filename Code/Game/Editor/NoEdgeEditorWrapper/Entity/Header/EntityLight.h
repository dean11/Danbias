#ifndef EDITOR_ENTITYLIGHT_H
#define EDITOR_ENTITYLIGHT_H

#include "..\Header\Entity.h"
#include "DllInterfaces/GFXAPI.h"

class EntityLight	:public Entity
{
public:
	union LightData
	{
		struct { Oyster::Graphics::Definitions::Pointlight pointLight; };
	};

public:
	EntityLight(NoEdgeType_Light type);
	~EntityLight();

	void SetLight(LightData& light);

public: //Overridein from Entity
	void SetPosition(Oyster::Math::Float3 position);
	void SetRotation(Oyster::Math::Quaternion roation);
	void SetScale(Oyster::Math::Float3 scale);

private:
	LightData lightData;
};

#endif // !EDITOR_ENTITYLIGHT_H
