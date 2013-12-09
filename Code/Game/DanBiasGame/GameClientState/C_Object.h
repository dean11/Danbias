#pragma once
namespace DanBias
{
	namespace Client
	{
class C_Object
{
private:
	
public:

	virtual void Init() = 0;
	virtual void setPos() = 0;

	virtual void Render() = 0;
	virtual void Release() = 0;
};};};

