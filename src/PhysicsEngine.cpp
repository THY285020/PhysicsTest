#include "PhysicsEngine.h"
#include "IntersectData.h"
#include "BoundingSphere.h"
#include "Plane.h"

void PhysicsEngine::Simulate(float deltaTime)
{
	for (auto& object : m_Objects)
	{
		object->Integrate(deltaTime);
	}
}

void PhysicsEngine::HandleCollision()
{
	for (int i = 0; i < m_Objects.size(); ++i)
	{
		for (int j = i + 1; j < m_Objects.size(); ++j)
		{
			IntersectData intersectData;
			
			if (m_Objects[i]->Gtype == GeometryType::Sphere)
			{
				intersectData = std::dynamic_pointer_cast<BoundingSphere>(m_Objects[i])
					->Intersect((m_Objects[j]));
			}
			else if (m_Objects[i]->Gtype == GeometryType::Plane)
			{
				intersectData = std::dynamic_pointer_cast<Plane>(m_Objects[i])
					->Intersect((m_Objects[j]));
			}
			if (intersectData.isIntersecting)
			{
				if (m_Objects[i]->Ptype == PhysicsType::Dynamic)
				{
					m_Objects[i]->SetVelocity(glm::vec3(-1.0) * m_Objects[i]->GetVelocity());
				}
				if (m_Objects[i]->Ptype == PhysicsType::Dynamic)
				{
					m_Objects[j]->SetVelocity(glm::vec3(-1.0) * m_Objects[j]->GetVelocity());
				}
			}
		}
	}
}
