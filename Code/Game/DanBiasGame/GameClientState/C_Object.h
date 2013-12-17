#ifndef DANBIAS_CLIENT_COBJECT_H
#define DANBIAS_CLIENT_COBJECT_H
#include "DllInterfaces/GFXAPI.h"
namespace DanBias
{
	namespace Client
	{

		struct  ModelInitData
		{
			std::wstring modelPath;
			Oyster::Math::Float4x4 world; 
			bool visible; 
		};

class C_Object
{
private:
	
public:

	virtual void Init(ModelInitData modelInit) = 0;
	virtual void setPos(Oyster::Math::Float4x4 world) = 0;

	virtual void Render() = 0;
	virtual void Release() = 0;
};};};
#endif
