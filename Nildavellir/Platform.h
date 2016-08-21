// PLATFORM.H

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

// Platform specific includes
#if defined _WINDOWS
#include <Windows.h>
#include <assert.h>
#endif

#include <string>

// Platform specific macros
#if defined _WINDOWS

// String constant macro
//#define PLATFORM_STRING_CONST(x) _T(x)

// Assert macro which only works in debug builds
#ifdef _DEBUG
#define PLATFORM_ASSERT(x) assert(x)
#else
#define PLATFORM_ASSERT(x)
#endif

//
#ifdef _DEBUG
#define PLATFORM_DEBUG_MESSAGE(x) OutputDebugString(x);
#else
#define PLATFORM_DEBUG_MESSAGE(x)
#endif

#endif

namespace Platform
{
	// Define generic type definitions for platform specific data types
#if defined _WINDOWS
	typedef HWND WindowHandle;
	typedef LPARAM LongParam;
#endif

	//
	const unsigned int kMaxStringLength = 256;

	/*! \struct LaunchInfo
	* OS specific parameters that are used to initialize the framework.
	*/
	struct LaunchInfo
	{
		/*! The application title */
		std::wstring applicationTitle;

		/*! The maximum number of threads that can be utilized by the application */
		int numberOfThreads;
	};

#if defined _WINDOWS
	inline unsigned int GetTime() { return ( unsigned int )timeGetTime(); }
#endif
}

#endif // _PLATFORM_H_
