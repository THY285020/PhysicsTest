#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glfw3.h>
#include <vector>

class PlaneRenderer
{
public:
	PlaneRenderer()
	{
	}

	void Create()
	{
		float data[] =
		{	
			//pos					normal				texCoord
			 -1.0f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 -1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			  1.0f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
			  1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		};

		glGenVertexArrays(1, &m_PlaneVAO);
		glBindVertexArray(m_PlaneVAO);
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(data), &data[0], GL_STATIC_DRAW);

		unsigned int stride = (3 + 3 + 2) * sizeof(float);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));

		glBindVertexArray(0);
	}

	void Draw()
	{
		glBindVertexArray(m_PlaneVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	~PlaneRenderer()
	{
		glDeleteBuffers(1, &m_PlaneVAO);
	}

private:
	GLuint m_PlaneVAO;
};