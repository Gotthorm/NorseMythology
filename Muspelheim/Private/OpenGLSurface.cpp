// OpenGLSurface.CPP

#include "Muspelheim.h"
#include "OpenGLRenderer.h"
#include "OpenGLSurface.h"
#include "OpenGLInterface.h"
#include "OpenGLRenderObject.h"
#include "OpenGLLightObject.h"
#include "OpenGLShader.h"
#include <glm/gtc/type_ptr.hpp>

namespace Muspelheim
{
	//
	bool OpenGLSurface::Init( std::weak_ptr<OpenGLSurface> weakSelf, unsigned short width, unsigned short height )
	{
		PLATFORM_ASSERT( width > 0 && height > 0 );

		if( m_Width == 0 && m_Height == 0 )
		{
			if( SetSize( width, height ) )
			{
				if( m_Text.Init( width, height ) )
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
		if( m_RenderBackground )
		{
			glDisable( GL_DEPTH_TEST );

			// Render background
			if( m_Shader && m_Shader->Use() )
			{
				OpenGLInterface::VertexAttrib4fv( 1, glm::value_ptr( m_Color ) );

				glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
			}

			glEnable( GL_DEPTH_TEST );
		}

		// Turn on any lights if any std::weak_ptr<LightObject>
		for( auto lightPointer : m_LightObjects )
		{
			std::shared_ptr<OpenGLLightObject> lightObject = lightPointer.lock();

			if( lightObject )
			{
				lightObject->Apply();
			}
		}
		

		// Render all objects
		for( std::shared_ptr<OpenGLRenderObject> renderObject : m_RenderQueue )
		{
			renderObject.get()->Draw( viewMatrix, projectionMatrix );

			// Do I need to call reset on each entry?
			renderObject.reset();
		}

		// Clear the render queue for next frame
		m_RenderQueue.clear();

		// Render text overlays

		m_Text.Render();
	}

	//
	bool OpenGLSurface::SetSize( unsigned short width, unsigned short height )
	{
		m_Width = width;
		m_Height = height;

		// TODO: Update any children like text

		return true;
	}

	bool OpenGLSurface::DrawString( const std::wstring& textString, unsigned short posX, unsigned short posY, Renderer::TextAlignment alignment )
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
		return m_Text.DrawString( textString, alignedX, posY );
	}

	std::shared_ptr<RenderObject> OpenGLSurface::CreateRenderObject()
	{
		PLATFORM_ASSERT( m_RenderObjects.size() < USHRT_MAX );
		unsigned short id = static_cast<unsigned short>(m_RenderObjects.size());

		std::shared_ptr<OpenGLRenderObject> newRenderObject = std::make_shared<OpenGLRenderObject>( id, m_WeakSelf );

		if( newRenderObject )
		{
			m_RenderObjects.push_back( newRenderObject );
		}

		return newRenderObject;
	}

	//
	void OpenGLSurface::AddLightObject( std::weak_ptr<OpenGLLightObject> lightObject )
	{
		m_LightObjects.push_back( lightObject );
	}

}