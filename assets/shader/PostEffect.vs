#version 450 core
layout(location = 0)in vec3 aPos;
layout(location = 1)in vec2 aTexCoords;

out float ColorEle;
out vec2 TexCoords;

void main()
{
	ColorEle = aTexCoords.x - aTexCoords.y;
	ColorEle = (ColorEle + 1.0) * 0.5;
	TexCoords = aTexCoords;
	gl_Position = vec4(aPos, 1.0);
}