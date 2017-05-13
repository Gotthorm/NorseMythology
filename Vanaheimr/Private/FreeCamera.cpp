// FREECAMERA.CPP

#include <windows.h>
#include "FreeCamera.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/compatibility.hpp>
#include "Helheimr.h"

namespace Vanaheimr
{
	FreeCamera::FreeCamera( const std::wstring& name ) 
		: FixedCamera( name )
		, m_Pitch( 0.f )
		, m_Yaw( -90.f )
		, m_Right( 0.f )
	{
		UpdateData();
	}

	void FreeCamera::Update( float timeElapsed, const Helheimr::Input* input )
	{
		const float mouseSpeed = 6.0f * timeElapsed;

		int deltaX;
		int deltaY;
		if( input->GetMouse( deltaX, deltaY ) )
		{
			m_Yaw += ( deltaX * mouseSpeed );
			m_Pitch -= ( deltaY * mouseSpeed );

			// Make sure that when pitch is out of bounds, screen doesn't get flipped
			if( m_Pitch > 89.0f )
			{
				m_Pitch = 89.0f;
			}
			if( m_Pitch < -89.0f )
			{
				m_Pitch = -89.0f;
			}

			// Update Front, Right and Up Vectors using the updated Eular angles
			UpdateData();
		}

		float movementIncrement = 10.0f * timeElapsed;
		
		if( input->GetKeyDown( Helheimr::Input::KEY_SHIFT ) )
		{
			movementIncrement *= 10.0f;
		}
		
		if( input->GetKeyDown( Helheimr::Input::KEY_W ) )
		{
			m_Position += m_Front * movementIncrement;
		}
		else if( input->GetKeyDown( Helheimr::Input::KEY_S ) )
		{
			m_Position -= m_Front * movementIncrement;
		}
		
		if( input->GetKeyDown( Helheimr::Input::KEY_A ) )
		{
			m_Position -= m_Right * movementIncrement;
		}
		else if( input->GetKeyDown( Helheimr::Input::KEY_D ) )
		{
			m_Position += m_Right * movementIncrement;
		}
	}

	void FreeCamera::SetDirection( const glm::vec3& direction )
	{
		// Just leave this disabled for this class?
		// Otherwise I would need to update the yaw and pitch to match
	}

	void FreeCamera::UpdateData()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos( glm::radians( m_Yaw ) ) * cos( glm::radians( m_Pitch ) );
		front.y = sin( glm::radians( m_Pitch ) );
		front.z = sin( glm::radians( m_Yaw ) ) * cos( glm::radians( m_Pitch ) );
		m_Front = glm::normalize( front );
		// Also re-calculate the Right and Up vector
		m_Right = glm::normalize( glm::cross( m_Front, m_WorldUp ) );  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		m_Up = glm::normalize( glm::cross( m_Right, m_Front ) );
	}
}