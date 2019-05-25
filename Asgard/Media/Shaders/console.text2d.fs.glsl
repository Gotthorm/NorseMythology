#version 450 core

// Set origin to top-left; we want left-to-right, top-to-bottom
layout (origin_upper_left) in vec4 gl_FragCoord;

// Output a single color
layout (location = 0) out vec4 o_color;

// Character map
layout (binding = 0) uniform isampler2D text_buffer;

// Font
layout (binding = 1) uniform isampler2DArray font_texture;

// Color map
layout (binding = 3) uniform sampler1D color_buffer;

// Input from the vertex shader
in vec4 vs_color;

// Default font scale is 100% unless overriden by the caller
uniform vec2 fontScalar = vec2(1.0, 1.0);

void main(void)
{
	// Get integer fragment coordinate
	ivec2 frag_coord = ivec2(gl_FragCoord.xy);

	// Get size of font
	ivec2 char_size = ivec2(textureSize(font_texture, 0).xy * fontScalar);

	// Find location in text buffer
	ivec2 char_location = frag_coord / char_size;

	//
	//o_color = texture(color_buffer, char_location.y).rgba;
	o_color = texture(color_buffer, 0).rgba;

	// Find texel within character
	ivec2 texel_coord = ivec2((frag_coord % char_size) / fontScalar);

	// Fetch character index
	int character = texelFetch(text_buffer, char_location, 0).x;

	// Fetch the given texel
	float val = texelFetch(font_texture, ivec3(texel_coord, character), 0).x;

	// If the texel is empty, exit
	if (val == 0.0)
		discard;

	// Write the texel data out
	o_color = vs_color;
}