// FRAMEWORK.H

#pragma once

#include <memory>
#include "Platform.h"
#include "Singleton.h"
#include "FrameTime.h"

// Forward declarations
namespace Muspelheim { class Renderer; }
namespace Niflheim { class MessageManager; class Logger;  }
namespace Vanaheimr { class CameraManager; }
class Game;
//class Input;
class Loki;
class Volstagg;
class FPS;

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
	bool Init( Platform::WindowHandle hWindow, Platform::LaunchInfo const & launchInfo );

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
	Framework();
	virtual ~Framework() = default;

	// Remove the default assignment operator
	Framework & operator =( Framework const & ) = delete;

	// Remove the copy constructor
	Framework( Framework const & ) = delete;

	// Subsystems
	Muspelheim::Renderer* m_pRenderer;
	Niflheim::Logger* m_pLogger;
	Game* m_pGame;
	//Input* m_pInput;
	Vanaheimr::CameraManager* m_pCameraManager;
	std::shared_ptr<Niflheim::MessageManager> m_MessageManager;

	// The main window identifier
	Platform::WindowHandle m_WindowHandle;

	// The main screen (surface) identifier
	unsigned char m_MainScreenID;

	// TODO : move these into game
	Loki* m_pLoki1 = nullptr;
	//Loki* m_Loki2 = nullptr;
	//Loki* m_Loki3 = nullptr;
	//Loki* m_Loki4 = nullptr;
	Volstagg* m_pVolstagg = nullptr;


	bool m_Initialized = false;

	// TODO: Shouldnt this be in the input module?
	// Tracking mouse capture
	bool m_MouseCaptured = false;

	FrameTime m_FrameTime;

	// Provide friend access to Singleton<Input> specialization
	friend Singleton<Framework>;
};
