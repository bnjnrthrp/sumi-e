#version 330

layout (location = 0) in vec3 pos;

void main()
{
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
	gl_PointSize = 1.5f;
}
