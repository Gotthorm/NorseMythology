// TEXT.CPP

#include "OpenGLText.h"
#include "KTX.h"
#include "Platform.h"
//#include "glext.h"
//#include "Vanaheimr.h"
#include "OpenGLShader.h"
#include <codecvt>

namespace Muspelheim
{
	OpenGLText::~OpenGLText()
	{
		delete [] m_ScreenBuffer;
		glDeleteTextures( 1, &font_texture );
		glDeleteTextures( 1, &text_buffer );
		OpenGLInterface::DeleteVertexArrays( 1, &vao );
		//OpenGLInterface::DeleteProgram( text_program );
	}

	bool OpenGLText::SetShader( OpenGLShader* shader )
	{
		if( m_Shader == nullptr )
		{
			m_Shader = shader;
			return true;
		}

		return false;
	}

	bool OpenGLText::Init( unsigned short width, unsigned int height )
	{
		if( SetSize( width, height ) )
		{
			//delete m_Shader;

			//GLuint vs = OpenGLInterface::LoadShader( L"Media/Shaders/text2d.vs.glsl", GL_VERTEX_SHADER, true );
			//GLuint fs = OpenGLInterface::LoadShader( L"Media/Shaders/text2d.fs.glsl", GL_FRAGMENT_SHADER, true );

			//text_program = OpenGLInterface::CreateProgram();
			//OpenGLInterface::AttachShader( text_program, vs );
			//OpenGLInterface::AttachShader( text_program, fs );
			//OpenGLInterface::LinkProgram( text_program );

			//OpenGLInterface::DeleteShader( fs );
			//OpenGLInterface::DeleteShader( vs );

			// glCreateVertexArrays(1, &vao);
			OpenGLInterface::GenVertexArrays( 1, &vao );
			OpenGLInterface::BindVertexArray( vao );

			// glCreateTextures(GL_TEXTURE_2D, 1, &text_buffer);
			OpenGLInterface::ActiveTexture( GL_TEXTURE0 ); // Fix from tesselation?
			glGenTextures( 1, &text_buffer );
			glBindTexture( GL_TEXTURE_2D, text_buffer );
			OpenGLInterface::TexStorage2D( GL_TEXTURE_2D, 1, GL_R8UI, m_BufferWidth, m_BufferHeight );

			glPixelStorei( GL_UNPACK_ALIGNMENT, 4 ); // Fix from tesselation?
			font_texture = KTX::load( "Media/Textures/cp437_9x16.ktx" );

			// Extract the font width and height.  Results must be 0 or larger.
			glGetTexLevelParameteriv( GL_TEXTURE_2D_ARRAY, 0, GL_TEXTURE_WIDTH, &m_FontWidth );
			glGetTexLevelParameteriv( GL_TEXTURE_2D_ARRAY, 0, GL_TEXTURE_HEIGHT, &m_FontHeight );

			return ( font_texture != 0 );
		}

		return false;
	}

	bool OpenGLText::SetSize( unsigned short width, unsigned int height )
	{
		// TODO: make this work after init also
		if( m_ScreenBuffer != nullptr )
		{
			return false;
			// delete {[] m_ScreenBuffer;
		}

		m_ScreenBuffer = new char[ width * height ];
		memset( m_ScreenBuffer, 0, width * height );

		m_BufferWidth = width;
		m_BufferHeight = height;

		return true;
	}

	void OpenGLText::Render()
	{
		if( m_Shader && m_Shader->Use() )
		{
			//OpenGLInterface::UseProgram( text_program );

			const GLfloat color[] = { 1.0f, 1.0f, 1.0f, 1.0f };

			OpenGLInterface::VertexAttrib1f( 1, -1.0f );
			//OpenGLInterface::VertexAttrib1f( 1, -0.5f );
			OpenGLInterface::VertexAttrib4fv( 2, color );

			OpenGLInterface::ActiveTexture( GL_TEXTURE0 );
			glBindTexture( GL_TEXTURE_2D, text_buffer );
			if( m_Dirty )
			{
				glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, m_BufferWidth, m_BufferHeight, GL_RED_INTEGER, GL_UNSIGNED_BYTE, m_ScreenBuffer );
				m_Dirty = false;
			}
			OpenGLInterface::ActiveTexture( GL_TEXTURE1 );
			OpenGLInterface::BindTexture( GL_TEXTURE_2D_ARRAY, font_texture );

			glFrontFace( GL_CCW );
			glPolygonMode( GL_FRONT, GL_FILL );
			glEnable( GL_DEPTH_TEST );
			glDepthFunc( GL_LEQUAL );
			glEnable( GL_CULL_FACE );

			OpenGLInterface::BindVertexArray( vao );
			glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
		}
	}

	// TODO: This is meant to be temporary
	//void Text::CopyWideCharBufferToCharBuffer( const wchar_t* source, char* target )
	//{
	//	if( source && target )
	//	{
	//		for( unsigned int index = 0; index < Platform::kMaxStringLength; ++index )
	//		{
	//			wchar_t code = source[ index ];
	//
	//			if( code == '\0' )
	//				break;
	//
	//			if( code < 128 )
	//			{
	//				*target = char( code );
	//			}
	//			else
	//			{
	//				*target = '?';
	//				if( code >= 0xD800 && code <= 0xD8FF )
	//				{
	//					// lead surrogate, skip the next code unit, which is the trail
	//					++index;
	//				}
	//			}
	//			++target;
	//		}
	//
	//		*target = '\0';
	//	}
	//}

	bool OpenGLText::DrawString( const std::wstring& str, unsigned short posX, unsigned short posY )
	{
		if( m_ScreenBuffer )
		{
			char* dst = m_ScreenBuffer + posY * m_BufferWidth + posX;

			// Convert the wide chars to regular
			using convert_type = std::codecvt_utf8<wchar_t>;
			std::wstring_convert<convert_type, wchar_t> converter;
			std::string convertedString = converter.to_bytes( str );

			strcpy( dst, convertedString.c_str() );

			return m_Dirty = true;
		}

		return false;
	}

	//void Text::scroll(int lines)
	//{
	//	const char* src = screen_buffer + lines * buffer_width;
	//	char * dst = screen_buffer;
	//
	//	memmove(dst, src, (buffer_height - lines) * buffer_width);
	//
	//	dirty = true;
	//}

	//void Text::print(const char* str)
	//{
	//	const char* p = str;
	//	char c;
	//	char* dst = screen_buffer + cursor_y * buffer_width + cursor_x;
	//
	//	while (*p != 0)
	//	{
	//		c = *p++;
	//		if (c == '\n')
	//		{
	//			cursor_y++;
	//			cursor_x = 0;
	//			if (cursor_y >= buffer_height)
	//			{
	//				cursor_y--;
	//				scroll(1);
	//			}
	//			dst = screen_buffer + cursor_y * buffer_width + cursor_x;
	//		}
	//		else
	//		{
	//			*dst++ = c;
	//			cursor_x++;
	//			if (cursor_x >= buffer_width)
	//			{
	//				cursor_y++;
	//				cursor_x = 0;
	//				if (cursor_y >= buffer_height)
	//				{
	//					cursor_y--;
	//					scroll(1);
	//				}
	//				dst = screen_buffer + cursor_y * buffer_width + cursor_x;
	//			}
	//		}
	//	}
	//
	//	dirty = true;
	//}

	//void Text::moveCursor(int x, int y)
	//{
	//	cursor_x = x;
	//	cursor_y = y;
	//}

	void OpenGLText::Clear()
	{
		if( m_ScreenBuffer )
		{
			memset( m_ScreenBuffer, 0, m_BufferWidth * m_BufferHeight );
			m_Dirty = true;
			//cursor_x = 0;
			//cursor_y = 0;
		}
	}
}