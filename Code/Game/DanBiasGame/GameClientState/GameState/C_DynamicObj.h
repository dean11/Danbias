#pragma once
#include "../C_Object.h"
namespace DanBias
{
	namespace Client
	{
class C_DynamicObj : private C_Object
{
private:
	struct myData;
	myData* privData;
public:
	C_DynamicObj(void);
	virtual ~C_DynamicObj(void);
	void Init(ModelInitData modelInit);
	void setPos();

	void Render();
	void Release();
};};};

