#pragma once

#include <Windows.h>
#include <gl\GL.h>
//#include "glcorearb.h"
#include "glext.h"
#include "wglext.h"
#include <string>

struct OpenGLInterface
{
	static bool Initialize();
	static GLuint LoadShader( const std::wstring& filename, GLenum shader_type, bool check_errors );

	static PFNGLCREATEPROGRAMPROC CreateProgram;
	static PFNGLCREATESHADERPROC CreateShader;
	static PFNGLSHADERSOURCEPROC ShaderSource;
	static PFNGLCOMPILESHADERPROC CompileShader;
	static PFNGLATTACHSHADERPROC AttachShader;
	static PFNGLLINKPROGRAMPROC LinkProgram;
	static PFNGLGENVERTEXARRAYSPROC GenVertexArrays;
	static PFNGLBINDVERTEXARRAYPROC BindVertexArray;
	static PFNGLUSEPROGRAMPROC UseProgram;
	static PFNGLDELETEVERTEXARRAYSPROC DeleteVertexArrays;
	static PFNGLDELETEPROGRAMPROC DeleteProgram;
	static PFNGLDELETESHADERPROC DeleteShader;
	static PFNGLACTIVETEXTUREPROC ActiveTexture;
	static PFNGLBINDTEXTUREUNITPROC BindTexture;
	static PFNGLTEXSTORAGE1DPROC TexStorage1D;
	static PFNGLTEXSTORAGE2DPROC TexStorage2D;
	static PFNGLTEXSTORAGE3DPROC TexStorage3D;
	static PFNGLCOMPRESSEDTEXIMAGE2DPROC CompressedTexImage2D;
	static PFNGLTEXSUBIMAGE2DEXTPROC TexSubImage2D;
	static PFNGLTEXSUBIMAGE3DPROC TexSubImage3D;
	static PFNGLGENERATEMIPMAPPROC GenerateMipmap;
	static PFNGLVERTEXATTRIB2FVPROC VertexAttrib2fv;
	static PFNGLVERTEXATTRIB4FVPROC VertexAttrib4fv;
	static PFNGLVERTEXATTRIB1FPROC VertexAttrib1f;
	static PFNGLUNIFORMMATRIX4FVPROC UniformMatrix4fv;
	static PFNGLGETOBJECTPARAMETERIVARBPROC GetObjectParameterivARB;
	static PFNGLGETSHADERINFOLOGPROC GetShaderInfoLog;
	static PFNGLGETUNIFORMLOCATIONPROC GetUniformLocation;
	static PFNGLUNIFORM2FVPROC Uniform2fv;
	static PFNGLUNIFORM1IPROC Uniform1i;

	//typedef void (APIENTRY* PFNGLCLEARBUFFERFVPROC)(GLenum, GLint, const GLfloat*);
	static PFNGLCLEARBUFFERFVPROC ClearBufferfv;
	static PFNGLCREATEBUFFERSPROC CreateBuffers;
	static PFNGLNAMEDBUFFERSTORAGEPROC NamedBufferStorage;
	static PFNGLBUFFERSUBDATAPROC BufferSubData;
	static PFNGLBINDBUFFERPROC BindBuffer;
	static PFNGLGENBUFFERSPROC GenBuffers;
	static PFNGLVERTEXATTRIBPOINTERPROC VertexAttribPointer;
	static PFNGLENABLEVERTEXATTRIBARRAYPROC EnableVertexAttribArray;

	//typedef void (APIENTRY* PFNGLBUFFERDATAPROC)(GLenum, GLuint, const GLvoid *, GLenum);
	static PFNGLBUFFERDATAPROC BufferData;

	//typedef void (APIENTRY* PFNGLDRAWARRAYSPROC)(GLenum, GLint, GLsizei);
	//static PFNGLDRAWARRAYSPROC DrawArrays;
	static PFNGLDRAWARRAYSINSTANCEDPROC DrawArraysInstanced;
	static PFNGLPATCHPARAMETERIPROC PatchParameteri;
	static PFNGLGETSHADERIVPROC GetShaderiv;

	static PFNGLGETPROGRAMIVPROC GetProgramiv;
	static PFNGLISSHADERPROC IsShader;
	static PFNGLISPROGRAMPROC IsProgram;

	static PFNWGLSWAPINTERVALEXTPROC SwapInterval;
	static PFNWGLGETSWAPINTERVALEXTPROC GetSwapInterval;

	static std::wstring VersionString;
};
