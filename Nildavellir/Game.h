// GAME.HPP

#ifndef _GAME_HPP_
#define _GAME_HPP_

#include "OpenGLInterface.h"

class Camera;
//class Input;

class Game
{
public:
	//
	Game();

	//
	~Game();

	//
	bool Init();

	//
	void Shutdown();

	//
	void Update();

	//
	Camera* GetCurrentCamera() { return m_pCurrentCamera; }

	//
	void EnableMouseCapture( bool bEnable );

private:
	Camera* m_pCurrentCamera;
	//Input* m_pInput;
	GLuint m_BufferName;
};

#endif // _GAME_HPP_

