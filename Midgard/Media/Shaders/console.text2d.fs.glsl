#version 450 core

// Set origin to top-left; we want left-to-right, top-to-bottom
layout (origin_upper_left) in vec4 gl_FragCoord;

// Output a single color
layout (location = 0) out vec4 o_color;

// Character map
layout (binding = 0) uniform isampler2D text_buffer;

// Font
layout (binding = 1) uniform isampler2DArray font_texture;

// Character color map
layout (binding = 2) uniform isampler1D text_color_buffer;

uniform vec2 fontScalar;

void main(void)
{
	// Get integer fragment coordinate.  The pixel location?
	ivec2 frag_coord = ivec2(gl_FragCoord.xy);

	// Get size of font
	ivec2 char_size = ivec2(textureSize(font_texture, 0).xy * fontScalar);

	// Find location in text buffer.  The array indexes.
	ivec2 char_location = frag_coord / char_size;

	// Find texel within character.  The pixel offsets within the text buffer entry.
	ivec2 texel_coord = ivec2((frag_coord % char_size) / fontScalar);

	// Fetch character index
	int character = texelFetch(text_buffer, char_location, 0).x;

	// Fetch the given texel
	float val = texelFetch(font_texture, ivec3(texel_coord, character), 0).x;

	if (val == 0.0)
		discard;

	uint data = texelFetch(text_color_buffer, char_location.y, 0).x;

	uint r = (data>> 24) & 0xff;
    uint g = (data>> 16) & 0xff;
    uint b = (data>> 8) & 0xff;
    uint a = data & 0xff;
	vec4 new_color = vec4( float(r) / 255.0, float(g) / 255.0, float(b) / 255.0, float(a) / 255.0 );

	// Write the texel data out
	o_color = new_color; 
}