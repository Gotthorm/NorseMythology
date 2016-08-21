// GRAPHICS.CPP

#include "Platform.h"
#include <windows.h>
#include <TCHAR.H>
#include <assert.h>
#include <stdio.h>
#include "Graphics.h"
#include "Text2D.h"
#include "Camera.h"
#include "OpenGLInterface.h"
#include <math.h>
#include <string>
#include "MessageManager.h"
#include <codecvt>
#include <sstream>

#pragma comment(lib, "OpenGL32.lib")

Graphics::Graphics() : 
	m_WindowHandleToDeviceContext(NULL),
	m_pCurrentCamera( NULL ),
	m_Width(640), 
	m_Height(480)
{
}

Graphics::~Graphics()
{
}

bool Graphics::InitializeRenderingContext(Platform::WindowHandle hWindow)
{
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
		PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
		32,                        //Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                        //Number of bits for the depthbuffer
		8,                        //Number of bits for the stencilbuffer
		0,                        //Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	m_WindowHandleToDeviceContext = GetDC(hWindow);

	if (m_WindowHandleToDeviceContext != NULL)
	{
		int letWindowsChooseThisPixelFormat = ChoosePixelFormat(m_WindowHandleToDeviceContext, &pfd);

		if (letWindowsChooseThisPixelFormat != 0)
		{
			if (SetPixelFormat(m_WindowHandleToDeviceContext, letWindowsChooseThisPixelFormat, &pfd) == TRUE)
			{
				HGLRC ourOpenGLRenderingContext = wglCreateContext(m_WindowHandleToDeviceContext);

				if (ourOpenGLRenderingContext != NULL)
				{
					if (wglMakeCurrent(m_WindowHandleToDeviceContext, ourOpenGLRenderingContext) == TRUE)
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool Graphics::InitializeSubSystems()
{
	// Initialize the OpenGL functions we will be using
	// This must be called after we make our OpenGL rendering context above
	OpenGLInterface::Initialize();

	RECT rect;
	HWND window = WindowFromDC( m_WindowHandleToDeviceContext );

	if( GetClientRect( window, &rect ) == FALSE )
	{
		MessageManager::GetInstance()->Post( Message::LOG_ERROR, std::wstring( L"Failed to extract the initial window size" ) );
	}

	unsigned int width = rect.right - rect.left;
	unsigned int height = rect.bottom - rect.top;

	if( m_Console.Initialize( width, height, 0.75f ) == false )
	{
		MessageManager::GetInstance()->Post( Message::LOG_ERROR, std::wstring( L"Failed to initialize the console system" ) );
	}
	else
	{
		// Set the console cache size to the number of lines needed in the log history
		m_Console.SetCacheSize( 20 );
	}

	SetWindowSize( width, height );

	// Initialize the 2D text system
	m_Text2D.init(128, 50);
	// overlay.init(128, 50);

	// Init the shaders
	InitShaders();

	OpenGLInterface::GenVertexArrays(1, &m_VertexArrayObject);
	OpenGLInterface::BindVertexArray(m_VertexArrayObject);

	return true;
}

//-----------------------------------------------------------------------------
// Name: SetupMatrices()
// Desc: Sets up the world, view, and projection transform matrices.
//-----------------------------------------------------------------------------
void Graphics::SetupMatrices()
{
	glMatrixMode(GL_PROJECTION); // Select The Projection Matrix
	glLoadIdentity();            // Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	//gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix
	glLoadIdentity();
}

void Graphics::SetWindowSize( unsigned short width, unsigned short height )
{
	m_Width = width;
	m_Height = height;

	m_Console.SetWindowSize( width, height );

	std::wstringstream s;
	s << L"SetWindowSize( " << width << L",  " << height << L" )";
	MessageManager::GetInstance()->Post( Message::LOG_INFO, s.str() );
}

void Graphics::Render(double timeElapsed)
{
	static double currentTime = 0;

	currentTime += timeElapsed;

	static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
	OpenGLInterface::ClearBufferfv(GL_COLOR, 0, green);

	glViewport(0, 0, m_Width, m_Height);

	OpenGLInterface::UseProgram(m_Program);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisable(GL_CULL_FACE);
	m_Text2D.draw();
	
	// Add test to determine if console is iitialized
	m_Console.Render();

	//m_Text2D.clear();
	
	SwapBuffers(m_WindowHandleToDeviceContext);
}

void Graphics::Shutdown()
{
	OpenGLInterface::DeleteVertexArrays(1, &m_VertexArrayObject);
	OpenGLInterface::DeleteProgram(m_Program);

	m_Console.Shutdown();

	m_Text2D.Shutdown();
}

bool Graphics::DisplayText( const wchar_t* pText, int posX, int posY )
{
	//return m_Text2D.Display( pText, posX, posY, 1.0f, 1.0f );
	m_Text2D.drawText(pText, posX, posY);

	return true;
}

void Graphics::SetCamera( Camera* pCamera )
{
	m_pCurrentCamera = pCamera;

	if( pCamera != NULL )
	{
		//pCamera->GetMatrix( m_MatrixView );
		//m_pDirect3DDevice->SetTransform( D3DTS_VIEW, &m_MatrixView );
	}
	else
	{
		// When there is no camera, we will revert to a default
		// position and point at the origin

		// For our world matrix, we set to the identity
		//D3DXMatrixIdentity( &m_MatrixWorld );
		//m_pDirect3DDevice->SetTransform( D3DTS_WORLD, &m_MatrixWorld );

		//// Set up our view matrix. A view matrix can be defined given an eye point,
		//// a point to lookat, and a direction for which way is up. Here, we set the
		//// eye five units back along the z-axis and up three units, look at the 
		//// origin, and define "up" to be in the y-direction.
		//D3DXVECTOR3 vEyePosition( -5.0f, 2.5f, -5.0f );
		//D3DXVECTOR3 vLookAtPosition( 0.0f, 0.0f, 0.0f );
		//D3DXVECTOR3 vUpVector( 0.0f, 1.0f, 0.0f );

		//D3DXMatrixLookAtLH( &m_MatrixView, &vEyePosition, &vLookAtPosition, &vUpVector );
		//m_pDirect3DDevice->SetTransform( D3DTS_VIEW, &m_MatrixView );

		//// For the projection matrix, we set up a perspective transform (which
		//// transforms geometry from 3D view space to 2D viewport space, with
		//// a perspective divide making objects smaller in the distance). To build
		//// a perpsective transform, we need the field of view (1/4 pi is common),
		//// the aspect ratio, and the near and far clipping planes (which define at
		//// what distances geometry should be no longer be rendered).
		//D3DXMatrixPerspectiveFovLH( &m_MatrixProjection, D3DX_PI/4, 1.0f, 1.0f, 100.0f );
		//m_pDirect3DDevice->SetTransform( D3DTS_PROJECTION, &m_MatrixProjection );
	}
}

void Graphics::InitShaders()
{
	static const char * vs_source[] =
	{
		"#version 420 core                                                 \n"
		"                                                                  \n"
		"void main(void)                                                   \n"
		"{                                                                 \n"
		"    const vec4 vertices[] = vec4[](vec4( 0.25, -0.25, 0.5, 1.0),  \n"
		"                                   vec4(-0.25, -0.25, 0.5, 1.0),  \n"
		"                                   vec4( 0.25,  0.25, 0.5, 1.0)); \n"
		"                                                                  \n"
		"    gl_Position = vertices[gl_VertexID];                          \n"
		"}                                                                 \n"
	};

	static const char * fs_source[] =
	{
		"#version 420 core                                                 \n"
		"                                                                  \n"
		"out vec4 color;                                                   \n"
		"                                                                  \n"
		"void main(void)                                                   \n"
		"{                                                                 \n"
		"    color = vec4(0.0, 0.8, 1.0, 1.0);                             \n"
		"}                                                                 \n"
	};

	m_Program = OpenGLInterface::CreateProgram();
	GLuint fs = OpenGLInterface::CreateShader(GL_FRAGMENT_SHADER);
	OpenGLInterface::ShaderSource(fs, 1, fs_source, NULL);
	OpenGLInterface::CompileShader(fs);

	GLuint vs = OpenGLInterface::CreateShader(GL_VERTEX_SHADER);
	OpenGLInterface::ShaderSource(vs, 1, vs_source, NULL);
	OpenGLInterface::CompileShader(vs);

	OpenGLInterface::AttachShader(m_Program, vs);
	OpenGLInterface::AttachShader(m_Program, fs);

	OpenGLInterface::LinkProgram(m_Program);
}

std::wstring Graphics::GetVersionInformation()
{
	// Determine what version of OpenGL is initialized
	const char* glVersion = (const char*)glGetString(GL_VERSION);
	CHAR tempString[256];
	sprintf_s( tempString, 256, "OpenGL information: %s", glVersion);

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring versionString = converter.from_bytes( tempString );

	return versionString;
}

void Graphics::ToggleConsole()
{
	m_Console.SetVisible( m_Console.IsVisible() == false );
}