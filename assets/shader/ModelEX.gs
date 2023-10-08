#version 450 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VS_OUT
{
	vec2 texCoords;
}gs_in[];

out vec2 TexCoords;

uniform mat4 Proj;
uniform float time;

vec4 explode(vec4 pos, vec3 normal)
{
	vec3 direction = normal * ((sin(time) + 1.0))*0.5;
	return pos + vec4(direction, 0.0);
}

void main()
{
	vec3 a = vec3(gl_in[0].gl_Position - gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position - gl_in[1].gl_Position);
	vec3 normal = normalize(abs(cross(a, b)));

	//gl_Position = Proj * (explode(gl_in[0].gl_Position, normal));
	gl_Position = Proj * (gl_in[0].gl_Position);
	TexCoords = gs_in[0].texCoords;
	EmitVertex();

	//gl_Position = Proj *  (explode(gl_in[1].gl_Position, normal));
	gl_Position = Proj * (gl_in[1].gl_Position);
	TexCoords = gs_in[1].texCoords;
	EmitVertex();

	//gl_Position = Proj *  (explode(gl_in[2].gl_Position, normal));
	gl_Position = Proj * (gl_in[2].gl_Position);
	TexCoords = gs_in[2].texCoords;
	EmitVertex();

	EndPrimitive();
}
