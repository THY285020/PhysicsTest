#include "Plane.h"

IntersectData Plane::Intersect(std::shared_ptr<PhysicsObject> object)
{
	if (object->Gtype == GeometryType::Sphere)
	{
		glm::vec3 center = std::dynamic_pointer_cast<BoundingSphere>(object)->GetCenter();
		float radius = std::dynamic_pointer_cast<BoundingSphere>(object)->GetRadius();
		float centerLength = sqrt(center.x * center.x + center.y * center.y + center.z * center.z);
		float distance = centerLength * glm::dot(m_Normal, center) - m_Offset;

		float  Dis = distance - radius;
		return IntersectData(Dis < 0, Dis);
	}

	return IntersectData(false, 0);
}
