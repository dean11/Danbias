#pragma once
#include "../C_Object.h"
namespace DanBias
{
	namespace Client
	{
class C_StaticObj : public C_Object 
{
private:
	struct myData;
	myData* privData;
public:
	C_StaticObj(void);
	virtual ~C_StaticObj(void);
	void Init(ModelInitData modelInit);
	void setPos();

	void Render();
	void Release();
};};};

