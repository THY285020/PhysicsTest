#pragma once
#include <glm/glm.hpp>

class IntersectData
{
public:
	IntersectData(bool is_Intersecting = false, float distance = 0):
		isIntersecting(is_Intersecting),
		Distance(distance)
	{}

	bool isIntersecting;
	float Distance;
};