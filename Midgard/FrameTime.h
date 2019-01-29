// FrameTime.h

#pragma once

/// \brief Encapsulation of the duration of a frame.
///
/// The FrameTime class encapsulates timing utilities for the framework.
/// This will abstract any platform specific timing code from the game engine internals.
class FrameTime
{
public:
	FrameTime();
	~FrameTime() = default;

	/// \brief Notification that a new frame has occurred.
	///
	/// This should be called at the beginning of each new frame.
	/// None of the other functionality of this class will be accurate if this method is not called.
	void Update();

	/// \brief Retrieve the current frame duration.
	///
	/// The duration value is the amount of time that elapsed between the last two calls to Update().
	/// \return Returns the duration in seconds.
	float Duration() { return m_Duration; }

	/// \brief Retrieve the current frames per seconds value.
	///
	/// The FPS value is updated every time one second elapses to provide a stable value.
	/// \return Returns the duration in seconds.
	unsigned int FPS() { return m_FPS; }

private:
	float m_Duration;
	unsigned int m_FPS;

	float m_OneSecondIntervalAccumulator;
	unsigned int m_UpdateAccumulator;
};

