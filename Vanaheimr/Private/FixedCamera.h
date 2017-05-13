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

		// Fixed camera has nothing to update
		virtual void Update( float timeElapsed, const Helheimr::Input* input ) {}

		//
		virtual glm::mat4 GetViewMatrix() const override { return glm::lookAt( m_Position, m_Position + m_Front, m_Up ); }

		//
		virtual const std::wstring& GetName() const override { return m_Name; }

		//
		virtual void SetPosition( const glm::vec3& position ) override { m_Position = position; }

		//
		virtual void SetDirection( const glm::vec3& direction ) override;

	protected:
		std::wstring m_Name;

		glm::vec3 m_Position;
		glm::vec3 m_Front;
		glm::vec3 m_Up;

		const glm::vec3 m_WorldUp;
	};
}

