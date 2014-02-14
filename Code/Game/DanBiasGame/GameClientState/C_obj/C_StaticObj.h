#ifndef DANBIAS_CLIENT_CSTATICOBJECT_H
#define DANBIAS_CLIENT_CSTATICOBJECT_H
#include "../C_Object.h"
namespace DanBias
{
	namespace Client
	{
class C_StaticObj : public C_Object 
{
private:
public:
	C_StaticObj(void);
	virtual ~C_StaticObj(void);
	bool Init(ModelInitData modelInit);

};};};
#endif