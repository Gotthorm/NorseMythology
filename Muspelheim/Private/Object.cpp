// OBJECT.CPP

#include <windows.h>
#include "Muspelheim.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Muspelheim
{
	GameObject::GameObject()
		: m_CachedOrientation( 1.0f )
		, m_Position( 0.0f )
		, m_Dirty( false )
	{
	}

	const glm::vec3& GameObject::GetRight()	{ UpdateOrientationCache(); return m_CachedOrientation[ 0 ]; }
	const glm::vec3& GameObject::GetUp()		{ UpdateOrientationCache(); return m_CachedOrientation[ 1 ]; }
	const glm::vec3& GameObject::GetForward()	{ UpdateOrientationCache(); return m_CachedOrientation[ 2 ]; }

	void GameObject::SetPosition( const glm::vec3& position )
	{
		m_Position = position;
	}

	void GameObject::SetRotation( const glm::vec3& rotation )
	{
		// Pitch
		glm::quat rotateX = glm::angleAxis( rotation.x, glm::vec3( 1.0f, 0.0f, 0.0f ) );

		// Yaw
		glm::quat rotateY = glm::angleAxis( rotation.y, glm::vec3( 0.0f, 1.0f, 0.0f ) );

		// Roll
		glm::quat rotateZ = glm::angleAxis( rotation.z, glm::vec3( 0.0f, 0.0f, 1.0f ) );

		// Eigen::Quaternion<double> q = rollAngle * yawAngle * pitchAngle;
		m_Orientation = glm::normalize( glm::normalize( rotateZ * rotateY ) * rotateX );

		m_Dirty = true;
	}

	void GameObject::SetDirection( const glm::vec3& direction )
	{
		glm::mat4 rotMatrix = glm::lookAt( m_Position, m_Position + direction, glm::vec3( 0, 1, 0 ) );
		m_Orientation = glm::quat_cast( rotMatrix );
		/*
			glm::vec3 newDirection = glm::normalize( direction );

			float dot = glm::dot( glm::vec3( 0.0f, 0.0f, 1.0f ), newDirection );

			if( fabs( dot - ( -1.0f ) ) < 0.000001f )
			{
				m_Orientation = glm::angleAxis( 180.0f, glm::vec3( 0.0f, 1.0f, 0.0f ) );
			}
			else if( fabs( dot - ( 1.0f ) ) < 0.000001f )
			{
				m_Orientation = glm::quat();
			}
			else
			{
				//float angle = -glm::degrees( acosf( dot ) );
				float angle = glm::degrees( acosf( dot ) );

				//glm::vec3 cross = glm::normalize( glm::cross( newDirection, glm::vec3( 0.0f, 0.0f, 1.0f ) ) );
				glm::vec3 cross = glm::normalize( glm::cross( glm::vec3( 0.0f, 0.0f, 1.0f ), newDirection ) );

				m_Orientation = glm::angleAxis( angle, cross );
			}
		*/

		m_Dirty = true;
	}

	const glm::mat4& GameObject::GetOrientation()
	{
		UpdateOrientationCache();

		return m_CachedOrientation;
		//m_CachedOrientation[ 3 ] = m_Position;
	//	//glm::mat4 identityMat = glm::mat4( 1.0f );
	//	glm::mat4 rotMatrix = glm::toMat4( m_Orientation );
	//	glm::mat4 transMatrix = glm::translate( glm::mat4(), m_Position );
	//	//return rotMatrix * glm::inverse( transMatrix );
	//	return glm::inverse( transMatrix ) * rotMatrix;

	//	/*
	//		glm::mat4 translationMatrix = glm::translate( glm::mat4(), m_Position );

	//		glm::mat4 orientation = glm::toMat4( m_Orientation );

	//		//orientation[3] = glm::vec4( m_Position, 1 );

	//		//return orientation;
	//		//return orientation * translationMatrix;
	//		return translationMatrix * orientation;
	//	*/
	}

	void GameObject::UpdateOrientationCache()
	{
		if( m_Dirty ) 
		{
			m_CachedOrientation = glm::toMat4( m_Orientation );
			m_Dirty = false;
		}
	}
}
