#include "C_Beam.h"
#include "Utilities.h"

using namespace ::DanBias::Client;
using namespace ::Oyster;
using namespace ::Oyster::Math3D;
using namespace ::Utility::Value;
using ::std::wstring;

C_Beam::C_Beam()
	: C_DynamicObj()
{ this->lifeTime = 0.0f; }

C_Beam::C_Beam( int ownerID, const Float3 &upNormal, const Float3 &startPoint, const Float3 &endPoint, const Float &beamRadius, const Float &lifeTime )
	: C_DynamicObj()
{
	this->lifeTime = lifeTime;
	this->position = Average( startPoint, endPoint );
	this->scale	   = 2.0f * beamRadius;

	Float3 forwardNormal = endPoint - startPoint;
	Float norm = forwardNormal.Dot( forwardNormal );

	if( norm > 0.0000000000001f )
	{
		this->scale.z = Float( ::std::sqrt(norm) );
		forwardNormal /= this->scale.z;
		OrientationMatrix_LookAtDirection( forwardNormal, upNormal, Float3::null, this->lookAtRotationMatrix );
	}
	else
	{
		this->lookAtRotationMatrix = Float4x4::identity;
	}
}

C_Beam::~C_Beam() { /* Do nothing*/ }

C_Beam & C_Beam::operator = ( const C_Beam &beam )
{
	C_DynamicObj::operator=( beam );
	this->lifeTime = beam.lifeTime;
	return *this;
}

void C_Beam::updateWorld()
{
	model->WorldMatrix
		= TranslationMatrix( this->position )
		* this->lookAtRotationMatrix
		* ScalingMatrix( this->scale );
}

bool C_Beam::LoadModel( const wstring &fileName )
{
	if( this->model )
	{
		Graphics::API::DeleteModel( this->model );
	}

	this->model = Graphics::API::CreateModel( fileName );
	this->model->Instanced = false;
	return this->model != nullptr;
}

bool C_Beam::Update( Float deltaTime )
{
	this->lifeTime -= deltaTime;
	if( this->lifeTime <= 0.0f )
	{
		this->lifeTime = 0.0f;
		return true;
	}
	return false;
}