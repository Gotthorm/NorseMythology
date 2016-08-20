// FRAMEWORK.H

#ifndef _FRAMEWORK_H_
#define _FRAMEWORK_H_

#include <windows.h>
#include "Graphics.h"
#include "Game.h"
#include "Input.h"
#include "Logger.h"

class Framework
{
public:
	/*! \struct LaunchInfo
	* OS specific parameters defined by the launch application.
	*/
	struct LaunchInfo
	{
		/*! The application title */
		LPCTSTR applicationTitle;
		/*! The maximum number of threads that can be utilized */
		int numberOfThreads;
	};

	//
	Framework();

	// Use the default destructor implementation
	virtual ~Framework() = default;

	//! The main initialization of the framework.
	/*! Some more detailed information
	\param instance an integer argument.
	\param window a constant character pointer.
	\param launchInfo a reference to launch parameters.
	\return The test results
	*/	bool Init( HINSTANCE instance, HWND window, const LaunchInfo& launchInfo );

	// 
	void Shutdown();

	//
	void Update();

	//
	void UpdateInput(LPARAM lParam);

	//
	//void Render();

	//
	void EnableMouseCapture( bool enable );

	//
	void ResizeWindow(int width, int height);

private:
	// Remove the default assignment operator
	Framework & operator =( const Framework & ) = delete;

	// Remove the copy constructor
	Framework( const Framework & ) = delete;

	Graphics m_Graphics;
	Game m_Game;
	Input m_Input;

	Logger m_Logger;

	bool m_Initialized;

	DWORD m_OldFrameTime;
	DWORD m_CurrentFPS;
	DWORD m_OneSecondIntervalAccumulator;
	DWORD m_UpdateAccumulator;
};

#endif // _FRAMEWORK_H_

