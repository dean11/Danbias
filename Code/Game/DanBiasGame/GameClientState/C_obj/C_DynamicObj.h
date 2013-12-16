#ifndef DANBIAS_CLIENT_CDYNAMICOBJECT_H
#define DANBIAS_CLIENT_CDYNAMICOBJECT_H
#include "../C_Object.h"
namespace DanBias
{
	namespace Client
	{
class C_DynamicObj : public C_Object
{
private:
	struct myData;
	myData* privData;
public:
	C_DynamicObj(void);
	virtual ~C_DynamicObj(void);
	void Init(ModelInitData modelInit);
	void setPos(Oyster::Math::Float4x4 world);

	void Render();
	void Release();
};};};
#endif
