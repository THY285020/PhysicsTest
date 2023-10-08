#include "BoundingSphere.h"
#include "Plane.h"
IntersectData BoundingSphere::Intersect(std::shared_ptr<PhysicsObject> other)
{
	m_center = PhysicsObject::m_Position;
	if (other->Gtype == GeometryType::Sphere)
	{
		float radiusSum = m_radius + std::dynamic_pointer_cast<BoundingSphere>(other)->GetRadius();
		glm::vec3 distanceVec = ( std::dynamic_pointer_cast<BoundingSphere>(other)->GetCenter() - m_center );
		float centerDistance = sqrt(distanceVec.x * distanceVec.x + distanceVec.y * distanceVec.y + distanceVec.z * distanceVec.z);
		float Distance = centerDistance - radiusSum;

		glm::vec3 direction = glm::normalize(distanceVec);

		return IntersectData(Distance < 0, Distance * direction);
	}
	if (other->Gtype == GeometryType::Plane)
	{
		glm::vec3 center = m_center;
		float radius = m_radius;
		float centerLength = sqrt(center.x * center.x + center.y * center.y + center.z * center.z);
		float distance = centerLength * abs(glm::dot(std::dynamic_pointer_cast<Plane>(other)->GetNormal(), glm::normalize(center))) - std::dynamic_pointer_cast<Plane>(other)->GetOffset();
		
		glm::vec3 direction = glm::normalize(-std::dynamic_pointer_cast<Plane>(other)->GetNormal());

		float  Dis = distance - radius;

		return IntersectData(Dis < 0, Dis * direction);
	}
	return IntersectData();
}
