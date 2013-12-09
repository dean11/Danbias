#pragma once
#include "../C_Object.h"
namespace DanBias
{
	namespace Client
	{
class C_Player : public C_Object
{
private:
	struct myData;
	myData* privData;
	//Oyster::Graphics:: LIght
public:
	C_Player(void);
	~C_Player(void);
	void Init();
	void setPos();

	void Render();
	void Release();

};};};

