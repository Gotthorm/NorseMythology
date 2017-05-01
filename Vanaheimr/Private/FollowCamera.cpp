// FOLLOWCAMERA.CPP

#include <windows.h>
#include "FollowCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/compatibility.hpp>
#include "Helheimr.h"

namespace Vanaheimr
{
	FollowCamera::FollowCamera( const std::wstring& name, const Object3D* target ) : FixedCamera( name ), m_Target( target ), m_TargetOffset( 0.0f )
	{
	}

	void FollowCamera::Update( float timeElapsed, Helheimr::Input* input )
	{
		if( m_Target != nullptr )
		{
			// Get the normalized direction vector of the target scaled by the correct follow distance
			//glm::vec3 targetOffset = m_Target->GetForward() * m_TargetOffset.x;

			//// Create the base follow target position
			//glm::vec3 targetPosition = m_Target->GetPosition() - targetOffset;

			//// Now adjust for the desired height
			//targetPosition += ( m_Target->GetUp() * m_TargetOffset.y );

			//SetPosition( targetPosition );

			//glm::vec3 directionVector = glm::normalize( m_Target->GetPosition() - targetPosition );

			//SetDirection( directionVector );
		}
	}
}