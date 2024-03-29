// ASGARD.CPP
//
// Test harness for Mythology
//

#include <windows.h>
#include <TCHAR.H>
#include <assert.h>
#include "Framework.h"

// Constants
const UINT kMaxApplicationString = 256;

// Forward declarations
ATOM RegisterWindowClass( HINSTANCE, LPCTSTR );
HWND CreateAndInitializeWindow( HINSTANCE, LPCTSTR, int, LPCTSTR );
LRESULT CALLBACK WindowProcedure( HWND, UINT, WPARAM, LPARAM );
UINT GetApplicationTitle( LPTSTR, UINT );

BOOL ProcessMessage( MSG const & message )
{
	// Received request to quit the application
	if ( WM_QUIT == message.message )
	{
		// Indicate it is time to shutdown
		return FALSE;
	}

	// Translate virtual-key messages into character messages.
	// If the message is translated, the return value is nonzero.
	// If the message is WM_KEYDOWN, WM_KEYUP, WM_SYSKEYDOWN, or WM_SYSKEYUP, 
	// the return value is nonzero, regardless of the translation. 
	// If the message is not translated, the return value is zero.
	TranslateMessage( &message );

	// Dispatch the message to a window procedure.
	// Return value is whatever the window procedure returns.
	DispatchMessage( &message );

	return TRUE;
}

// The main entry point
int APIENTRY _tWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	// Avoid any compiler warnings for an unused variable
	UNREFERENCED_PARAMETER( hPrevInstance );

	TCHAR applicationTitle[ kMaxApplicationString ];

	UINT usedLength = GetApplicationTitle( applicationTitle, kMaxApplicationString );
	assert( usedLength < kMaxApplicationString );

	// Default launch info
	Platform::LaunchInfo launchInfo;

	// TODO : Add support for the command line to overwrite the default values for the launch info

	// Register our main window
	ATOM nWindowClassID = RegisterWindowClass( hInstance, applicationTitle );

	// Check that our registration was successful
	if ( nWindowClassID != 0 )
	{
		// CreateWindow accepts either a class name string or a class ID
		// Convert our class ID to a 32 bit class string name pointer
		LPCTSTR pClassName = reinterpret_cast<LPCTSTR>( MAKELRESULT( nWindowClassID, 0 ) );

		// Create and initialize our main window
		HWND hWindow = CreateAndInitializeWindow( hInstance, pClassName, nCmdShow, applicationTitle );

		// Check that our window creation was successful
		if ( hWindow != NULL )
		{
			RECT rect;
			if ( GetClientRect( hWindow, &rect ) )
			{
				launchInfo.width = rect.right - rect.left;
				launchInfo.height = rect.bottom - rect.top;
			}

			Framework framework;

			// Initialize the game and shutdown if failure
			if ( framework.Init( hWindow, launchInfo ) )
			{
				MSG theMessage;
				BOOL runLoop = TRUE;

				// Drive the main windows pump until the application shuts down
				while ( runLoop )
				{
					// Use PeekMessage when running so it doesn't wait.
					// Retrieve all window or thread messages for the current thread
					// and removes them from the queue.

					// Since we will be reading a buffer of raw input in the framework
					// we do not remove WM_INPUT messages from the queue

					//Process and remove all messages before WM_INPUT
					while ( runLoop && PeekMessage( &theMessage, NULL, 0, WM_INPUT - 1, PM_REMOVE ) )
					{
						runLoop = ProcessMessage( theMessage );
					}
					//Process and remove all messages after WM_INPUT
					while ( runLoop && PeekMessage( &theMessage, NULL, WM_INPUT + 1, ( UINT )-1, PM_REMOVE ) )
					{
						runLoop = ProcessMessage( theMessage );
					}

					// Pump the framework
					framework.Update();
				}
			}
			else
			{
				OutputDebugString( _T( "Could not initialize the game framework\n" ) );
			}

			// Shutdown the game object
			framework.Shutdown();
		}
		else
		{
			OutputDebugString( _T( "Could not create window\n" ) );
		}

		UnregisterClass( pClassName, hInstance );
	}
	else
	{
		OutputDebugString( _T( "Could not register window\n" ) );
	}

	return 0;
}

ATOM RegisterWindowClass( HINSTANCE hInstance, LPCTSTR applicationTitle )
{
	// Describe and register our window class
	// Returns a new window class ID
	// Returns zero if unsuccessful

	WNDCLASSEX WindowClassDescriptor;

	WindowClassDescriptor.cbSize = sizeof( WNDCLASSEX );

	WindowClassDescriptor.style = CS_OWNDC /*CS_HREDRAW | CS_VREDRAW*/;
	WindowClassDescriptor.lpfnWndProc = WindowProcedure;
	WindowClassDescriptor.cbClsExtra = 0;
	WindowClassDescriptor.cbWndExtra = 0;
	WindowClassDescriptor.hInstance = hInstance;
	WindowClassDescriptor.hIcon = NULL /*LoadIcon( hInstance, IDI_APPLICATION )*/;
	WindowClassDescriptor.hCursor = NULL /*LoadCursor( NULL, IDC_ARROW )*/;
	WindowClassDescriptor.hbrBackground = ( HBRUSH )( COLOR_BACKGROUND );
	WindowClassDescriptor.lpszMenuName = NULL;
	WindowClassDescriptor.lpszClassName = applicationTitle;
	WindowClassDescriptor.hIconSm = NULL;

	return RegisterClassEx( &WindowClassDescriptor );
}

HWND CreateAndInitializeWindow( HINSTANCE hInstance, LPCTSTR pClassName, int nCmdShow, LPCTSTR applicationTitle )
{
	// Create our main window
	// When horizontal position is set to CW_USEDEFAULT, vertical position is ignored
	// When width is set to CW_USEDEFAULT, height is ignored
	// Returns NULL if unsuccessful
	HWND hWindow = CreateWindow( pClassName,						// Window Class name or ID
		applicationTitle,				// Window name
		WS_OVERLAPPEDWINDOW,			// Window styles
		CW_USEDEFAULT,					// Horizontal position
		0,								// Verical position (ignored)
		CW_USEDEFAULT,					// Width
		0,								// Height (ignored)
		GetDesktopWindow(),				// Handle to parent window
		NULL,							// Handle to menu
		hInstance,						// Handle to application instance
		NULL );							// Pointer to CreateWindow parameters

										// Check the creation result
	if ( hWindow != NULL )
	{
		// Show the window we just created
		// We are ignoring the return result.
		// If the window was previously visible, the return value is nonzero. 
		// If the window was previously hidden, the return value is zero. 
		ShowWindow( hWindow, nCmdShow );

		// Tell the window to update its client area by sending a WM_PAINT message
		// We are ignoring the return result.
		// If the function succeeds, the return value is nonzero.
		UpdateWindow( hWindow );
	}

	// Return a handle to the window
	// This may be NULL if window creation was unsuccessful
	return hWindow;
}

LRESULT CALLBACK WindowProcedure( HWND hWindow, UINT nMessage, WPARAM wParam, LPARAM lParam )
{
	// Process any windows messages that are of interest to us
	switch ( nMessage )
	{
	case WM_PAINT:
		//hDeviceContext = BeginPaint( hWindow, &thePaintStructure );
		ValidateRect( hWindow, NULL );
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
		//case WM_SIZING:
	case WM_SIZE:
		//Framework::GetInstance()->ResizeWindow( LOWORD( lParam ), HIWORD( lParam ) );
		break;
	default:
		// Pass the message to the default handlers
		return DefWindowProc( hWindow, nMessage, wParam, lParam );
	}

	return 0;
}

UINT GetApplicationTitle( LPTSTR title, UINT titleMaxLength )
{
	// TODO: Add the version control system value?

	LPTSTR applicationTemplateName = _T( "Asgard : %s : %s : %s" );

#ifdef _DEBUG
	const TCHAR buildType[] = _T( "Debug Build" );
#else
	const TCHAR buildType[] = _T( "Release Build" );
#endif

	return _stprintf_s( title, titleMaxLength, applicationTemplateName, buildType, _T( __DATE__ ), _T( __TIME__ ) );
}

