#pragma once
#include <glm/glm.hpp>
#include <memory>
#include "IntersectData.h"

enum class GeometryType
{
	Base,
	Sphere,
	Plane
};
enum class PhysicsType
{
	Static,
	Kinematic,
	Dynamic
};
class PhysicsObject
{
public:
	PhysicsObject(glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f), float velocity = 0.0f) 
		: m_Position(position), m_Velocity(velocity)
	{
	}

	virtual void Integrate(float deltaTime)
	{
		m_Position += m_Velocity * deltaTime;
	}

	virtual glm::vec3 GetPos()const { return m_Position; }
	virtual glm::vec3 GetVelocity()const { return m_Velocity; }

	virtual IntersectData Intersect(std::shared_ptr<PhysicsObject> other)
	{
		return IntersectData();
	}

	virtual void SetPos(glm::vec3 pos) { m_Position = pos; }
	virtual void SetVelocity(glm::vec3 vec) { m_Velocity = vec; }

	//Sphere
	//virtual glm::vec3 GetCenter()const = 0;
	//virtual float GetRadius()const = 0;

	GeometryType Gtype = GeometryType::Base;
	PhysicsType Ptype = PhysicsType::Static;
protected:
	glm::vec3 m_Position;
	glm::vec3 m_Velocity;
};

