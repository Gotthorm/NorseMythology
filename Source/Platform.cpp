// PLATFORM.CPP

#include "Platform.h"

#pragma comment(lib, "Winmm.lib")

namespace Platform
{
	unsigned int GetTime() 
	{ 
#if defined _WINDOWS
		return ( unsigned int )timeGetTime();
#endif
	}
}
