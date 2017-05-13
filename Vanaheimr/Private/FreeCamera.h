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

		virtual void Update( float timeElapsed, const Helheimr::Input* input ) override;

		virtual void SetDirection( const glm::vec3& direction ) override;

	protected:
		void UpdateData();

		float m_Pitch;
		float m_Yaw;

		glm::vec3 m_Right;
	};
}

