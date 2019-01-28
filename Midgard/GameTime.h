#pragma once

class GameTime
{
public:
	GameTime();
	~GameTime() = default;

	void Update();

	float Duration() { return m_Duration; }

	unsigned int FPS() { return m_FPS; }

private:
	float m_Duration;
	unsigned int m_FPS;

	float m_OneSecondIntervalAccumulator;
	unsigned int m_UpdateAccumulator;
};

