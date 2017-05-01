// SHADER.H

#ifndef _SHADER_H_
#define _SHADER_H_

#include <vector>
#include <string>
#include "OpenGLInterface.h"

namespace Muspelheim
{
	class OpenGLShader
	{
	public:
		OpenGLShader() = default;

		virtual ~OpenGLShader();

		//
		bool Load( const std::wstring& shaderName );

		//
		bool Use();

		//
		GLint GetUniformVariableLocation( const std::wstring& variableName );

	private:
		bool TryLoadingShader( GLuint program, const std::wstring& shaderFile, unsigned int shaderType );

		void DeleteShaders();

		void LogShaderInfo();

		GLuint m_Program = 0;

		std::vector<GLuint> m_LoadedShaderList;
	};
}

#endif // _SHADER_H_

