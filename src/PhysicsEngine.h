#pragma once
#include <vector>
#include <memory>
#include "PhysicsObject.h"

class PhysicsEngine
{
public:
	void AddObject(std::shared_ptr<PhysicsObject> object)
	{
		m_Objects.push_back(object);
	}
	void Simulate(float deltaTime);
	void HandleCollision();

	std::shared_ptr<PhysicsObject> GetObject (int index)const
	{
		return m_Objects[index];
	}
	unsigned int GetSize()const
	{
		return m_Objects.size();
	}
private:
	std::vector<std::shared_ptr<PhysicsObject>> m_Objects;
};