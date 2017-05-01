// CAMERAMANAGER.CPP

#include <windows.h>
#include "Vanaheimr.h"
#include "FollowCamera.h"
#include "OrbitCamera.h"
#include "FreeCamera.h"
#include "Helheimr.h"

#pragma comment(lib, "Helheimr.lib")

namespace Vanaheimr
{
	CameraManager::CameraManager() : m_CurrentCameraIndex( 0 )
	{
	}

	CameraManager::~CameraManager()
	{
		for( std::vector<std::pair<Camera*, Mode>>::iterator it = m_Cameras.begin(); it != m_Cameras.end(); ++it )
		{
			delete ( *it ).first;
		}
		m_Cameras.clear();
	}

	const Camera* CameraManager::GetCamera( CameraId cameraIndex )
	{
		return ( cameraIndex > 0 && cameraIndex <= m_Cameras.size() ) ? m_Cameras[ cameraIndex - 1 ].first : nullptr;
	}

	void CameraManager::Update( float timeElapsed, Helheimr::Input* input )
	{
		// Capture or release the mouse based on current state of right mouse button and window focus?

		// TODO : Calculate captured mouse X & Y here and pass to the camera

		// Will I update all cameras or just the current camera?

		if( m_CurrentCameraIndex > 0 && m_CurrentCameraIndex <= m_Cameras.size() )
		{
			Mode mode = m_Cameras[ m_CurrentCameraIndex - 1 ].second;

			if( mode == FOLLOW )
			{
				FollowCamera* camera = static_cast<FollowCamera*>( m_Cameras[ m_CurrentCameraIndex - 1 ].first );

				camera->Update( timeElapsed, input );
			}
			else if( mode == ORBIT )
			{
				OrbitCamera* camera = static_cast<OrbitCamera*>( m_Cameras[ m_CurrentCameraIndex - 1 ].first );

				camera->Update( timeElapsed, input );
			}
			else if( mode == FREE )
			{
				FreeCamera* camera = static_cast<FreeCamera*>( m_Cameras[ m_CurrentCameraIndex - 1 ].first );

				camera->Update( timeElapsed, input );
			}
		}
	}

	CameraManager::CameraId CameraManager::AddCamera( Mode mode, const std::wstring& name )
	{
		Camera* newCamera( nullptr );

		switch( mode )
		{
		case FIXED:
			newCamera = new FixedCamera( name );
			break;
		case FOLLOW:
			newCamera = new FollowCamera( name, nullptr );
			break;
		case ORBIT:
			newCamera = new OrbitCamera( name, nullptr );
			break;
		case FREE:
			newCamera = new FreeCamera( name );
			break;
		default:
			// Do nothing
			return 0;
		}

		m_Cameras.push_back( std::make_pair( newCamera, mode ) );

		unsigned int CameraId = m_Cameras.size();

		if( m_CurrentCameraIndex == 0 )
		{
			m_CurrentCameraIndex = CameraId;
		}

		return CameraId;
	}

	bool CameraManager::SetCameraTarget( CameraId cameraId, Object3D* target )
	{
		if( cameraId > 0 && cameraId <= m_Cameras.size() )
		{
			Mode mode = m_Cameras[ cameraId - 1 ].second;

			if( mode == FOLLOW )
			{
				FollowCamera* camera = static_cast<FollowCamera*>( m_Cameras[ cameraId - 1 ].first );

				camera->SetTarget( target );

				return true;
			}
			else if( mode == ORBIT )
			{
				OrbitCamera* camera = static_cast<OrbitCamera*>( m_Cameras[ cameraId - 1 ].first );

				camera->SetTarget( target );

				return true;
			}
		}

		return false;
	}

	bool CameraManager::SetCameraPosition( CameraId cameraId, const glm::vec4& position )
	{
		if( cameraId > 0 && cameraId <= m_Cameras.size() )
		{
			Camera* camera = m_Cameras[ cameraId - 1 ].first;

			camera->SetPosition( position );

			return true;
		}

		return false;
	}

	bool CameraManager::SetCameraOffset( CameraId cameraId, const glm::vec4& position )
	{
		if( cameraId > 0 && cameraId <= m_Cameras.size() )
		{
			Mode mode = m_Cameras[ cameraId - 1 ].second;

			if( mode == FOLLOW )
			{
				FollowCamera* camera = static_cast<FollowCamera*>( m_Cameras[ cameraId - 1 ].first );

				camera->SetOffset( glm::vec2( position ) );

				return true;
			}
			else if( mode == ORBIT )
			{
				OrbitCamera* camera = static_cast<OrbitCamera*>( m_Cameras[ cameraId - 1 ].first );

				camera->SetOffset( glm::vec2( position ) );

				return true;
			}
		}

		return false;
	}

	bool CameraManager::SetCurrentCamera( CameraId cameraId )
	{
		if( cameraId > 0 && cameraId <= m_Cameras.size() )
		{
			m_CurrentCameraIndex = cameraId;

			return true;
		}

		return false;
	}

	bool CameraManager::SetCurrentCameraToNext()
	{
		if( m_CurrentCameraIndex > 0 )
		{
			if( ++m_CurrentCameraIndex > m_Cameras.size() )
			{
				m_CurrentCameraIndex = 1;
			}

			return true;
		}

		return false;
	}
}