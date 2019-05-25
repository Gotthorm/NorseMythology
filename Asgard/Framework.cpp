// FRAMEWORK.CPP

#include <sstream>
#include <cwchar>
#include <functional>
#include <limits>
#include <glm/gtc/matrix_transform.hpp>
#include "Framework.h"
#include "Platform.h"
#include "Muspelheim.h"

// https://stackoverflow.com/questions/41781135/sampling-integers-in-opengl-shader-doesnt-work-result-is-always-0
// http://antongerdelan.net/opengl/vertexbuffers.html

//glTexImage1D( GL_TEXTURE_1D, 0, GL_R8, ( temp ).size(), 0, GL_RED, GL_UNSIGNED_BYTE, &( temp )[ 0 ] );
// glTexImage2D(GL_TEXTURE_2D, 0, GL_R16I, 512, 512, 0, GL_RED_INTEGER, GL_SHORT, data);
// glTexImage1D(GL_TEXTURE_1D, 0, GL_R32I, myVec.size(), 0, GL_RED_INTEGER, GL_INT, &myVec[ 0 ]);
// glTexImage1D(GL_TEXTURE_1D, 0, GL_RG16UI, 128, 0, GL_RG_INTEGER, GL_UNSIGNED_SHORT, data);

#pragma comment(lib, "Muspelheim.lib")

std::wstring const surfaceShaderName( L"Media/Shaders/surface.background" );
std::wstring const text2DShaderName( L"Media/Shaders/text2d" );

Framework::Framework()
	: m_WindowHandle( 0 )
	, m_MainScreenID( Muspelheim::InvalidSurface )
{
}

bool Framework::Init( Platform::WindowHandle hWindow, const Platform::LaunchInfo& launchInfo )
{
	if( false == m_Initialized )
	{
		// Initialize the renderer
		m_Renderer = Muspelheim::Renderer::Create();
		PLATFORM_ASSERT( nullptr != m_Renderer );
		if( nullptr == m_Renderer || false == m_Renderer->Initialize( hWindow ) )
		{
			return false;
		}
		m_Renderer->SetVSyncEnabled( false );

		// Set up the main screen
		if( false == m_Renderer->CreateSurface( m_MainScreenID ) )
		{
			return false;
		}
		else
		{
			unsigned int shaderId = m_Renderer->LoadShader( surfaceShaderName );

			if ( 0 == shaderId || false == m_Renderer->SetSurfaceShader( m_MainScreenID, shaderId ) )
			{
			}

			shaderId = m_Renderer->LoadShader( text2DShaderName );
			
			if ( 0 == shaderId || false == m_Renderer->SetSurfaceTextShader( m_MainScreenID, shaderId ) )
			{
			}
		}

		m_WindowHandle = hWindow;
	}

	// Set the initialized flag as true and return it as the results
    return m_Initialized = true;
}

void Framework::Shutdown()
{
	m_Renderer.reset();
}

void Framework::Update()
{
	if( m_Initialized )
	{
		m_Renderer->BeginRender( glm::mat4() );

		wchar_t stringBuffer[ Platform::kMaxStringLength ];

		std::swprintf( stringBuffer, Platform::kMaxStringLength, L"My mother and your mother were hanging up clothes." );
		m_Renderer->DrawSurfaceString( m_MainScreenID, stringBuffer, 1, 1, Muspelheim::Renderer::TEXT_LEFT );

		m_Renderer->EndRender();
	}
}

void Framework::ResizeWindow( unsigned short width, unsigned short height )
{
	if( m_Initialized )
	{
		PLATFORM_ASSERT( nullptr != m_Renderer );
		m_Renderer->SetWindowSize( width, height );
	}
}