// ORBITCAMERA.CPP

#include <windows.h>
#include "OrbitCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/compatibility.hpp>
#include "Helheimr.h"

namespace Vanaheimr
{
	OrbitCamera::OrbitCamera( const std::wstring& name, const Object3D* target ) : FixedCamera( name ), m_Target( target ), m_TargetOffset( 0.0f )
	{
	}

	void OrbitCamera::Update( float timeElapsed, const Helheimr::Input* input )
	{
		if( m_Target != NULL )
		{
			//glm::mat4 matrix = m_Target->GetMatrix();

			//// Get the normalized direction vector of the target scaled by the correct follow distance
			//glm::vec3 oldDirectionVector = matrix[2] * m_TargetOffset.x;

			//// Create the base follow target position
			//glm::vec3 targetPosition = m_Target->GetPosition() - oldDirectionVector;

			//// Now adjust for the desired height
			//targetPosition += ( matrix[1] * m_TargetOffset.y );

			//SetPosition( targetPosition );

			//glm::vec3 newDirectionVector = m_Target->GetPosition() - targetPosition;

			// Get cross product of oldDirectionVector and newDirectionVector for the rotation axis
			// Get the angle of rotation between the two vectors
			//Call Rotate( float angle, const glm::vec3 &axis )
			
			// Old reference
			//SetDirection( glm::normalize( newDirectionVector ) );
		}
	}
}