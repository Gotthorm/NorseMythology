#version 450 core

// "color" is an input vertex attribute
layout (location = 1) in vec4 inputColor;

// "vs_color" is an output that will be sent to the next shader stage
out vec4 vsColor;

void main(void)
{
    const vec4 vertices[] = vec4[](vec4( 0.25, -0.25, 0.5, 1.0),
                                   vec4(-0.25, -0.25, 0.5, 1.0),
                                   vec4( 0.25,  0.25, 0.5, 1.0));

    gl_Position = vertices[gl_VertexID];

	vsColor = inputColor;
}                                                                 
