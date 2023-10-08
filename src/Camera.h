#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	Camera(glm::vec3 pos, glm::vec3 Front, glm::vec3 Up, float fov = 45.0f)
		:Pos(pos), cameraFront(Front),cameraUp(Up), Fov(fov)
	{}

	void setPos(glm::vec3 pos) { Pos = pos; return; }
	void setAspectRatio(float aspectRatio) { AspectRatio = aspectRatio; return; }
	void setFront(glm::vec3 front) { cameraFront = front; return; }

	glm::mat4 getViewProj()
	{
		return getProjection() * getView();
	}

	glm::mat4 getView() 
	{
		return glm::lookAt(Pos, Pos+cameraFront, cameraUp);
	}
	glm::mat4 getProjection()
	{
		return glm::perspective(glm::radians(Fov), AspectRatio, Near, Far);
	}
private:
	float Fov ;
	float Near = 0.1f;
	float Far = 100.0f;
	float AspectRatio = 16.0f / 9.0f;
	glm::vec3 Pos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
};

