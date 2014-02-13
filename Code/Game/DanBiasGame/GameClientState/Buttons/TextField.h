/********************************************************************
 * Created by Dan Andersson, 2014
 ********************************************************************/
#include <string>
#include "EventButtonGUI.h"
#include "OysterMath.h"

#ifndef DANBIAS_CLIENT_TEXT_FIELD_H
#define DANBIAS_CLIENT_TEXT_FIELD_H

namespace DanBias { namespace Client
{
	template <typename Owner>
	class TextField : public EventButtonGUI<Owner>
	{
	public:
		::std::wstring text;

		TextField( std::wstring textureName, Owner owner, ::Oyster::Math::Float3 centerPos, ::Oyster::Math::Float2 size );
		TextField();


	private:
		::Oyster::Math::Float3 center;
		::Oyster::Math::Float2 reach;

	};
} }

#endif