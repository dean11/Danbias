#ifndef DANBIAS_CLIENT_CBEAM_H
#define DANBIAS_CLIENT_CBEAM_H

#include "C_DynamicObj.h"
#include "OysterMath.h"

namespace DanBias { namespace Client
{
	class C_Beam : public C_DynamicObj
	{
	public:
		C_Beam();
		C_Beam
		(
			int ownerID,
			const ::Oyster::Math::Float3 &upNormal,
			const ::Oyster::Math::Float3 &startPoint,
			const ::Oyster::Math::Float3 &endPoint,
			const ::Oyster::Math::Float  &beamRadius,
			const ::Oyster::Math::Float  &lifeTime
		);
		virtual ~C_Beam();

		C_Beam & operator = ( const C_Beam &beam );

		void updateWorld();

		/** @return true if successfully loaded */
		bool LoadModel( const ::std::wstring &fileName );

		/** @return true if expired */
		bool Update( ::Oyster::Math::Float deltaTime );

	private:
		::Oyster::Math::Float lifeTime;
		::Oyster::Math::Float4x4 lookAtRotationMatrix; // because I can't seem to get RotQ_LookDir to work, while I know that RotM_LookDir works
	};
} }

#endif