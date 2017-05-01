// SHADER.CPP

#include <windows.h>
#include <codecvt>
#include "OpenGLShader.h"

namespace Muspelheim
{
	OpenGLShader::~OpenGLShader()
	{
		DeleteShaders();
	}

	bool OpenGLShader::Load( const std::wstring& shaderFile )
	{
		GLuint program = OpenGLInterface::CreateProgram();

		if( program )
		{
			// We need the full path and file name without any extensions
			std::wstring shaderPath( shaderFile );

			if( TryLoadingShader( program, shaderPath, GL_VERTEX_SHADER ) )
			{
				// Log info
				LogShaderInfo();
			}

			if( TryLoadingShader( program, shaderPath, GL_TESS_CONTROL_SHADER ) )
			{
				// Log info
				LogShaderInfo();
			}

			if( TryLoadingShader( program, shaderPath, GL_TESS_EVALUATION_SHADER ) )
			{
				// Log info
				LogShaderInfo();
			}

			if( TryLoadingShader( program, shaderPath, GL_FRAGMENT_SHADER ) )
			{
				// Log info
				LogShaderInfo();
			}

			// Attempt to link the program
			OpenGLInterface::LinkProgram( program );

			// Delete all of the shaders that were successfuly loaded
			DeleteShaders();

			// Retrieve the results of the program linking step
			GLint params;
			OpenGLInterface::GetProgramiv( program, GL_LINK_STATUS, &params );

			if( params == GL_TRUE )
			{
				m_Program = program;

				return true;
			}
		}

		return false;
	}

	bool OpenGLShader::TryLoadingShader( GLuint program, const std::wstring& shaderPath, unsigned int shaderType )
	{
		std::wstring filePath( shaderPath );

		switch( shaderType )
		{
		case GL_VERTEX_SHADER:
			filePath += L".vs";
			break;
		case GL_TESS_CONTROL_SHADER:
			filePath += L".tcs";
			break;
		case GL_TESS_EVALUATION_SHADER:
			filePath += L".tes";
			break;
		case GL_FRAGMENT_SHADER:
			filePath += L".fs";
			break;
		default:
			return false;
		}

		filePath += L".glsl";

		GLuint shaderId = OpenGLInterface::LoadShader( filePath, shaderType, true );

		if( shaderId )
		{
			if( OpenGLInterface::IsShader( shaderId ) && OpenGLInterface::IsProgram( program ) )
			{
				OpenGLInterface::AttachShader( program, shaderId );

				// Confirm the previous attach call succeeded
				if( glGetError() == GL_NO_ERROR )
				{
					m_LoadedShaderList.push_back( shaderId );

					return true;
				}
			}
		}

		return false;
	}

	void OpenGLShader::DeleteShaders()
	{
		for( GLuint shaderId : m_LoadedShaderList )
		{
			OpenGLInterface::DeleteShader( shaderId );
		}
		m_LoadedShaderList.clear();
	}

	bool OpenGLShader::Use()
	{
		if( m_Program )
		{
			OpenGLInterface::UseProgram( m_Program );

			GLenum results = glGetError();

			if( results == GL_NO_ERROR )
			{
				return true;
			}
		}

		return false;
	}

	GLint OpenGLShader::GetUniformVariableLocation( const std::wstring& variableName )
	{
		GLint variableLocation = -1;

		if( m_Program )
		{
			// Internally OpenGL shaders use string instead of wstring
			using convert_type = std::codecvt_utf8<wchar_t>;
			std::wstring_convert<convert_type, wchar_t> converter;
			std::string convertedVariableName = converter.to_bytes( variableName );

			variableLocation = OpenGLInterface::GetUniformLocation( m_Program, convertedVariableName.c_str() );
		}

		return variableLocation;
	}

	void OpenGLShader::LogShaderInfo()
	{
		GLint bufferLength;
		GLuint shaderId = m_LoadedShaderList.back();

		OpenGLInterface::GetShaderiv( shaderId, GL_INFO_LOG_LENGTH, &bufferLength );

		if( bufferLength > 0 )
		{
			GLchar* infoBuffer = new GLchar[ bufferLength ];

			GLsizei infoLength;
			OpenGLInterface::GetShaderInfoLog( shaderId, bufferLength, &infoLength, infoBuffer );

			// TODO: Change to message
			OutputDebugStringA( infoBuffer );
			OutputDebugStringA( "\n" );

			delete[] infoBuffer;
		}
	}
}