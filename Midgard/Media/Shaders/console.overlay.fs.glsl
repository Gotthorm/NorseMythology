#version 450 core

out vec4 fsColor;

// Input from the vertex shader
in vec4 vsColor;

void main(void)
{
	fsColor = vsColor;
}