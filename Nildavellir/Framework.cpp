// FRAMEWORK.CPP

#include "Framework.h"
#include <assert.h>
#include <stdio.h>
#include <TCHAR.H>
#include <sstream>
#include "MessageManager.h"

// Used for the call to timeGetTime
#pragma comment(lib, "winmm.lib")

const DWORD kOneSecond = 1000;
const DWORD kMaxStringLength = 256;

Framework::Framework() : m_OldFrameTime(0), m_OneSecondIntervalAccumulator(0), m_UpdateAccumulator(0), m_CurrentFPS(0), m_Initialized(false)
{
}

bool Framework::Init( HINSTANCE instance, HWND window, const LaunchInfo& launchInfo )
{
	// Initialize the log system
	if( m_Logger.Initialize( L"LogFile.txt" ) == false )
	{
		OutputDebugString( _T("Cannot initialize the logger system\n") );
		return false;
	}

	std::wstring launchInfoString( launchInfo.applicationTitle );

	MessageManager::GetInstance()->Post( Message::LOG_INFO, launchInfoString );

    if( m_Graphics.InitializeRenderingContext( window ) == false )
	{
		MessageManager::GetInstance()->Post( Message::LOG_ERROR, std::wstring(L"Failed to initialize the graphics rendering context") );
        return false;
	}

	if( m_Graphics.InitializeSubSystems() == false )
	{
		MessageManager::GetInstance()->Post( Message::LOG_ERROR, std::wstring( L"Failed to initialize the graphics subsystems") );
		return false;
	}

	if( m_Input.Init() == false )
	{
		MessageManager::GetInstance()->Post( Message::LOG_ERROR, std::wstring( L"Cannot initialize the input system") );
		return false;
	}

    if( m_Game.Init( instance, window ) == false )
	{
		MessageManager::GetInstance()->Post( Message::LOG_ERROR, std::wstring( L"Cannot initialize the game system") );
        return false;
	}

	m_Game.EnableMouseCapture( false );

	// Initialize the stats related variables
	m_OldFrameTime = timeGetTime();
	m_OneSecondIntervalAccumulator = 0;
	m_UpdateAccumulator = 0;
	m_CurrentFPS = 0;

    return m_Initialized = true;
}

void Framework::Shutdown()
{
	m_Graphics.Shutdown();
	m_Input.Shutdown();
	m_Game.Shutdown();

	m_Logger.Shutdown();
}

void Framework::Update()
{
	if( m_Initialized )
	{
		// Get the current time in milliseconds since the computer was turned on
		DWORD newFrameTime = timeGetTime();

		// Calculate the amount of milliseconds since the last update
		DWORD timeElapsed = newFrameTime - m_OldFrameTime;

		// If someone's computer has been running for 49 days, the counter may wrap over
		if( newFrameTime < m_OldFrameTime )
		{
			timeElapsed = newFrameTime + ( MAXDWORD - m_OldFrameTime );
		}

		// Update the 1 second accumulator
		m_OneSecondIntervalAccumulator += timeElapsed;

		// Increment the update accumulator;
		++m_UpdateAccumulator;

		if( m_OneSecondIntervalAccumulator >= kOneSecond )
		{
			DWORD secondElapsed = 0;

			while( m_OneSecondIntervalAccumulator >= kOneSecond )
			{
				++secondElapsed;
				m_OneSecondIntervalAccumulator -= kOneSecond;
			}

			m_CurrentFPS = m_UpdateAccumulator / secondElapsed;

			m_UpdateAccumulator = 0;

			// For debugging
			static int counter = 0;
			std::wstringstream s;
			s << L"One frame has elapsed : " << counter++;
			MessageManager::GetInstance()->Post( Message::LOG_INFO, s.str() );
		}

		// Update the old time for the next update
		m_OldFrameTime = newFrameTime;

		// Check for the console activation/deactivation
		if( m_Input.GetKeyUp( Input::KEY_TILDA ) && m_Input.GetKeyDown( Input::KEY_SHIFT ) )
		{
			// Toggle the console
			m_Graphics.ToggleConsole();
		}

		m_Game.Update();

		char stringBuffer[ kMaxStringLength ];

		sprintf_s( stringBuffer, kMaxStringLength, "%4u FPS", m_CurrentFPS );

		m_Graphics.DisplayText( stringBuffer, 110, 0 );

		m_Graphics.SetCamera( m_Game.GetCurrentCamera() );

		m_Graphics.Render( timeElapsed );

		//	ValidateRect( hWindow, NULL );

		// Clear input key releases
		// TODO : Can I get rid of this call?
		m_Input.AdvanceFrame();

		MessageManager::GetInstance()->Update();
	}
}

void Framework::UpdateInput(LPARAM lParam)
{
	if( m_Initialized )
	{
		m_Input.Update( lParam );
	}
}

//void Framework::Render()
//{
//
//}

void Framework::EnableMouseCapture( bool bEnable ) 
{ 
	if( m_Initialized )
	{
		m_Game.EnableMouseCapture( bEnable );
	}
}

void Framework::ResizeWindow(int width, int height)
{
	if( m_Initialized )
	{
		m_Graphics.SetWindowSize( width, height );
	}
}