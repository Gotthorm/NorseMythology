// TEXT.CPP

#include <ft2build.h>
#include FT_FREETYPE_H
#include <vector>
#include "Muspelheim.h"
#include "OpenGLText.h"
#include "KTX.h"
#include "Platform.h"
#include "OpenGLShader.h"
#include "Niflheim.h"

#pragma comment(lib, "freetype.lib")

char const defaultFontFile[] = "Media/Fonts/CourierNew.ttf";
unsigned int const fontPointSize = 16U;

namespace Muspelheim
{
	OpenGLText::~OpenGLText()
	{
		delete [] m_pScreenBuffer;
		m_pScreenBuffer = nullptr;
		glDeleteTextures( 1, &font_texture );
		glDeleteTextures( 1, &text_buffer );
		OpenGLInterface::DeleteVertexArrays( 1, &m_VertexArrayObject );
		//OpenGLInterface::DeleteProgram( text_program );

		FT_Done_Face( m_Face );
		FT_Done_FreeType( m_FreeTypeLibrary );
	}

	bool OpenGLText::SetShader( OpenGLShader* shader )
	{
		if( m_Shader == nullptr )
		{
			m_Shader = shader;

			m_FontScalarLocationId = m_Shader->GetUniformVariableLocation( L"fontScalar" );
			//m_FontScalarLocationId = OpenGLInterface::GetUniformLocation( m_Shader, "fontScalar" );

			return (0 <= m_FontScalarLocationId);
		}

		return false;
	}

	bool OpenGLText::Init( unsigned short width, unsigned int height, std::weak_ptr<Niflheim::MessageManager> const & messageManager )
	{
		std::shared_ptr<Niflheim::MessageManager> sharedMessageManager = messageManager.lock();

		// Initialize the free type library
		//if ( 0 == FT_Init_FreeType( &m_FreeTypeLibrary ) )
		{
			// Load our default TTF font
			//if ( 0 == FT_New_Face( m_FreeTypeLibrary, defaultFontFile, 0, &m_Face ) )
			{
				// Set the font size : 16 equates to a 9 x 9 pixel font
				//if ( 0 == FT_Set_Pixel_Sizes( m_Face, 0, fontPointSize ) )
				{
					//if ( 0 == FT_Load_Char( m_Face, 'X', FT_LOAD_RENDER ) )
					{
						//m_FontWidth = m_Face->glyph->bitmap.width;
						//m_FontHeight = m_Face->glyph->bitmap.rows;

						GLuint tex;
						OpenGLInterface::ActiveTexture( GL_TEXTURE0 );
						glGenTextures( 1, &tex );
						glBindTexture( GL_TEXTURE_2D, tex );
						OpenGLInterface::Uniform1i( m_UniformTex, 0 );
					}

					if ( SetBufferSize( width, height ) )
					{
						OpenGLInterface::GenVertexArrays( 1, &m_VertexArrayObject );
						OpenGLInterface::BindVertexArray( m_VertexArrayObject );

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
				}
				//else
				//{
				//	if ( sharedMessageManager )
				//	{
				//		sharedMessageManager->Post( Niflheim::Message::LOG_ERROR, std::wstring( L"Failed to set font point size to " + std::to_wstring( fontPointSize ) ) );
				//	}
				//}
			}
			//else
			//{
			//	if ( sharedMessageManager )
			//	{
			//		sharedMessageManager->Post( Niflheim::Message::LOG_ERROR, std::wstring( L"Could not open default font" ) );
			//	}
			//}
		}
		//else
		//{
		//	if ( sharedMessageManager )
		//	{
		//		sharedMessageManager->Post( Niflheim::Message::LOG_ERROR, std::wstring( L"Could not init freetype library" ) );
		//	}
		//}

		return false;
	}

	bool OpenGLText::SetBufferSize( unsigned short width, unsigned int height )
	{
		// TODO: make this work after init also
		if( nullptr != m_pScreenBuffer )
		{
			return false;
			// delete {[] m_ScreenBuffer;
		}

		if ( 0 < width && 0 < height )
		{
			m_BufferSize = width * height;
			m_BufferWidth = width;
			m_BufferHeight = height;

			m_pScreenBuffer = new char[ m_BufferSize ];
			PLATFORM_ASSERT( m_pScreenBuffer );
			Clear();

			return true;
		}

		return false;
	}

	void OpenGLText::Render()
	{
		if( m_Shader && m_Shader->Use() )
		{
			const GLfloat color[] = { 1.0f, 1.0f, 1.0f, 1.0f };

			OpenGLInterface::VertexAttrib4fv( 1, color );

			OpenGLInterface::Uniform2fv( m_FontScalarLocationId, 1, m_FontScale );

			OpenGLInterface::ActiveTexture( GL_TEXTURE0 );
			glBindTexture( GL_TEXTURE_2D, text_buffer );
			if( m_Dirty )
			{
				glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, m_BufferWidth, m_BufferHeight, GL_RED_INTEGER, GL_UNSIGNED_BYTE, m_pScreenBuffer );
				m_Dirty = false;
			}

			OpenGLInterface::ActiveTexture( GL_TEXTURE1 );
			OpenGLInterface::BindTexture( GL_TEXTURE_2D_ARRAY, font_texture );

			glFrontFace( GL_CCW );
			glPolygonMode( GL_FRONT, GL_FILL );
			glEnable( GL_DEPTH_TEST );
			glDepthFunc( GL_LEQUAL );
			glEnable( GL_CULL_FACE );

			OpenGLInterface::BindVertexArray( m_VertexArrayObject );
			glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
		}
	}

	bool OpenGLText::DrawString( const std::wstring& str, unsigned int color, unsigned short posX, unsigned short posY )
	{
		if( nullptr != m_pScreenBuffer )
		{
			char* pTarget = m_pScreenBuffer + posY * m_BufferWidth + posX;

			CopyAndConvert( pTarget, str.c_str(), static_cast<unsigned int>(str.length()) );

			return m_Dirty = true;
		}

		return false;
	}

	bool OpenGLText::DrawStringBuffer( TextBuffer const & textBuffer )
	{
		unsigned int const size = static_cast<unsigned int>(textBuffer.Text.size());

		if ( nullptr != m_pScreenBuffer && 0 < size )
		{
			unsigned int const copySize = ( size < m_BufferSize ) ? size : m_BufferSize;

			CopyAndConvert( m_pScreenBuffer, &(textBuffer.Text[0]), copySize );

			return true;
		}

		return false;
	}

	void OpenGLText::CopyAndConvert( char * pTarget, wchar_t const * pSource, unsigned int size )
	{
		for ( unsigned int index = 0; index < size; ++index )
		{
			wchar_t const code = pSource[ index ];

			//if( code == '\0' )
			//	break;

			if ( code < 128 )
			{
				*pTarget = char( code );
			}
			else
			{
				*pTarget = '?';
				if ( code >= 0xD800 && code <= 0xD8FF )
				{
					// lead surrogate, skip the next code unit, which is the trail
					++index;
				}
			}
			++pTarget;
		}
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
		if( m_pScreenBuffer )
		{
			memset( m_pScreenBuffer, 0, m_BufferSize );
			m_Dirty = true;
			//cursor_x = 0;
			//cursor_y = 0;
		}
	}

	void OpenGLText::SetScale( float scaleX, float scaleY )
	{
		m_FontScale[ 0 ] = scaleX;
		m_FontScale[ 1 ] = scaleY;
	}
}