// FRAMEWORK.H

#pragma once

#include <memory>
#include "Platform.h"

// Forward declarations
namespace Muspelheim { class Renderer; }

/// \brief Encapsulation all of the engine's subsystems.
///
/// The Framework class is meant to be the parent object of all of the systems that actually implement the game play.
/// All of the main systems will be allocated, initialized, updated, and shutdown using this class.
/// This will abstract any platform specific code from the game engine internals.
class Framework
{
public:
	Framework();
	virtual ~Framework() = default;

	/// \brief The main initialization of the framework.
	/// 
	/// This is called to initialize all subsystems and to put the game in a ready state.
	/// If this call fails, you should immediately call Shutdown to clean up any subsystems that might have been successfully initialized.
	/// \param hWindow A constant character pointer.
	/// \param launchInfo A reference to launch parameters.
	/// \return Returns true if all subsystems were initialized properly.
	bool Init( Platform::WindowHandle hWindow, Platform::LaunchInfo const & launchInfo );

	/// \brief The main shutdown of the framework.
	/// 
	/// This is called to shutdown all subsystems and release all memory that was allocated when the system was initialized.
	void Shutdown();

	//
	void Update();

	/// \brief Notify the framework that the window has been resized.
	/// 
	/// This should be called directly from the platform whenever the window has been resized.
	/// \param width The new window width in pixels.
	/// \param height The new window height in pixels.
	void ResizeWindow( unsigned short width, unsigned short height );

private:
	// Remove the default assignment operator
	Framework & operator =( Framework const & ) = delete;

	// Remove the copy constructor
	Framework( Framework const & ) = delete;

	// The main window identifier
	Platform::WindowHandle m_WindowHandle;

	// The main screen (surface) identifier
	unsigned char m_MainScreenID;

	// Subsystems
	std::shared_ptr<Muspelheim::Renderer> m_Renderer;

	bool m_Initialized = false;
};
