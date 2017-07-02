// FOLLOWCAMERA.H

#pragma once

#include "FixedCamera.h"

namespace Vanaheimr
{
	class FollowCamera : public FixedCamera
	{
	public:
		FollowCamera( const std::wstring& name, const Object3D* target );
		virtual ~FollowCamera() = default;

		virtual void Update( float timeElapsed, Helheimr::Input* input ) override;

		void SetTarget( const Object3D* target ) { m_Target = target; }

		void SetOffset( const glm::vec2& offset ) { m_TargetOffset = offset; }

	private:
		const Object3D* m_Target;
		glm::vec2 m_TargetOffset;
	};
}

