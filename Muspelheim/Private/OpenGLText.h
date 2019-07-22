#pragma once

#include <string>
#include "OpenGLInterface.h"

struct FT_LibraryRec_;
struct FT_FaceRec_;

namespace Muspelheim
{
	class OpenGLShader;
	struct TextBuffer;

	class OpenGLText
	{
	public:
		OpenGLText() = default;

		~OpenGLText();

		// Called by the parent surface
		bool Init( unsigned short width, unsigned int height, std::weak_ptr<Niflheim::MessageManager> const & messageManager );

		bool SetShader( OpenGLShader * pShader );

		// Called by the parent surface
		void Render();

		//bool LoadFont( const std::wstring& font ) override;

		bool DrawString( std::wstring const & str, unsigned int color, unsigned short posX, unsigned short posY );

		bool DrawStringBuffer( TextBuffer const & textBuffer );

		//bool MoveCursor( unsigned short posX, unsigned short posY ) override;

		void Clear();

		GLuint GetFontWidth() { return static_cast<GLuint>( m_FontWidth ); }
		GLuint GetFontHeight() { return static_cast<GLuint>( m_FontHeight ); }

		void SetScale( float scaleX, float scaleY );

	private:
		bool SetBufferSize( unsigned short width, unsigned int height );

		void CopyAndConvert( char * pTarget, wchar_t const * pSource, unsigned int size );

		GLuint      text_buffer;
		GLuint      font_texture;
		GLuint      m_VertexArrayObject;

		GLint		m_FontWidth = 0;
		GLint		m_FontHeight = 0;

		GLint		m_FontScalarLocationId = -1;

		GLfloat		m_FontScale[ 2 ] = { 1.0f, 1.0f };

		//GLuint      text_program = 0;
		OpenGLShader* m_Shader = nullptr;
		//char *      screen_buffer = nullptr;;
		//int         cursor_x = 0;
		//int         cursor_y = 0;

		bool m_Dirty = true;

		char* m_pScreenBuffer = nullptr;
		unsigned short m_BufferWidth = 0;
		unsigned short m_BufferHeight = 0;
		unsigned int m_BufferSize = 0;

		FT_LibraryRec_* m_FreeTypeLibrary = nullptr;
		FT_FaceRec_ * m_Face = nullptr;

		GLint m_UniformTex = -1;;
	};
}