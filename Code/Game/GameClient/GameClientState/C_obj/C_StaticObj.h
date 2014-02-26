#ifndef DANBIAS_CLIENT_CSTATICOBJECT_H
#define DANBIAS_CLIENT_CSTATICOBJECT_H
#include "../C_Object.h"
#include "ObjectDefines.h"
namespace DanBias
{
	namespace Client
	{
class C_StaticObj : public C_Object 
{
private:
	const GameLogic::ObjectSpecialType gameObjectType;
public:
	C_StaticObj(GameLogic::ObjectSpecialType type = GameLogic::ObjectSpecialType_Unknown);
	virtual ~C_StaticObj(void);
	bool Init(ModelInitData modelInit);
	inline GameLogic::ObjectSpecialType GetGameObjectType() const { return this->gameObjectType; }

};};};
#endif