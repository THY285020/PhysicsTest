#pragma once
#include "IntersectData.h"
#include "BoundingSphere.h"
#include "PhysicsObject.h"
#include <glm/glm.hpp>

class Plane : public PhysicsObject
{
public:
	Plane(glm::vec3 Normal, float Offset):
		m_Offset(Offset)
	{
		m_Normal = glm::normalize(Normal);
		PhysicsObject::m_Position = m_Normal * m_Offset;
		PhysicsObject::Gtype = GeometryType::Plane;
		PhysicsObject::Ptype = PhysicsType::Kinematic;
	}
	virtual IntersectData Intersect(std::shared_ptr<PhysicsObject> other) override;

	glm::vec3 GetNormal()const { return m_Normal; }
	float GetOffset()const { return m_Offset; }

private:
	glm::vec3 m_Normal;
	float m_Offset;
};

