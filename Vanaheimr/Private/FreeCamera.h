// FREECAMERA.H

#pragma once

#include "FixedCamera.h"

namespace Vanaheimr
{
	class FreeCamera : public FixedCamera
	{
	public:
		FreeCamera( const std::wstring& name );
		virtual ~FreeCamera() = default;

		virtual void Update( float timeElapsed, Helheimr::Input* input ) override;

		virtual glm::mat4 GetMatrix() const override;

	private:
		float m_CurrentPitch;
		float m_CurrentYaw;
	};
}

