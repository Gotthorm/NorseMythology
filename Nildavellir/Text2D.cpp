// TEXT2DMANAGER.CPP

#include "Text2D.h"
//#include "glext.h"
#include "Vanaheimr.h"
#include "KTX.h"

void Text2D::init(int width, int height, const char* font)
{
	GLuint vs = OpenGLInterface::LoadShader("Media/Shaders/text2d.vs.glsl", GL_VERTEX_SHADER, true);
	GLuint fs = OpenGLInterface::LoadShader("Media/Shaders/text2d.fs.glsl", GL_FRAGMENT_SHADER, true);

	buffer_width = width;
	buffer_height = height;

	text_program = OpenGLInterface::CreateProgram();
	OpenGLInterface::AttachShader(text_program, vs);
	OpenGLInterface::AttachShader(text_program, fs);
	OpenGLInterface::LinkProgram(text_program);

	OpenGLInterface::DeleteShader(fs);
	OpenGLInterface::DeleteShader(vs);

	// glCreateVertexArrays(1, &vao);
	OpenGLInterface::GenVertexArrays(1, &vao);
	OpenGLInterface::BindVertexArray(vao);

	// glCreateTextures(GL_TEXTURE_2D, 1, &text_buffer);
	OpenGLInterface::ActiveTexture( GL_TEXTURE0 ); // Fix from tesselation?
	glGenTextures(1, &text_buffer);
	glBindTexture(GL_TEXTURE_2D, text_buffer);
	OpenGLInterface::TexStorage2D(GL_TEXTURE_2D, 1, GL_R8UI, width, height);

	glPixelStorei( GL_UNPACK_ALIGNMENT, 4 ); // Fix from tesselation?
	font_texture = KTX::load(font ? font : "Media/Textures/cp437_9x16.ktx");

	// Extract the font width and height
	glGetTexLevelParameteriv( GL_TEXTURE_2D_ARRAY, 0, GL_TEXTURE_WIDTH, &m_FontWidth );
	glGetTexLevelParameteriv( GL_TEXTURE_2D_ARRAY, 0, GL_TEXTURE_HEIGHT, &m_FontHeight );

	screen_buffer = new char[width * height];
	memset(screen_buffer, 0, width * height);
}

void Text2D::Shutdown()
{
	delete[] screen_buffer;
	glDeleteTextures(1, &font_texture);
	glDeleteTextures(1, &text_buffer);
	OpenGLInterface::DeleteVertexArrays(1, &vao);
	OpenGLInterface::DeleteProgram(text_program);
}

void Text2D::draw()
{
	OpenGLInterface::UseProgram(text_program);

	const GLfloat color[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	//OpenGLInterface::VertexAttrib1f( 1, -1.0f );
	OpenGLInterface::VertexAttrib1f( 1, 0.0f );
	OpenGLInterface::VertexAttrib4fv( 2, color );

	OpenGLInterface::ActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, text_buffer);
	if (dirty)
	{
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, buffer_width, buffer_height, GL_RED_INTEGER, GL_UNSIGNED_BYTE, screen_buffer);
		dirty = false;
	}
	OpenGLInterface::ActiveTexture(GL_TEXTURE1);
	OpenGLInterface::BindTexture(GL_TEXTURE_2D_ARRAY, font_texture);

	OpenGLInterface::BindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

// TODO: This is meant to be temporary
void Text2D::CopyWideCharBufferToCharBuffer( const wchar_t* source, char* target )
{
	if( source && target )
	{
		for( unsigned int index = 0; index < Platform::kMaxStringLength; ++index )
		{
			wchar_t code = source[ index ];

			if( code == '\0' )
				break;

			if( code < 128 )
			{
				*target = char( code );
			}
			else
			{
				*target = '?';
				if( code >= 0xD800 && code <= 0xD8FF )
				{
					// lead surrogate, skip the next code unit, which is the trail
					++index;
				}
			}
			++target;
		}

		*target = '\0';
	}
}

void Text2D::drawText(const wchar_t* str, int x, int y)
{
	char* dst = screen_buffer + y * buffer_width + x;
	
	// Temporary until this class properly handles wide characters
	//strcpy(dst, str);
	CopyWideCharBufferToCharBuffer( str, dst );

	dirty = true;
}

void Text2D::scroll(int lines)
{
	const char* src = screen_buffer + lines * buffer_width;
	char * dst = screen_buffer;

	memmove(dst, src, (buffer_height - lines) * buffer_width);

	dirty = true;
}

void Text2D::print(const char* str)
{
	const char* p = str;
	char c;
	char* dst = screen_buffer + cursor_y * buffer_width + cursor_x;

	while (*p != 0)
	{
		c = *p++;
		if (c == '\n')
		{
			cursor_y++;
			cursor_x = 0;
			if (cursor_y >= buffer_height)
			{
				cursor_y--;
				scroll(1);
			}
			dst = screen_buffer + cursor_y * buffer_width + cursor_x;
		}
		else
		{
			*dst++ = c;
			cursor_x++;
			if (cursor_x >= buffer_width)
			{
				cursor_y++;
				cursor_x = 0;
				if (cursor_y >= buffer_height)
				{
					cursor_y--;
					scroll(1);
				}
				dst = screen_buffer + cursor_y * buffer_width + cursor_x;
			}
		}
	}

	dirty = true;
}

void Text2D::moveCursor(int x, int y)
{
	cursor_x = x;
	cursor_y = y;
}

void Text2D::clear()
{
	memset(screen_buffer, 0, buffer_width * buffer_height);
	dirty = true;
	cursor_x = 0;
	cursor_y = 0;
}
