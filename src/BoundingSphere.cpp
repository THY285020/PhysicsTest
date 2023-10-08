#include "BoundingSphere.h"
#include "Plane.h"
IntersectData BoundingSphere::Intersect(std::shared_ptr<PhysicsObject> other)
{
	m_center = PhysicsObject::m_Position;
	if (other->Gtype == GeometryType::Sphere)
	{
		float radiusSum = m_radius + std::dynamic_pointer_cast<BoundingSphere>(other)->GetRadius();
		glm::vec3 distanceVec = (m_center - std::dynamic_pointer_cast<BoundingSphere>(other)->GetCenter());
		float centerDistance = sqrt(distanceVec.x * distanceVec.x + distanceVec.y * distanceVec.y + distanceVec.z * distanceVec.z);
		float Distance = centerDistance - radiusSum;
		return IntersectData(Distance < 0, Distance);
	}
	if (other->Gtype == GeometryType::Plane)
	{
		glm::vec3 center = m_center;
		float radius = m_radius;
		float centerLength = sqrt(center.x * center.x + center.y * center.y + center.z * center.z);
		float distance = centerLength * abs(glm::dot(std::dynamic_pointer_cast<Plane>(other)->GetNormal(), glm::normalize(center))) - std::dynamic_pointer_cast<Plane>(other)->GetOffset();

		float  Dis = distance - radius;
		return IntersectData(Dis < 0, Dis);
	}
	return IntersectData(false, 0);
}
