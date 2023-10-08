#pragma once
#include <string>
#include <glad/glad.h>
class Shader
{
public:
	GLuint ID;
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath);

	void use();
	void setInt(const std::string name, int value) const;
	void deleteProgram() { glDeleteProgram(ID);}

private:
	void checkErrors(unsigned int shader, std::string type);
};

