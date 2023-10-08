#pragma once

#include "IntersectData.h"
#include "PhysicsObject.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class BoundingSphere : public PhysicsObject
{
public:

	BoundingSphere(const glm::vec3& center, float radius):m_center(center) , m_radius(radius)
	{
		PhysicsObject::m_Position = m_center;
		PhysicsObject::Gtype = GeometryType::Sphere;
		PhysicsObject::Ptype = PhysicsType::Dynamic;
	}

	virtual IntersectData Intersect(std::shared_ptr<PhysicsObject> other) override;

	glm::vec3 GetCenter(){ return PhysicsObject::m_Position; }
	float GetRadius() { return m_radius; }

private:
	glm::vec3 m_center;
	float m_radius;
};