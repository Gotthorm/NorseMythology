// FIXEDCAMERA.H

#pragma once

#include "Vanaheimr.h"

namespace Helheimr { class Input; }

namespace Vanaheimr
{
	class FixedCamera : public Camera
	{
	public:
		FixedCamera( const std::wstring& name );
		virtual ~FixedCamera() = default;

		virtual void Update( float timeElapsed, std::shared_ptr<Helheimr::Input> const & input );
	};
}

