#version 450 core

in float ColorEle;
in vec2 TexCoords;

uniform sampler2D ScreenTex;
out vec4 fragColor;

void main()
{	
	vec4 ScreenCol = texture(ScreenTex, TexCoords);
	fragColor = vec4(ScreenCol.x * ColorEle, ScreenCol.y * ColorEle, ScreenCol.z * ColorEle, ScreenCol.w) ;
	//fragColor = ScreenCol;
}