//#include "Windows.h"
#include "OpenGLInterface.h"
#include "Utility.h"
#include <string>
#include <fstream>
#include <streambuf>


PFNGLCREATEPROGRAMPROC OpenGLInterface::CreateProgram = NULL;
PFNGLCREATESHADERPROC OpenGLInterface::CreateShader = NULL;
PFNGLSHADERSOURCEPROC OpenGLInterface::ShaderSource = NULL;
PFNGLCOMPILESHADERPROC OpenGLInterface::CompileShader = NULL;
PFNGLATTACHSHADERPROC OpenGLInterface::AttachShader = NULL;
PFNGLLINKPROGRAMPROC OpenGLInterface::LinkProgram = NULL;
PFNGLGENVERTEXARRAYSPROC OpenGLInterface::GenVertexArrays = NULL;
PFNGLBINDVERTEXARRAYPROC OpenGLInterface::BindVertexArray = NULL;
PFNGLUSEPROGRAMPROC OpenGLInterface::UseProgram = NULL;
PFNGLDELETEVERTEXARRAYSPROC OpenGLInterface::DeleteVertexArrays = NULL;
PFNGLDELETEPROGRAMPROC OpenGLInterface::DeleteProgram = NULL;
PFNGLDELETESHADERPROC OpenGLInterface::DeleteShader = NULL;
PFNGLACTIVETEXTUREPROC OpenGLInterface::ActiveTexture = NULL;
PFNGLBINDTEXTUREUNITPROC OpenGLInterface::BindTexture = NULL;
PFNGLTEXSTORAGE1DPROC OpenGLInterface::TexStorage1D = NULL;
PFNGLTEXSTORAGE2DPROC OpenGLInterface::TexStorage2D = NULL;
PFNGLTEXSTORAGE3DPROC OpenGLInterface::TexStorage3D = NULL;
PFNGLCOMPRESSEDTEXIMAGE2DPROC OpenGLInterface::CompressedTexImage2D = NULL;
PFNGLTEXSUBIMAGE2DEXTPROC OpenGLInterface::TexSubImage2D = NULL;
PFNGLTEXSUBIMAGE3DPROC OpenGLInterface::TexSubImage3D = NULL;
PFNGLGENERATEMIPMAPPROC OpenGLInterface::GenerateMipmap = NULL;
PFNGLVERTEXATTRIB4FVPROC OpenGLInterface::VertexAttrib4fv = NULL;
PFNGLVERTEXATTRIB1FPROC OpenGLInterface::VertexAttrib1f = NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC OpenGLInterface::GetObjectParameterivARB = NULL;
PFNGLGETSHADERINFOLOGPROC OpenGLInterface::GetShaderInfoLog = NULL;


PFNGLCLEARBUFFERFVPROC OpenGLInterface::ClearBufferfv = NULL;
PFNGLCREATEBUFFERSPROC OpenGLInterface::CreateBuffers = NULL;
PFNGLNAMEDBUFFERSTORAGEPROC OpenGLInterface::NamedBufferStorage = NULL;
PFNGLBINDBUFFERPROC OpenGLInterface::BindBuffer = NULL;
PFNGLBUFFERSUBDATAPROC OpenGLInterface::BufferSubData = NULL;

PFNGLBUFFERDATAPROC OpenGLInterface::BufferData = NULL;
//PFNGLDRAWARRAYSPROC OpenGLInterface::DrawArrays = NULL;

bool OpenGLInterface::Initialize()
{
	CreateProgram = (PFNGLCREATEPROGRAMPROC)(Utility::GetOpenGLFunctionAddress("glCreateProgram"));
	CreateShader = (PFNGLCREATESHADERPROC)(Utility::GetOpenGLFunctionAddress("glCreateShader"));
	ShaderSource = (PFNGLSHADERSOURCEPROC)(Utility::GetOpenGLFunctionAddress("glShaderSource"));
	CompileShader = (PFNGLCOMPILESHADERPROC)(Utility::GetOpenGLFunctionAddress("glCompileShader"));
	AttachShader = (PFNGLATTACHSHADERPROC)(Utility::GetOpenGLFunctionAddress("glAttachShader"));
	LinkProgram = (PFNGLLINKPROGRAMPROC)(Utility::GetOpenGLFunctionAddress("glLinkProgram"));
	GenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)(Utility::GetOpenGLFunctionAddress("glGenVertexArrays"));
	BindVertexArray = (PFNGLBINDVERTEXARRAYPROC)(Utility::GetOpenGLFunctionAddress("glBindVertexArray"));
	UseProgram = (PFNGLUSEPROGRAMPROC)(Utility::GetOpenGLFunctionAddress("glUseProgram"));
	DeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)(Utility::GetOpenGLFunctionAddress("glDeleteVertexArrays"));
	DeleteProgram = (PFNGLDELETEPROGRAMPROC)(Utility::GetOpenGLFunctionAddress("glDeleteProgram"));
	DeleteShader = (PFNGLDELETESHADERPROC)(Utility::GetOpenGLFunctionAddress("glDeleteShader"));
	ActiveTexture = (PFNGLACTIVETEXTUREPROC)(Utility::GetOpenGLFunctionAddress("glActiveTexture"));
	BindTexture = (PFNGLBINDTEXTUREUNITPROC)(Utility::GetOpenGLFunctionAddress("glBindTexture"));
	TexStorage1D = (PFNGLTEXSTORAGE1DPROC)(Utility::GetOpenGLFunctionAddress("glTexStorage1D"));
	TexStorage2D = (PFNGLTEXSTORAGE2DPROC)(Utility::GetOpenGLFunctionAddress("glTexStorage2D"));
	TexStorage3D = (PFNGLTEXSTORAGE3DPROC)(Utility::GetOpenGLFunctionAddress("glTexStorage3D"));
	CompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC)(Utility::GetOpenGLFunctionAddress("glCompressedTexImage2D"));
	TexSubImage2D = (PFNGLTEXSUBIMAGE2DEXTPROC)(Utility::GetOpenGLFunctionAddress("glTexSubImage2D"));
	TexSubImage3D = (PFNGLTEXSUBIMAGE3DPROC)(Utility::GetOpenGLFunctionAddress("glTexSubImage3D"));
	GenerateMipmap = (PFNGLGENERATEMIPMAPPROC)(Utility::GetOpenGLFunctionAddress("glGenerateMipmap"));
	VertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC)(Utility::GetOpenGLFunctionAddress("glVertexAttrib4fv"));
	VertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC)(Utility::GetOpenGLFunctionAddress("glVertexAttrib1f"));
	GetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC)(Utility::GetOpenGLFunctionAddress("glGetObjectParameterivARB"));
	GetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)(Utility::GetOpenGLFunctionAddress("glGetShaderInfoLog"));

	ClearBufferfv = (PFNGLCLEARBUFFERFVPROC)(Utility::GetOpenGLFunctionAddress("glClearBufferfv"));
	CreateBuffers = (PFNGLCREATEBUFFERSPROC)(Utility::GetOpenGLFunctionAddress("glCreateBuffers"));
	NamedBufferStorage = (PFNGLNAMEDBUFFERSTORAGEPROC)(Utility::GetOpenGLFunctionAddress("glNamedBufferStorage"));
	BindBuffer = (PFNGLBINDBUFFERPROC)(Utility::GetOpenGLFunctionAddress("glBindBuffer"));
	BufferData = (PFNGLBUFFERDATAPROC)(Utility::GetOpenGLFunctionAddress("glBufferData"));
	BufferSubData = (PFNGLBUFFERSUBDATAPROC)(Utility::GetOpenGLFunctionAddress("glBufferSubData"));
	//DrawArrays = (PFNGLDRAWARRAYSPROC)(Utility::GetOpenGLFunctionAddress("glDrawArrays"));

	return true;
}

GLuint OpenGLInterface::LoadShader(const char* filename, GLenum shader_type, bool check_errors)
{
	GLuint result = 0;

	// Attempt to open the given file by name
	std::ifstream t(filename);

	// Test if the file open step was successful
	if ((t.rdstate() & std::ifstream::failbit) == 0)
	{
		std::string str;

		// Determine the size of the file and preallocate the size of the string we will be reading into
		t.seekg(0, std::ios::end);
		str.reserve((size_t)t.tellg());
		t.seekg(0, std::ios::beg);

		// Read the entire file's contents into the string
		str.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

		// Create an empty shader of the given type
		result = CreateShader(shader_type);

		if (result)
		{
			const char* data = str.c_str();

			ShaderSource(result, 1, &data, NULL);

			CompileShader(result);

			if (check_errors)
			{
				GLint status = 0;
				GetObjectParameterivARB(result, GL_COMPILE_STATUS, &status);

				if (!status)
				{
					char buffer[4096];
					GetShaderInfoLog(result, 4096, NULL, buffer);

					OutputDebugStringA(filename);
					OutputDebugStringA(":");
					OutputDebugStringA(buffer);
					OutputDebugStringA("\n");

					DeleteShader(result);
				}
			}
		}
		else
		{
			// Failed shader alloc
			OutputDebugStringA("Failed shader alloc\n");
		}
	}
	else
	{
		// Failed to open shader text file
		OutputDebugStringA("Failed to open shader file\n");
	}
		
	return result;
}