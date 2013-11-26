#pragma once
#ifndef Mesh_h
#define Mesh_h

namespace Oyster
{
	namespace Graphics
	{
		namespace Model
		{
			struct Model
			{
				void* info;
				Oyster::Math::Float4x4 WorldMatrix;
				bool Visible;
			};
		}
		
	};
};

#endif