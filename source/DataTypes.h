#pragma once
#include "pch.h"

namespace dae
{
	struct Vertex_PosCol
	{
		Vector3 position{};
		ColorRGB color{ colors::White };
		Vector2 uv{}; 
		Vector3 normal{}; 
		Vector3 tangent{};
		Vector3 viewDirection{}; 
	};

	
}

