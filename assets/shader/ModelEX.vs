#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT
{
	vec2 texCoords;
}vs_out;

uniform mat4 Model;
uniform mat4 View;

void main()
{
	vs_out.texCoords = aTexCoords;
	//vs_out.Normal = normalize(mat3(transpose(inverse(View * Model))) * aNormal);
	gl_Position = View * Model * vec4(aPos, 1.0f);
}