// OBJECT.CPP

#include <windows.h>
#include "Vanaheimr.h"
#include<glm\glm.hpp>

namespace Vanaheimr
{
	/*
	Object3D::Object3D() : m_Position( 0.0f )
	{
		UpdateOrientationCache();
	}

	void Object3D::SetPosition( const glm::vec3& position )
	{
		m_Position = position;
		UpdateOrientationCache();
	}

	void Object3D::SetRotation( const glm::vec3& rotation )
	{
		// Pitch
		glm::quat rotateX = glm::angleAxis( rotation.x, glm::vec3( 1.0f, 0.0f, 0.0f ) );

		// Yaw
		glm::quat rotateY = glm::angleAxis( rotation.y, glm::vec3( 0.0f, 1.0f, 0.0f ) );

		// Roll
		glm::quat rotateZ = glm::angleAxis( rotation.z, glm::vec3( 0.0f, 0.0f, 1.0f ) );

		// Eigen::Quaternion<double> q = rollAngle * yawAngle * pitchAngle;
		m_Orientation = glm::normalize( glm::normalize( rotateZ * rotateY ) * rotateX );

		UpdateOrientationCache();
	}

	void Object3D::SetDirection( const glm::vec3& direction )
	{
		glm::mat4 rotMatrix = glm::lookAt( m_Position, m_Position + direction, glm::vec3( 0, 1, 0 ) );
		m_Orientation = glm::quat_cast( rotMatrix );
		
			//glm::vec3 newDirection = glm::normalize( direction );

			//float dot = glm::dot( glm::vec3( 0.0f, 0.0f, 1.0f ), newDirection );

			//if( fabs( dot - ( -1.0f ) ) < 0.000001f )
			//{
			//	m_Orientation = glm::angleAxis( 180.0f, glm::vec3( 0.0f, 1.0f, 0.0f ) );
			//}
			//else if( fabs( dot - ( 1.0f ) ) < 0.000001f )
			//{
			//	m_Orientation = glm::quat();
			//}
			//else
			//{
			//	//float angle = -glm::degrees( acosf( dot ) );
			//	float angle = glm::degrees( acosf( dot ) );

			//	//glm::vec3 cross = glm::normalize( glm::cross( newDirection, glm::vec3( 0.0f, 0.0f, 1.0f ) ) );
			//	glm::vec3 cross = glm::normalize( glm::cross( glm::vec3( 0.0f, 0.0f, 1.0f ), newDirection ) );

			//	m_Orientation = glm::angleAxis( angle, cross );
			//}
		

		UpdateOrientationCache();
	}

	void Object3D::UpdateOrientationCache()
	{
		m_CachedOrientation = glm::translate( glm::toMat4( m_Orientation ), m_Position );

		m_OrientationRight		= m_CachedOrientation[ 0 ];
		m_OrientationUp			= m_CachedOrientation[ 1 ];
		m_OrientationForward	= m_CachedOrientation[ 2 ];
	}
	*/
}
