// FRAMEWORK.CPP

#include <sstream>
#include <cwchar>
#include <functional>
#include <limits>
#include <glm\glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
#include "Framework.h"
#include "Platform.h"
#include "Muspelheim.h"

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
				return false;
			}

			// Dark green : Handy color calculator : https://www.tydac.ch/color/
			m_Renderer->SetSurfaceColor( m_MainScreenID, glm::vec4( 0.0f, 0.45f, 0.05f, 1.0f ) );

			shaderId = m_Renderer->LoadShader( text2DShaderName );
			
			if ( 0 == shaderId || false == m_Renderer->SetSurfaceTextShader( m_MainScreenID, shaderId ) )
			{
				return false;
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

void Framework::ResizeWindow( unsigned short width, unsigned short height )
{
	if( m_Initialized )
	{
		PLATFORM_ASSERT( nullptr != m_Renderer );
		m_Renderer->SetWindowSize( width, height );
	}
}

// Research
void Framework::Update()
{
	if ( m_Initialized )
	{
		m_Renderer->BeginRender( glm::mat4() );

		wchar_t stringBuffer[ Platform::kMaxStringLength ];

		unsigned int const color(0xFFFFFFFF);

		std::swprintf( stringBuffer, Platform::kMaxStringLength, L"My mother and your mother were hanging up clothes." );
		m_Renderer->DrawSurfaceString( m_MainScreenID, stringBuffer, color, 1, 1, Muspelheim::Renderer::TEXT_LEFT );

		m_Renderer->EndRender();
	}
}