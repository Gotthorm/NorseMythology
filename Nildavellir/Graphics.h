// GRAPHICS.HPP

#ifndef _GRAPHICS_HPP_
#define _GRAPHICS_HPP_

//
#include "Text2D.h"
#include "Console.h"
#include <gl\GL.h>

class Camera;

class Graphics
{
public:
	//
	Graphics();

	// 
	~Graphics();

	//
    bool InitializeRenderingContext( HWND hWindow );

	//
	bool InitializeSubSystems();

	//
	void Shutdown();

	//
	void Render(double timeElapsed);

	//
	void SetWindowSize(int width, int height);

	//
	bool DisplayText( LPCSTR pText, int posX, int posY );

	//
	void SetCamera( Camera* pCamera );

	//
	bool GetVersionInformation(std::wstring& VersionString);

	//
	void ToggleConsole();

private:
	//bool InitGeometry();
	void SetupMatrices();
	void InitShaders();

	HDC m_WindowHandleToDeviceContext;

	GLuint m_Program;
	GLuint m_VertexArrayObject;
	GLint m_Width;
	GLint m_Height;

	Text2D m_Text2D;

	Console m_Console;

	Camera* m_pCurrentCamera;
};

#endif // _GRAPHICS_HPP_
