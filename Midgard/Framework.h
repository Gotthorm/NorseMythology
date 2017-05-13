// FRAMEWORK.H

#ifndef _FRAMEWORK_H_
#define _FRAMEWORK_H_

#include <memory>
#include "Platform.h"
#include "Singleton.h"
#include "Muspelheim.h"

// Forward declarations
//namespace Muspelheim { class Renderer; class Surface; class Text; }
namespace Niflheim { class MessageManager; }
namespace Niflheim { class Logger; }
namespace Vanaheimr { class CameraManager; }
class Game;
class Input;
class Loki;
class Volstagg;

/// \brief Encapsulation all of the engine's subsystems.
///
/// The Framework class is meant to be the parent object of all of the systems that actually implement the game play.
/// All of the main systems will be allocated, initialized, updated, and shutdown using this class.
/// This will abstract any platform specific code from the game engine internals.
class Framework : public Singleton<Framework>
{
public:
	/// \brief The main initialization of the framework.
	/// 
	/// This is called to initialize all subsystems and to put the game in a ready state.
	/// If this call fails, you should immediately call Shutdown to clean up any subsystems that might have been successfully initialized.
	/// \param hWindow A constant character pointer.
	/// \param launchInfo A reference to launch parameters.
	/// \return Returns true if all subsystems were initialized properly.
	bool Init( Platform::WindowHandle hWindow, const Platform::LaunchInfo& launchInfo );

	/// \brief The main shutdown of the framework.
	/// 
	/// This is called to shutdown all subsystems and release all memory that was allocated when the system was initialized.
	void Shutdown();

	//
	void Update();

	/// \brief Notify the framework that an input event has occurred.
	///
	/// This is called by the platform anytime an input event occurs.
	/// \param lParam A platform specific data type containing information regarding the event.
	void ProcessInputEvent( Platform::LongParam lParam );

	/// \brief Notify the framework that the window has been resized.
	/// 
	/// This should be called directly from the platform whenever the window has been resized.
	/// \param width The new window width in pixels.
	/// \param height The new window height in pixels.
	void ResizeWindow( unsigned short width, unsigned short height );

private:
	// Use the default implementation
	Framework() = default;

	// Use the default implementation
	virtual ~Framework() = default;

	// Remove the default assignment operator
	Framework & operator =( const Framework & ) = delete;

	// Remove the copy constructor
	Framework( const Framework & ) = delete;

	// Subsystems
	Muspelheim::Renderer* m_Renderer = nullptr;
	std::shared_ptr<Niflheim::MessageManager> m_MessageManager;
	Niflheim::Logger* m_Logger = nullptr;

	Loki* m_Loki = nullptr;
	Volstagg* m_Volstagg = nullptr;
	Game* m_pGame = nullptr;
	Input* m_pInput = nullptr;

	Platform::WindowHandle m_hWindow = 0;

	// The main screen
	Muspelheim::SurfaceID m_MainScreen = Muspelheim::InvalidSurface;

	Vanaheimr::CameraManager* m_CameraManager;

	bool m_Initialized = false;

	// Tracking mouse capture
	bool m_MouseCaptured = false;

	// State variables for FPS tracking
	unsigned int m_OldFrameTime = 0;
	unsigned int m_CurrentFPS = 0;
	unsigned int m_OneSecondIntervalAccumulator = 0;
	unsigned int m_UpdateAccumulator = 0;

	// Provide friend access to Singleton<Input> specialization
	friend Singleton<Framework>;
};

#endif // _FRAMEWORK_H_

