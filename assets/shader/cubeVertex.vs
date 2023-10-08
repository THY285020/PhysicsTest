#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;


out vec3 TexCoords;
out vec3 Normal;
out vec3 Pos;

uniform mat4 Model;
uniform mat4 ViewProj;

void main()
{

	Normal = mat3(transpose(inverse(Model))) * aNormal;
	TexCoords = aPos;
	Pos = vec3(Model * vec4(aPos, 1.0));
	vec4 pos = ViewProj * Model * vec4(aPos, 1.0f);
	gl_Position = pos;
}
