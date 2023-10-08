#pragma once
#include <glm/glm.hpp>

class IntersectData
{
public:
	IntersectData(bool is_Intersecting = false, glm::vec3 intersectVec = glm::vec3(0.0)):
		isIntersecting(is_Intersecting),
		IntersectVec(intersectVec)
	{}

	bool isIntersecting;
	glm::vec3 IntersectVec;
};