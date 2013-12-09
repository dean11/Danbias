#pragma once
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
	virtual void setPos() = 0;

	virtual void Render() = 0;
	virtual void Release() = 0;
};};};

