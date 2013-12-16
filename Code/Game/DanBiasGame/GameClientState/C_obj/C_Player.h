#ifndef DANBIAS_CLIENT_CPLAYER_H
#define DANBIAS_CLIENT_CPLAYER_H
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
	void Init(ModelInitData modelInit);
	void setPos();

	void Render();
	void Release();

};};};
#endif

