#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 Model;
uniform mat4 ViewProj;

void main()
{
	FragPos = vec3(Model * vec4(aPos, 1.0f));
	//TODO:out
	Normal = mat3(transpose(inverse(Model))) * aNormal;
	TexCoords = aTexCoords;
	gl_Position = ViewProj * Model * vec4(aPos, 1.0f);
}
