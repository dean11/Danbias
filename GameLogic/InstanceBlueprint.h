/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef GAME_INSTANCEBLUEPRINT_H
#define GAME_INSTANCEBLUEPRINT_H

#include <string>
#include <vector>
#include "OysterMath.h"

namespace GameLogic
{
	struct InstanceBlueprint
	{
		enum Result { Success, Failure };		
		static Result loadFromFile( ::std::vector<InstanceBlueprint*> &output, ::std::vector<::std::string> &idOutput, const ::std::string &entityFile, const ::Oyster::Math::Float4x4 &transform = ::Oyster::Math::Float4x4::identity );

		::std::string name, objFileName, description;
		float mass;
		::Oyster::Math::Float3 centerOfMass;
		struct { ::Oyster::Math::Float3 halfSize, position; } hitBox;
		// engineData
		int hullPoints, shieldPoints;

		struct
		{
			::Oyster::Math::Float maxSpeed, deAcceleration;
			struct{ ::Oyster::Math::Float forward, backward, horizontal, vertical; } acceleration;
		} movementProperty;

		struct
		{
			::Oyster::Math::Float maxSpeed, deAcceleration;
			struct{ ::Oyster::Math::Float pitch, yaw, roll; } acceleration;
		} rotationProperty;

		InstanceBlueprint( );
	};
}

#endif