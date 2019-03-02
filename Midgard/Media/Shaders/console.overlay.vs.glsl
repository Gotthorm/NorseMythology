#version 450 core

// "color" is an input vertex attribute
layout (location = 1) in vec4 inputColor;

layout (location = 2) in vec4 clip;

// "vs_color" is an output that will be sent to the next shader stage
out vec4 vsColor;

void main(void)
{
	const vec4 vertices[] = vec4[](	vec4(clip[2], clip[3], 0.0, 1.0),
									vec4(clip[0], clip[3], 0.0, 1.0),
									vec4(clip[2], clip[1], 0.0, 1.0),
									vec4(clip[0], clip[1], 0.0, 1.0));

    gl_Position = vertices[gl_VertexID];

	vsColor = inputColor;
}
