#pragma once
#ifndef GRAHIC_STRUCTS_H
#define GRAHIC_STRUCTS_H

#include "OysterCollision.h"

namespace GraphicStruct
{

	struct PointLight
	{
		struct{ ::Oyster::Math::Float3 center; ::Oyster::Math::Float radius; } pos;
		::Oyster::Math::Float3 color;
		::Oyster::Math::Float intensty;
	};

	struct LightData
	{
		::Oyster::Math::Float4x4 viewMatrix, projectionMatrix;
		::LinearAlgebra::Vector3<unsigned int> numDispatches;
		unsigned int reservedPadding;
	};
}

#endif