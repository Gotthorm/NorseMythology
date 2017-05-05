// OpenGLRenderObject.CPP

#include "Muspelheim.h"
#include "OpenGLRenderer.h"
#include "OpenGLRenderObject.h"
#include "OpenGLInterface.h"
#include "OpenGLSurface.h"
#include "OpenGLShader.h"
#include <glm/gtc/type_ptr.hpp>

namespace Muspelheim
{
	OpenGLRenderer* OpenGLRenderObject::sm_Renderer = nullptr;

	//
	OpenGLRenderObject::OpenGLRenderObject( unsigned short id, std::weak_ptr<OpenGLSurface> surface ) 
		: m_ID( id )
		, m_Surface( surface )
		, m_Shader( nullptr )
	{
	}

	//
	OpenGLRenderObject::~OpenGLRenderObject()
	{
		delete m_VertexArray;
		delete m_NormalArray;
	}

	//
	bool OpenGLRenderObject::SetShader( unsigned int shaderID )
	{
		if( shaderID )
		{
			m_Shader = sm_Renderer->GetShader( shaderID );
			if( m_Shader )
			{
				m_UniformProjectionMatrix = m_Shader->GetUniformVariableLocation( L"projection_matrix" );
				m_UniformViewMatrix = m_Shader->GetUniformVariableLocation( L"view_matrix" );
				m_UniformModelMatrix = m_Shader->GetUniformVariableLocation( L"model_matrix" );

				return ( m_UniformProjectionMatrix >= 0 ) && ( m_UniformViewMatrix >= 0 ) && ( m_UniformModelMatrix >= 0 );
			}
		}

		// Log something about the given shader failed to load?

		return false;
	}

	bool OpenGLRenderObject::LoadData( unsigned int size, void* data, unsigned int objectCount )
	{
		// We want to support two different methods of rendering
		if( m_Shader )
		{
			OpenGLInterface::GenVertexArrays( 1, &m_VertexArrayObject );
			OpenGLInterface::BindVertexArray( m_VertexArrayObject );

			OpenGLInterface::GenBuffers( 1, &m_DataBuffer );
			OpenGLInterface::BindBuffer( GL_ARRAY_BUFFER, m_DataBuffer );
			OpenGLInterface::BufferData( GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW );

			OpenGLInterface::BufferSubData( GL_ARRAY_BUFFER, 0, size, data );

			OpenGLInterface::VertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(uintptr_t)0 );
			OpenGLInterface::EnableVertexAttribArray( 0 );

			// data_size * (4/7) Assuming equal entries, first block uses 4 floats, second block uses 3
			OpenGLInterface::VertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)uintptr_t( ( size * 4 ) / 7 ) );
			OpenGLInterface::EnableVertexAttribArray( 1 );

			OpenGLInterface::BindVertexArray( 0 );
			OpenGLInterface::BindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
		}
		else
		{
			m_VertexArray = new float[ size * 4 ];
			m_NormalArray = new float[ size * 3 ];

			memcpy( m_VertexArray, data, size * 4 * sizeof( float ) );
			data = (GLvoid *)(uintptr_t(data) + (size * 4 * sizeof( float )));
			memcpy( m_VertexArray, data, size * 3 * sizeof( float ) );
		}

		m_ObjectCount = objectCount;

		return true;
	}

	bool OpenGLRenderObject::LoadTexture( const unsigned char* imageData, unsigned int height, unsigned int width, bool hasAlpha )
	{
		PLATFORM_ASSERT( m_LoadedTextures.size() < 32 );

		OpenGLInterface::ActiveTexture( GL_TEXTURE0 + GLenum( m_LoadedTextures.size() ) );

		unsigned int textureID = 0;

		if( imageData )
		{
			glGenTextures( 1, &textureID );

			glBindTexture( GL_TEXTURE_2D, textureID );

			glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

			GLint format = hasAlpha ? GL_RGBA : GL_RGB;

			glTexImage2D( GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData );

			OpenGLInterface::GenerateMipmap( GL_TEXTURE_2D );

			m_LoadedTextures.push_back( textureID );

			return true;
		}

		return false;
	}

	//
	bool OpenGLRenderObject::Render()
	{
		// Check that the surface still exists
		std::shared_ptr<OpenGLSurface> surface = m_Surface.lock();
		if( surface != nullptr )
		{
			return surface->QueueRender( m_ID );
		}

		return false;
	}

	void OpenGLRenderObject::Draw( const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix )
	{
		if( m_LoadedTextures.size() > 0 )
		{
			OpenGLInterface::ActiveTexture( GL_TEXTURE0 );
			glBindTexture( GL_TEXTURE_2D, m_LoadedTextures[ 0 ] );
		}

		GLenum windingOrder = m_WindingOrderClockwise ? GL_CW : GL_CCW;
		GLenum face = m_PolyBackFace ? GL_FRONT_AND_BACK : GL_FRONT;
		GLenum mode = m_PolyLineMode ? GL_LINE : GL_FILL;

		glFrontFace( windingOrder );
		glPolygonMode( face, mode );
		glEnable( GL_DEPTH_TEST );
		glDepthFunc( GL_LEQUAL );
		glEnable( GL_CULL_FACE );

		if( m_Shader && m_Shader->Use() )
		{
			OpenGLInterface::UniformMatrix4fv( m_UniformViewMatrix, 1, GL_FALSE, glm::value_ptr( viewMatrix ) );
			OpenGLInterface::UniformMatrix4fv( m_UniformProjectionMatrix, 1, GL_FALSE, glm::value_ptr( projectionMatrix ) );
			OpenGLInterface::UniformMatrix4fv( m_UniformModelMatrix, 1, GL_FALSE, glm::value_ptr( GetMatrix() ) );

			if( m_PolyMode == PATCH )
			{
				OpenGLInterface::PatchParameteri( GL_PATCH_VERTICES, 4 );

				OpenGLInterface::DrawArraysInstanced( GL_PATCHES, 0, 4, 4096 );
			}
			else
			{
				OpenGLInterface::BindVertexArray( m_VertexArrayObject );

				glDrawArrays( GL_TRIANGLES, 0, m_ObjectCount );

				OpenGLInterface::BindVertexArray( 0 );
			}
		}
		else
		{
			float* vertData = &m_VertexArray[ 0 ];
			float* normalData = &m_NormalArray[ 0 ];
			int vertCount = m_ObjectCount;

			while( vertCount > 0 )
			{
				for( int vertIndex = 0; vertIndex < 3; ++vertIndex )
				{
					glNormal3fv( normalData );
					normalData += 3;

					glVertex3fv( vertData );
					vertData += 4;

					--vertCount;
				}
			}
		}
	}
}