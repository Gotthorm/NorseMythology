// FRAMEWORK.CPP

#include "Framework.h"
#include "Graphics.h"
#include "Game.h"
#include "Input.h"
#include "Logger.h"
#include <sstream>
#include <cwchar>
#include "MessageManager.h"

bool Framework::Init( Platform::WindowHandle hWindow, const Platform::LaunchInfo& launchInfo )
{
	if( m_Initialized == false )
	{
		// Initialize the log system
		m_pLogger = new Logger();
		PLATFORM_ASSERT( m_pLogger != nullptr );
		if( m_pLogger == nullptr || m_pLogger->Initialize( L"LogFile.txt" ) == false )
		{
			PLATFORM_DEBUG_MESSAGE( L"Cannot initialize the logger system\n" );
			return false;
		}

		MessageManager::GetInstance()->Post( Message::LOG_INFO, launchInfo.applicationTitle );

		// Initialize the graphics system
		m_pGraphics = new Graphics();
		PLATFORM_ASSERT( m_pGraphics != nullptr );
		if( m_pGraphics == nullptr || m_pGraphics->InitializeRenderingContext( hWindow ) == false )
		{
			MessageManager::GetInstance()->Post( Message::LOG_ERROR, std::wstring( L"Failed to initialize the graphics rendering context" ) );
			return false;
		}

		MessageManager::GetInstance()->Post( Message::LOG_INFO, m_pGraphics->GetVersionInformation() );

		if( m_pGraphics->InitializeSubSystems() == false )
		{
			MessageManager::GetInstance()->Post( Message::LOG_ERROR, std::wstring( L"Failed to initialize the graphics subsystems" ) );
			return false;
		}

		// Initialize the input system
		m_pInput = new Input();
		PLATFORM_ASSERT( m_pInput != nullptr );
		if( m_pInput == nullptr || m_pInput->Init() == false )
		{
			MessageManager::GetInstance()->Post( Message::LOG_ERROR, std::wstring( L"Cannot initialize the input system" ) );
			return false;
		}

		// Initialize the game
		m_pGame = new Game();
		PLATFORM_ASSERT( m_pGame != nullptr );
		if( m_pGame == nullptr || m_pGame->Init() == false )
		{
			MessageManager::GetInstance()->Post( Message::LOG_ERROR, std::wstring( L"Cannot initialize the game system" ) );
			return false;
		}

		//m_pGame->EnableMouseCapture( false );

		// Initialize the stats related variables
		m_OldFrameTime = timeGetTime();
		m_OneSecondIntervalAccumulator = 0;
		m_UpdateAccumulator = 0;
		m_CurrentFPS = 0;
	}

	// Set the initialized flag as true and return it as the results
    return m_Initialized = true;
}

void Framework::Shutdown()
{
	if( m_pGraphics != nullptr )
	{
		m_pGraphics->Shutdown();
		delete m_pGraphics;
		m_pGraphics = nullptr;
	}
	if( m_pInput != nullptr )
	{
		m_pInput->Shutdown();
		delete m_pInput;
		m_pInput = nullptr;
	}
	if( m_pGame != nullptr )
	{
		m_pGame->Shutdown();
		delete m_pGame;
		m_pGame = nullptr;
	}
	// Try to always shutdown the logger last if possible
	if( m_pLogger != nullptr )
	{
		m_pLogger->Shutdown();
		delete m_pLogger;
		m_pLogger = nullptr;
	}
}

void Framework::Update()
{
	if( m_Initialized )
	{
		// Get the current time in milliseconds since the computer was turned on
		unsigned int newFrameTime = Platform::GetTime();

		// Calculate the amount of milliseconds since the last update
		unsigned int timeElapsed = newFrameTime - m_OldFrameTime;

		// If someone's computer has been running for 49 days, the counter may wrap over
		if( newFrameTime < m_OldFrameTime )
		{
			timeElapsed = newFrameTime + ( UINT_MAX - m_OldFrameTime );
		}

		// Calculate the current FPS value
		{
			// Update the 1 second accumulator
			m_OneSecondIntervalAccumulator += timeElapsed;

			// Increment the update accumulator;
			++m_UpdateAccumulator;

			const unsigned int kOneSecond = 1000;

			if( m_OneSecondIntervalAccumulator >= kOneSecond )
			{
				unsigned int secondElapsed = 0;

				while( m_OneSecondIntervalAccumulator >= kOneSecond )
				{
					++secondElapsed;
					m_OneSecondIntervalAccumulator -= kOneSecond;
				}

				m_CurrentFPS = m_UpdateAccumulator / secondElapsed;

				m_UpdateAccumulator = 0;

#ifdef _DEBUG
				// TEMP: For debugging
				static int counter = 0;
				std::wstringstream s;
				s << L"One frame has elapsed : " << counter++ << "#";
				size_t length = s.str().length();
				for( size_t index = length; length < 150; ++length )
				{
					s << length % 10;
				}
				MessageManager::GetInstance()->Post( Message::LOG_INFO, s.str() );
#endif
			}
		}

		// Update the old time for the next update
		m_OldFrameTime = newFrameTime;

		// Check for the console activation/deactivation
		PLATFORM_ASSERT( m_pInput != nullptr );
		if( m_pInput->GetKeyUp( Input::KEY_TILDA ) )
		{
			// Toggle the console
			PLATFORM_ASSERT( m_pGraphics  != nullptr );
			m_pGraphics->ToggleConsole();
		}

		// Update the game
		PLATFORM_ASSERT( m_pGame != nullptr );
		m_pGame->Update();

		// Render the current FPS
		wchar_t stringBuffer[ Platform::kMaxStringLength ];
		std::swprintf( stringBuffer, Platform::kMaxStringLength, L"%4u FPS", m_CurrentFPS );
		PLATFORM_ASSERT( m_pGraphics != nullptr );
		m_pGraphics->DisplayText( stringBuffer, 110, 0 );

		// Update which is the current camera
		m_pGraphics->SetCamera( m_pGame->GetCurrentCamera() );

		// Render the scene
		m_pGraphics->Render( timeElapsed );

		// Clear input key releases
		m_pInput->AdvanceFrame();

		// Pump the message manager to broadcast all cached messages
		MessageManager::GetInstance()->Update();
	}
}

void Framework::ProcessInputEvent( Platform::LongParam lParam )
{
	if( m_Initialized )
	{
		PLATFORM_ASSERT( m_pInput != nullptr );
		// TODO: Convert the data to something platform agnostic before sending to the input manager?
		m_pInput->ProcessEvent( lParam );
	}
}

void Framework::ResizeWindow( unsigned short width, unsigned short height )
{
	if( m_Initialized )
	{
		PLATFORM_ASSERT( m_pGraphics != nullptr );
		m_pGraphics->SetWindowSize( width, height );
	}
}