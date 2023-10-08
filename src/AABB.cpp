#include "AABB.h"

#define MAX(v1, v2) (v1>v2 ? v1 : v2)

namespace Utils
{
	glm::vec3 C_Max(glm::vec3 v1, glm::vec3 v2)
	{
		float max_x = v1.x > v2.x ? v1.x : v2.x;
		float max_y = v1.y > v2.y ? v1.y : v2.y;
		float max_z = v1.z > v2.z ? v1.z : v2.z;
		return glm::vec3(max_x, max_y, max_z);
	}

	float E_Max(glm::vec3 v)
	{
		return MAX(MAX(v.x, v.y), v.z);
	}
}

IntersectData AABB::intersectAABB(const AABB& other)
{
	glm::vec3 distance1 = minVertex - other.GetMax();
	glm::vec3 distance2 = other.GetMin() - maxVertex;
	glm::vec3 maxDistance = Utils::C_Max(distance1, distance2);

	float maxDis = Utils::E_Max(maxDistance);//用于判断是否相交
	float Dis = maxDistance.length();//距离

	return IntersectData();
}
