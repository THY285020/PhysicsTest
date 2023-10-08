#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;

out vec3 Pos;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
	Normal = mat3(transpose(inverse(Model))) * aNormal;
	Pos =  vec3(Model * vec4(aPos, 1.0));
	TexCoords = aTexCoords;
	gl_Position = Proj * View * Model * vec4(aPos, 1.0);
}

