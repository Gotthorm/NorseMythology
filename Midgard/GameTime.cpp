#include "GameTime.h"
#include <chrono>
#include <iostream>

unsigned int const OneSecondInMilliseconds = 1000;

// Use a global variable private to this file to hide implementation
namespace 
{
	std::chrono::time_point<std::chrono::steady_clock> oldTimePoint;
}

GameTime::GameTime()
	: m_Duration(0.0f)
	, m_FPS( 0 )
	, m_OneSecondIntervalAccumulator( 0.0f )
	, m_UpdateAccumulator( 0 )
{
	oldTimePoint = std::chrono::high_resolution_clock::now();
}

void GameTime::Update()
{
	std::chrono::time_point<std::chrono::steady_clock> currentTimePoint = std::chrono::high_resolution_clock::now();

	//auto duration = std::chrono::duration_cast< std::chrono::seconds >( currentTimePoint - m_OldTimePoint ).count();

	auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>( currentTimePoint - oldTimePoint ).count();

	m_Duration = microseconds / 1000000.0f;

	oldTimePoint = currentTimePoint;

	m_OneSecondIntervalAccumulator += m_Duration;

	// Increment the update accumulator;
	++m_UpdateAccumulator;

	if ( m_OneSecondIntervalAccumulator >= 1.0f )
	{
		unsigned int secondsElapsed = 0;

		while ( m_OneSecondIntervalAccumulator >= 1.0f )
		{
			++secondsElapsed;
			m_OneSecondIntervalAccumulator -= 1.0f;
		}

		m_FPS = m_UpdateAccumulator / secondsElapsed;

		m_UpdateAccumulator = 0;
	}

}

