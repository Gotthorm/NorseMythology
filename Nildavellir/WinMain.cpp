// WINMAIN.CPP

#include <windows.h>
#include <TCHAR.H>
#include <assert.h>
#include <stdio.h>
#include "Platform.h"
#include "Framework.h"

// Constants
const unsigned int kMaxApplicationString = 256;

// Forward declarations
ATOM RegisterWindowClass( HINSTANCE );
HWND CreateAndInitializeWindow( HINSTANCE, LPCTSTR, int );
LRESULT CALLBACK WindowProcedure( HWND, UINT, WPARAM, LPARAM );
LPCTSTR GetApplicationTitle();
void ProcessCommandLineArguments();

// Globals
LPCTSTR g_LaunchTitle;				// A cached pointer to the application title
Platform::LaunchInfo g_LaunchInfo;	// Startup information that will be passed to the main application
Framework g_Framework;				// The instance of the application framework

// The main entry point
int APIENTRY _tWinMain( HINSTANCE instance, HINSTANCE prevInstance, LPTSTR cmdLine, int cmdShow )
{
	// Avoid any compiler warnings for an unused variable
	UNREFERENCED_PARAMETER( prevInstance );

	// Default configuration will be with one thread
	g_LaunchInfo.numberOfThreads = 1;

	// Process the command line arguments and write valid results to globals
	ProcessCommandLineArguments();

	// Build and get a pointer to our application title
	g_LaunchTitle = GetApplicationTitle();
	g_LaunchInfo.applicationTitle = std::wstring( g_LaunchTitle );

	// Register our main window
	ATOM nWindowClassID = RegisterWindowClass( instance );

	// Check that our registration was successful
	if( nWindowClassID != 0 )
	{
		// CreateWindow accepts either a class name string or a class ID
		// Convert our class ID to a 32 bit class string name pointer
		LPCTSTR pClassName = reinterpret_cast<LPCTSTR>( MAKELRESULT( nWindowClassID, 0 ) );

		// Create and initialize our main window
		HWND hWindow = CreateAndInitializeWindow( instance, pClassName, cmdShow );

		// Check that our window creation was successful
		if( hWindow != NULL )
		{
			// Initialize the game and shutdown if failure
			if( g_Framework.Init( hWindow, g_LaunchInfo ) )
			{
				MSG theMessage;
				BOOL bGotMessage;

				// Drive the main windows pump until the application shuts down
				while( true ) 
				{
					// Use PeekMessage when running so it doesn't wait.
					// Retrieve all window or thread messages for the current thread
					// and removes them from the queue.
					bGotMessage = PeekMessage( &theMessage, 0, 0, 0, PM_REMOVE );

					// If there was a windows message, process it now
					if( bGotMessage ) 
					{
						// Received equest to quit the application
						if( theMessage.message == WM_QUIT )
						{
							// Break from the main loop and shut down
							break;
						}

						// Translate virtual-key messages into character messages.
						// If the message is translated, the return value is nonzero.
						// If the message is WM_KEYDOWN, WM_KEYUP, WM_SYSKEYDOWN, or WM_SYSKEYUP, 
						// the return value is nonzero, regardless of the translation. 
						// If the message is not translated, the return value is zero.
						TranslateMessage( &theMessage );

						// Dispatch the message to a window procedure.
						// Return value is whatever the window procedure returns.
						DispatchMessage( &theMessage );
					}

					// Pump the framework
					g_Framework.Update();
				}
			}
			else
			{
				OutputDebugString( _T("Could not initialize the game framework\n") );
			}

			// Shutdown the game object
			g_Framework.Shutdown();
		}
		else
		{
			OutputDebugString( _T("Could not create window\n") );
		}

		UnregisterClass( pClassName, instance );
	}
	else
	{
		OutputDebugString( _T("Could not register window\n") );
	}

    return 0;
}

ATOM RegisterWindowClass( HINSTANCE instance )
{
	// Describe and register our window class
	// Returns a new window class ID
	// Returns zero if unsuccessful

	WNDCLASSEX WindowClassDescriptor;

	WindowClassDescriptor.cbSize = sizeof( WNDCLASSEX );

	WindowClassDescriptor.style			= CS_OWNDC /*CS_HREDRAW | CS_VREDRAW*/;
	WindowClassDescriptor.lpfnWndProc	= WindowProcedure;
	WindowClassDescriptor.cbClsExtra	= 0;
	WindowClassDescriptor.cbWndExtra	= 0;
	WindowClassDescriptor.hInstance		= instance;
	WindowClassDescriptor.hIcon			= NULL /*LoadIcon( hInstance, IDI_APPLICATION )*/;
	WindowClassDescriptor.hCursor		= NULL /*LoadCursor( NULL, IDC_ARROW )*/;
	WindowClassDescriptor.hbrBackground	= (HBRUSH)(COLOR_BACKGROUND);
	WindowClassDescriptor.lpszMenuName	= NULL;
	WindowClassDescriptor.lpszClassName	= g_LaunchTitle;
	WindowClassDescriptor.hIconSm		= NULL;

	return RegisterClassEx( &WindowClassDescriptor );
}

HWND CreateAndInitializeWindow( HINSTANCE instance, LPCTSTR className, int cmdShow )
{
	// Create our main window
	// When horizontal position is set to CW_USEDEFAULT, vertical position is ignored
	// When width is set to CW_USEDEFAULT, height is ignored
	// Returns NULL if unsuccessful
	HWND window = CreateWindow(	className,						// Window Class name or ID
								g_LaunchTitle,					// Window name
								WS_OVERLAPPEDWINDOW,			// Window styles
								CW_USEDEFAULT,					// Horizontal position
								0,								// Verical position (ignored)
								CW_USEDEFAULT,					// Width
								0,								// Height (ignored)
								GetDesktopWindow(),				// Handle to parent window
								NULL,							// Handle to menu
								instance,						// Handle to application instance
								NULL );							// Pointer to CreateWindow parameters

	// Check the creation result
	if( window != NULL )
	{
		// Show the window we just created
		// We are ignoring the return result.
		// If the window was previously visible, the return value is nonzero. 
		// If the window was previously hidden, the return value is zero. 
		ShowWindow( window, cmdShow );

		// Tell the window to update its client area by sending a WM_PAINT message
		// We are ignoring the return result.
		// If the function succeeds, the return value is nonzero.
		UpdateWindow( window );
	}

	// Return a handle to the window
	// This may be NULL if window creation was unsuccessful
    return window;
}

LRESULT CALLBACK WindowProcedure( HWND window, UINT message, WPARAM wParam, LPARAM lParam )
{
	// Process any windows messages that are of interest to us
	switch( message )
	{
	case WM_PAINT:
		//hDeviceContext = BeginPaint( hWindow, &thePaintStructure );
		//g_Framework.Render();
		ValidateRect( window, NULL );
		break;
	case WM_DESTROY:
	case WM_CLOSE:
		PostQuitMessage( 0 );
		break;
	case WM_ENTERMENULOOP:
		//g_Framework.EnableMouseCapture( false );
		break;
	case WM_EXITSIZEMOVE:
		//g_Framework.EnableMouseCapture( true );
		break;
	case WM_INPUT:
		g_Framework.ProcessInputEvent( lParam );
		break;
	//case WM_SIZING:
	case WM_SIZE:
		g_Framework.ResizeWindow( LOWORD( lParam ), HIWORD( lParam ) );
		break;
	default:
		// Pass the message to the default handlers
		return DefWindowProc( window, message, wParam, lParam );
	}

	return 0;
}

LPCTSTR GetApplicationTitle()
{
#pragma region TODO
	// Add the version control system value
#pragma endregion

	const TCHAR g_ApplicationTemplateName[] = _T("Nildavellir : %s : %s : %s");
	static TCHAR applicationName[ kMaxApplicationString ];

#ifdef _DEBUG
	const TCHAR g_BuildType[] = _T("Debug Build");
#else
	const TCHAR g_BuildType[] = _T("Release Build");
#endif

	_stprintf_s(applicationName, kMaxApplicationString, g_ApplicationTemplateName, g_BuildType, _T(__DATE__), _T(__TIME__) );

	return applicationName;
}

void ProcessCommandLineArguments()
{
	// Since we are trying to keep this code string type agnostic
	// We will extract the command line from the OS instead of using
	// the version passed to the application.  This ensures we have
	// the wide char version which is needed to use the CommandLineToArgvW function.
	int NumberOfArguments;

	LPWSTR* argumentList = CommandLineToArgvW( GetCommandLineW(), &NumberOfArguments );

	if( argumentList != NULL )
	{
		// Index 0 in the list will be the application name.
		// Search for supported arguments starting at index 1
		for( int nIndex = 1; nIndex < NumberOfArguments; nIndex++ )
		{
			// Some paranoia
			assert(argumentList[ nIndex ] );

#pragma region TODO
			// Insert other supported command line parameters here
			//
			// 1) Alternate log file name and location
			// 2) Console window to be open by default
			// 3) etc
			//
			// Test for the number of threads parameter
			//if( _tcsncicmp( argumentList[ nIndex ], _T("THREADS="), 8 ) == 0 )
			//{
			//	// Parameter name syntax is valid, now test the given value

			//	// Parameter value must be non-null and a numeric value
			//	// If the given value is valid, it is assigned to our variable
			//	// Otherwise we assign a zero which indicates an error condition
			//	g_LaunchInfo.numberOfThreads = ( _tcslen( argumentList[ nIndex ] ) > 8 ) ? _tstoi( &( argumentList[ nIndex ][ 8 ] ) ) : 0;

			//	// Test for the error condition
			//	if( g_LaunchInfo.numberOfThreads < 1 )
			//	{
			//		// Thread value is bogus, reassign to the default value
			//		g_LaunchInfo.numberOfThreads = 1;

			//		OutputDebugString( _T("Invalid thread count argument\n") );
			//	}
			//}
			//else if( ) 
			//{ 
			//}
			//else
#pragma endregion
			{
				// Command line parameter was unrecognized
				TCHAR ErrorString[ kMaxApplicationString ];

				_stprintf_s( ErrorString, kMaxApplicationString, _T("Invalid command line parameter: %s\n"), argumentList[ nIndex ] );
				
				OutputDebugString( ErrorString );
			}
		}
	}
	else
	{
	  OutputDebugString( _T("CommandLineToArgvW failed\n") );
	}

	// Free memory allocated for CommandLineToArgvW arguments.
	LocalFree( argumentList );
}

