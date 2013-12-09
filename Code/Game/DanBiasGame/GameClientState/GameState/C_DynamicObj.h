#pragma once
namespace DanBias
{
	namespace Client
	{
class C_DynamicObj
{
private:
	struct myData;
	myData* privData;
public:
	C_DynamicObj(void);
	virtual ~C_DynamicObj(void);
	void Init();
	void setPos();

	void Render();
	void Release();
};};};

