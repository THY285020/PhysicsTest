#version 450 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT
{
	vec3 normal;
}gs_in[];

uniform mat4 Proj; 
uniform float time;

void GenerateLine(int index, vec4 offset)
{

	gl_Position = Proj * (gl_in[index].gl_Position + offset);
	EmitVertex();
	gl_Position = Proj * (gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * 0.3 + offset);
	EmitVertex();

	EndPrimitive();
}

void main()
{	
	vec3 a = vec3(gl_in[0].gl_Position - gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position - gl_in[1].gl_Position);
	vec4 offset = vec4(normalize(abs(cross(a, b)))*((sin(time) + 1.0))*0.5, 0.0);

	GenerateLine(0, offset);
	GenerateLine(1, offset);
	GenerateLine(2, offset);
}