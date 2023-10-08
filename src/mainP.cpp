#include "Shader.h"
#include "Model.hpp"
#include "Camera.h"
#include "SphereRenderer.hpp"
#include "PlaneRenderer.hpp"
#include "CubeRenderer.hpp"
#include "PhysicsEngine.h"
#include "BoundingSphere.h"
#include "Plane.h"

#include <iostream>
#include <array>
#include <glad/glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::vec3 cameraPos = { 0.0, 0.0, 1.5 };
glm::vec3 cameraFront = { 0.0, 0.0, -1.0 };
glm::vec3 cameraUp = { 0.0, 1.0, 0.0 };
Camera camera(cameraPos, cameraFront, cameraUp);

const unsigned int screenWidth = 1280;
const unsigned int screenHeight = 720;

bool firstMouse = true;
float yaw = -90.f;
float pitch = 0.0f;
float lastX = screenWidth / 2.0f;
float lastY = screenHeight / 2.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void processInput(GLFWwindow* window);

unsigned int loadCubeTexture(std::array<std::string, 6> textures_faces);

std::array<std::string, 6> textures_faces =
{
	"E:/3DModel/assets/texture/skybox/right.jpg",
	"E:/3DModel/assets/texture/skybox/left.jpg",
	"E:/3DModel/assets/texture/skybox/top.jpg",
	"E:/3DModel/assets/texture/skybox/bottom.jpg",
	"E:/3DModel/assets/texture/skybox/front.jpg",
	"E:/3DModel/assets/texture/skybox/back.jpg"
};

int main()
{
#pragma region Configuration
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//主版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//子版本
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//使用core模式
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Physics", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);//将创建的窗口设置为主线程的上下文
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//注册到回调函数
	glfwSetCursorPosCallback(window, mouse_callback);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glEnable(GL_DEPTH_TEST);
#pragma endregion
	//------------------------Sphere
	Shader shaderSphere("E:/3DModel/assets/shader/SphereRender/Sphere.vs", "E:/3DModel/assets/shader/SphereRender/Sphere.fs");
	shaderSphere.use();

	//glm::mat4 modelSphere = glm::mat4(1.0f);
	//modelSphere = glm::translate(modelSphere, glm::vec3(3.0f, 1.0f, -1.0f));
	//modelSphere = glm::rotate(modelSphere, glm::radians(0.0f), glm::vec3(0.0, 0.0, 1.0));
	//modelSphere = glm::scale(modelSphere, glm::vec3(0.8f, 0.8f, 0.8f));

	glUniform3fv(glGetUniformLocation(shaderSphere.ID, "cameraPos"), 1, glm::value_ptr(cameraPos));
	glUniform4fv(glGetUniformLocation(shaderSphere.ID, "SphereColor"), 1, glm::value_ptr(glm::vec4(1.0f, 0.65f, 0.0f, 1.0f)));

	//glm::mat4 modelSphere1 = glm::mat4(1.0f);
	//modelSphere1 = glm::translate(modelSphere1, glm::vec3(6.0f, 1.0f, -1.0f));
	//modelSphere1 = glm::rotate(modelSphere1, glm::radians(0.0f), glm::vec3(0.0, 0.0, 1.0));
	//modelSphere1 = glm::scale(modelSphere1, glm::vec3(0.5f, 0.5f, 0.5f));

	SphereRenderer sphereRenderer;
	sphereRenderer.Create();

	//------------------------Plane
	Shader shaderPlane("E:/3DModel/assets/shader/PlaneRender/Plane.vs", "E:/3DModel/assets/shader/PlaneRender/Plane.fs");
	shaderPlane.use();

	//glm::mat4 modelPlane = glm::mat4(1.0f);
	//modelPlane = glm::translate(modelPlane, glm::vec3(0.0f, -2.0f, -1.0f));
	//modelPlane = glm::rotate(modelPlane, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
	//modelPlane = glm::scale(modelPlane, glm::vec3(2.0f, 2.0f, 2.0f));

	glUniform3fv(glGetUniformLocation(shaderPlane.ID, "cameraPos"), 1, glm::value_ptr(cameraPos));
	glUniform4fv(glGetUniformLocation(shaderPlane.ID, "PlaneColor"), 1, glm::value_ptr(glm::vec4(1.0f, 0.0f, 0.65f, 1.0f)));

	PlaneRenderer planeRenderer;
	planeRenderer.Create();

	//------------------------Cube
	Shader shaderCube("E:/3DModel/assets/shader/CubeRender/Cube.vs", "E:/3DModel/assets/shader/CubeRender/Cube.fs");
	shaderCube.use();

	glm::mat4 modelCube = glm::mat4(1.0f);
	modelCube = glm::translate(modelCube, glm::vec3(-3.0f, 2.0f, -1.0f));
	modelCube = glm::rotate(modelCube, glm::radians(45.0f), glm::vec3(0.0, 1.0, 0.0));
	modelCube = glm::scale(modelCube, glm::vec3(1.0f, 1.0f, 1.0f));

	glUniform3fv(glGetUniformLocation(shaderCube.ID, "cameraPos"), 1, glm::value_ptr(cameraPos));
	glUniform4fv(glGetUniformLocation(shaderCube.ID, "CubeColor"), 1, glm::value_ptr(glm::vec4(0.65f, 0.0f, 1.0f, 1.0f)));

	CubeRenderer cubeRenderer;
	cubeRenderer.Create();

	//---------------------------Physics
	PhysicsEngine pe;
	std::shared_ptr<BoundingSphere> bs0 = std::make_shared<BoundingSphere>(glm::vec3(3.0f, 1.0f, -1.0f), 0.8f);
	bs0->SetVelocity(glm::vec3(0.2f, 0.0f, 0.0f));
	std::shared_ptr<BoundingSphere> bs1 = std::make_shared<BoundingSphere>(glm::vec3(6.0f, 1.0f, -1.0f), 0.5f);
	bs1->SetVelocity(glm::vec3(-0.2f, 0.0f, 0.0f));

	std::shared_ptr<Plane> bs2 = std::make_shared<Plane>(glm::vec3(1.0f, 0.0f, 0.0f), 2.0f);
	pe.AddObject(bs0);
	pe.AddObject(bs1);
	pe.AddObject(bs2);

	float deltaTime = 0.f;
	float lastTime = 0.f;
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		float currentTime = static_cast<float>(glfwGetTime());
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		pe.Simulate(deltaTime);
		pe.HandleCollision();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view = camera.getView();
		glm::mat4 proj = camera.getProjection();

		shaderPlane.use();

		glm::mat4 modelPlane = glm::mat4(1.0f);
		modelPlane = glm::translate(modelPlane, pe.GetObject(2)->GetPos());
		modelPlane = glm::rotate(modelPlane, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		modelPlane = glm::scale(modelPlane, glm::vec3(2.0f, 2.0f, 2.0f));

		glUniformMatrix4fv(glGetUniformLocation(shaderPlane.ID, "Model"), 1, GL_FALSE, glm::value_ptr(modelPlane));
		glUniformMatrix4fv(glGetUniformLocation(shaderPlane.ID, "View"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shaderPlane.ID, "Proj"), 1, GL_FALSE, glm::value_ptr(proj));

		planeRenderer.Draw();
//-------------------------------test
		glm::mat4 modelSphere = glm::mat4(1.0f);
		modelSphere = glm::translate(modelSphere, pe.GetObject(0)->GetPos());
		modelSphere = glm::rotate(modelSphere, glm::radians(0.0f), glm::vec3(0.0, 0.0, 1.0));
		modelSphere = glm::scale(modelSphere, glm::vec3(bs0->GetRadius()));
		shaderSphere.use();
		glUniformMatrix4fv(glGetUniformLocation(shaderSphere.ID, "Model"), 1, GL_FALSE, glm::value_ptr(modelSphere));
		glUniformMatrix4fv(glGetUniformLocation(shaderSphere.ID, "View"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shaderSphere.ID, "Proj"), 1, GL_FALSE, glm::value_ptr(proj));

		sphereRenderer.Draw();

		glm::mat4 modelSphere1 = glm::mat4(1.0f);
		modelSphere1 = glm::translate(modelSphere1, pe.GetObject(1)->GetPos());
		modelSphere1 = glm::rotate(modelSphere1, glm::radians(0.0f), glm::vec3(0.0, 0.0, 1.0));
		modelSphere1 = glm::scale(modelSphere1, glm::vec3(bs1->GetRadius()));
		glUniformMatrix4fv(glGetUniformLocation(shaderSphere.ID, "Model"), 1, GL_FALSE, glm::value_ptr(modelSphere1));
		sphereRenderer.Draw();
//-----------------------------------test
		shaderCube.use();
		glUniformMatrix4fv(glGetUniformLocation(shaderCube.ID, "Model"), 1, GL_FALSE, glm::value_ptr(modelCube));
		glUniformMatrix4fv(glGetUniformLocation(shaderCube.ID, "View"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shaderCube.ID, "Proj"), 1, GL_FALSE, glm::value_ptr(proj));

		cubeRenderer.Draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	shaderSphere.deleteProgram();
	shaderPlane.deleteProgram();

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	camera.setAspectRatio(float(width) / float(height));
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 0.05f; // adjust accordingly
	float cameraUpSpeed = 0.01f;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cameraPos += cameraUpSpeed * cameraUp;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cameraPos -= cameraUpSpeed * cameraUp;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	camera.setPos(cameraPos);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{
		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);

		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;//翻转y坐标
		lastX = xpos;
		lastY = ypos;

		float sensitivity = 0.05;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(front);
		camera.setFront(cameraFront);
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_RELEASE)
	{
		firstMouse = true;
	}
}

unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, channel;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &width, &height, &channel, 0);
	if (data)
	{
		GLenum format;
		if (channel == 1)
			format = GL_RED;
		else if (channel == 3)
			format = GL_RGB;
		else if (channel == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

unsigned int loadCubeTexture(std::array<std::string, 6> textures_faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, channel;
	unsigned char* data;
	for (unsigned int i = 0; i < 6; ++i)
	{
		data = stbi_load(textures_faces[i].c_str(), &width, &height, &channel, 0);
		if (data)
		{
			GLenum format;
			if (channel == 1)
				format = GL_RED;
			else if (channel == 3)
				format = GL_RGB;
			else if (channel == 4)
				format = GL_RGBA;

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << textures_faces[i].c_str() << std::endl;
			stbi_image_free(data);
			break;
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}


