#pragma once

#include "IntersectData.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class AABB
{
public:

	AABB(glm::vec3 max_Vertex, glm::vec3 min_Vertex) :
		maxVertex(max_Vertex),
		minVertex(min_Vertex)
	{}
	IntersectData intersectAABB(const AABB& other);

	glm::vec3 GetMin()const { return minVertex; }
	glm::vec3 GetMax()const { return maxVertex; }

private:
	glm::vec3 maxVertex;
	glm::vec3 minVertex;
};

