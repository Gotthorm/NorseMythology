// FIXEDCAMERA.CPP

#include <windows.h>
#include "FixedCamera.h"
#include "Helheimr.h"

namespace Vanaheimr
{
	FixedCamera::FixedCamera( const std::wstring& name )
	{
		SetName( name );
	}

	void FixedCamera::Update( float timeElapsed, Helheimr::Input* input )
	{

	}
}

