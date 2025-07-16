#version 330 core

out vec4 color;

uniform vec3 colorCircle;

void main()
{
	color = vec4(colorCircle.x, colorCircle.y, colorCircle.z, 1.0f);
}