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
public:
	C_DynamicObj(void);
	virtual ~C_DynamicObj(void);
	bool Init(ModelInitData modelInit);

};};};
#endif