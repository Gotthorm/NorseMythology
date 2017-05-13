// FIXEDCAMERA.CPP

#include <windows.h>
#include "FixedCamera.h"
#include "Helheimr.h"

namespace Vanaheimr
{
	FixedCamera::FixedCamera( const std::wstring& name )
		: m_Name( name )
		, m_Position( 0.f )
		, m_Up( 0.f, 1.f, 0.f )
		, m_WorldUp( 0.f, 1.f, 0.f )
		, m_Front( 0.f, 0.f, -1.f )
	{
	}

	//
	void FixedCamera::SetDirection( const glm::vec3& direction )
	{ 
		m_Front = glm::normalize( direction ); 

		// Update the Up vector
		m_Up = glm::normalize( glm::cross( glm::normalize( glm::cross( m_Front, m_WorldUp ) ), m_Front ) );
	}
}

