// OpenGLSurface.CPP

#include <glm/gtc/type_ptr.hpp>
#include "OpenGLRenderer.h"
#include "OpenGLSurface.h"
#include "OpenGLInterface.h"
#include "OpenGLRenderObject.h"
#include "OpenGLLightObject.h"
#include "OpenGLShader.h"

namespace Muspelheim
{
	//
	bool OpenGLSurface::Init( std::weak_ptr<OpenGLSurface> weakSelf, unsigned short width, unsigned short height, std::weak_ptr<Niflheim::MessageManager> const & messageManager )
	{
		PLATFORM_ASSERT( width > 0 && height > 0 );

		if( m_Width == 0 && m_Height == 0 )
		{
			if( SetSize( width, height ) )
			{
				if( m_Text.Init( width, height, messageManager ) )
				{
					m_WeakSelf = weakSelf;

					return true;
				}
			}
		}

		return false;
	}

	//
	void OpenGLSurface::SetShader( OpenGLShader* shader )
	{
		PLATFORM_ASSERT( shader );

		m_Shader = shader;
	}	
	
	//
	void OpenGLSurface::SetTextShader( OpenGLShader* shader )
	{
		PLATFORM_ASSERT( shader );

		m_Text.SetShader( shader );
	}

	void OpenGLSurface::SetColor( const glm::vec4& color )
	{
		m_Color = color;
	}

	//
	void OpenGLSurface::SetDepth( unsigned char depth )
	{
		m_Depth = depth;

		// TODO: Notify the renderer to sort the surfaces
	}

	// This gets called from each render object
	bool OpenGLSurface::QueueRender( unsigned short renderObjectID )
	{
		// Since each render object contains a reference to its parent surface
		// we assume any caller is a child of this surface.

		if( renderObjectID < m_RenderObjects.size() )
		{
			// Do not allow render objects that add themselves twice per render frame?
			m_RenderQueue.push_back( m_RenderObjects[ renderObjectID ] );

			return true;
		}

		return false;
	}

	//
	void OpenGLSurface::RenderAll( const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix )
	{
		// Only render this surface if it is supposed to be visible
		if( m_Visible )
		{
			// Test current alpha value
			if ( 0.0f < m_Color[3] )
			{
				bool const transparent = ( 1.0f > m_Color[ 3 ] );

				glDisable( GL_DEPTH_TEST );

				if( transparent )
				{
					glEnable( GL_BLEND );
					glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
				}

				// Render background
				if( m_Shader && m_Shader->Use() )
				{
					OpenGLInterface::VertexAttrib4fv( 1, glm::value_ptr( m_Color ) );
					OpenGLInterface::VertexAttrib4fv( 2, glm::value_ptr( m_ClipSize ) );

					glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
				}

				if( transparent )
				{
					glDisable( GL_BLEND );
				}

				glEnable( GL_DEPTH_TEST );
			}

			// Turn on any lights if any
			glEnable( GL_LIGHT0 );

			// Render all objects
			for ( std::shared_ptr<OpenGLRenderObject> renderObject : m_RenderQueue )
			{
				renderObject.get()->Draw( viewMatrix, projectionMatrix );

				// Do I need to call reset on each entry?
				renderObject.reset();
			}

			// Clear the render queue for next frame
			m_RenderQueue.clear();

			// Render text overlays
			m_Text.Render();

			m_Text.Clear();
		}
	}

	//
	bool OpenGLSurface::SetSize( unsigned short width, unsigned short height )
	{
		m_Width = width;
		m_Height = height;

		// TODO: Update any children like text

		return true;
	}

	float CenterJustify( float value )
	{
		if ( 0.0f > value )
		{
			value = 0.0f;
		}
		else if ( 1.0f < value )
		{
			value = 1.0f;
		}

		return (value - 0.5f) * 2.0f;
	}

	void OpenGLSurface::SetClipping( float leftX, float topY, float rightX, float bottomY )
	{
		// Convert values from left/top justified to center justified
		m_ClipSize[ 0 ] = CenterJustify( leftX );
		m_ClipSize[ 1 ] = CenterJustify( 1 - bottomY );
		m_ClipSize[ 2 ] = CenterJustify( rightX );
		m_ClipSize[ 3 ] = CenterJustify( 1 - topY );
	}

	bool OpenGLSurface::DrawString( const std::wstring& textString, unsigned int color, unsigned short posX, unsigned short posY, Renderer::TextAlignment alignment )
	{
		// TODO: Refactor this to handle invalid data and also the case of TEXT_MIDDLE

		PLATFORM_ASSERT( alignment == Renderer::TEXT_LEFT || alignment == Renderer::TEXT_RIGHT );

		unsigned short alignedX = posX;

		if( alignment != Renderer::TEXT_LEFT )
		{
			size_t stringLength = textString.size();

			if( alignment == Renderer::TEXT_RIGHT )
			{
				alignedX = ( m_Width / m_Text.GetFontWidth() ) - posX;
			}
		}
		return m_Text.DrawString( textString, color, alignedX, posY );
	}

	bool OpenGLSurface::DrawStringBuffer( TextBuffer const & textBuffer )
	{
		return m_Text.DrawStringBuffer( textBuffer );
	}

	std::shared_ptr<RenderObject> OpenGLSurface::CreateRenderObject()
	{
		PLATFORM_ASSERT( m_RenderObjects.size() < std::numeric_limits<unsigned short>::max() );
		unsigned short id = static_cast<unsigned short>(m_RenderObjects.size());

		std::shared_ptr<OpenGLRenderObject> newRenderObject = std::make_shared<OpenGLRenderObject>( id, m_WeakSelf );

		if( newRenderObject )
		{
			m_RenderObjects.push_back( newRenderObject );
		}

		return newRenderObject;
	}

	//
	void OpenGLSurface::AddLightObject(std::weak_ptr<OpenGLLightObject> lightObject)
	{

	}

	std::shared_ptr<LightObject> OpenGLSurface::CreateLightObject()
	{
		// Ask the renderer for a new light, which may fail is we have reached the maximum
		//std::shared_ptr<OpenGLLightObject> newLightObject = std::make_shared<OpenGLLightObject>( m_WeakSelf );

		return nullptr;
	}

	void OpenGLSurface::SetTextScale( float widthScale, float heightScale )
	{
		m_Text.SetScale( widthScale, heightScale );
	}
}