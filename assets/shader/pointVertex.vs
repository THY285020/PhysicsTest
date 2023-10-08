#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 Model;
uniform mat4 ViewProj;
uniform float PointSize;

void main()
{
	gl_Position = ViewProj * Model * vec4(aPos, 1.0);
	gl_PointSize = PointSize;
}