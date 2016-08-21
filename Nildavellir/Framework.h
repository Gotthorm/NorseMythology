// FRAMEWORK.H

#ifndef _FRAMEWORK_H_
#define _FRAMEWORK_H_

#include "Platform.h"

// Forward declarations
class Graphics;
class Game;
class Input;
class Logger;

//! Encapsulation all of the engine's subsystems.
/*!
The Framework class is meant to be the parent object of all of the systems that actually implement the game play.
All of the main systems will be allocated, initialized, updated, and shutdown using this class.
This will abstract any platform specific code from the game engine internals.
*/
class Framework
{
public:
	//
	Framework();

	// Use the default destructor implementation
	virtual ~Framework() = default;

	//! The main initialization of the framework.
	/*! 
	This is called to initialize all subsystems and to put the game in a ready state.
	If this call fails, you should immediately call Shutdown to clean up any subsystems that might have been successfully initialized.
	\param hWindow a constant character pointer.
	\param launchInfo a reference to launch parameters.
	\return Returns true if all subsystems were initialized properly.
	*/
	bool Init( Platform::WindowHandle hWindow, const Platform::LaunchInfo& launchInfo );

	//! The main shutdown of the framework.
	/*! 
	This is called to shutdown all subsystems and release all memory that was allocated when the system was initialized.
	*/
	void Shutdown();

	//
	void Update();

	//
	void UpdateInput( Platform::LongParam lParam );

	//
	void EnableMouseCapture( bool enable );

	// The OS should call this whenever the main window is resized
	//! Notify the framework that the window has been resized.
	/*! 
	This should be called directly from the platform whenever the window has been resized.
	*/
	void ResizeWindow( int width, int height );

private:
	// Remove the default assignment operator
	Framework & operator =( const Framework & ) = delete;

	// Remove the copy constructor
	Framework( const Framework & ) = delete;

	// Subsystems
	Graphics* m_pGraphics;
	Game* m_pGame;
	Input* m_pInput;
	Logger* m_pLogger;

	bool m_Initialized;

	// State variables for FPS tracking
	unsigned int m_OldFrameTime;
	unsigned int m_CurrentFPS;
	unsigned int m_OneSecondIntervalAccumulator;
	unsigned int m_UpdateAccumulator;
};

#endif // _FRAMEWORK_H_

