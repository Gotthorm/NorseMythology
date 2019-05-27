// FREECAMERA.CPP

#include <windows.h>
#include "FreeCamera.h"
#include<glm\glm.hpp>
#include "Helheimr.h"

namespace Vanaheimr
{
	glm::vec3 eye( 0.0f, 20.0f, 30.0f );
	glm::vec3 center( 0.0f, 0.0f, 0.0f );
	glm::vec3 up( 0.0f, 1.0f, 0.0f );

	glm::vec3 directionVector( 0.0f, 0.0f, 1.0f );
	glm::quat orientation;

	float m_Yaw = 0.0f;
	float m_Pitch = 0.0f;

	glm::mat4 viewMatrix;

	FreeCamera::FreeCamera( const std::wstring& name ) : FixedCamera( name )
	{
		SetPosition( glm::vec3( 100.0f, -150.0f, -500.0f ) );
	}

	void FreeCamera::Update( float timeElapsed, std::shared_ptr<Helheimr::Input> const & input )
	{
		viewMatrix = glm::mat4();

		//if ( pInput->GetKeyDown( Helheimr::Input::KEY_MOUSE_RIGHT ) )
		{
			int deltaX;
			int deltaY;
			if ( input->GetMouse( deltaX, deltaY ) )
			{
				m_Yaw += deltaX * timeElapsed;
				m_Pitch += deltaY * timeElapsed;

				float yawDegrees = glm::degrees( m_Yaw );
				float pitchDegrees = glm::degrees( m_Pitch );

				if ( yawDegrees < 0.0f )
					m_Yaw = glm::radians( yawDegrees + 360.0f );
				else if ( yawDegrees > 360.0f )
					m_Yaw = glm::radians( yawDegrees - 360.0f );

				if ( pitchDegrees < 0.0f )
					m_Pitch = glm::radians( pitchDegrees + 360.0f );
				else if ( pitchDegrees > 360.0f )
					m_Pitch = glm::radians( pitchDegrees - 360.0f );

				viewMatrix = glm::rotate( viewMatrix, m_Yaw, glm::vec3( 0.0f, 1.0f, 0.0f ) );

				viewMatrix = glm::rotate( viewMatrix, m_Pitch, glm::vec3( viewMatrix[ 0 ][ 0 ], viewMatrix[ 1 ][ 0 ], viewMatrix[ 2 ][ 0 ] ) );
			}
		}

		glm::vec3 positionVector = GetPosition();

		viewMatrix = glm::translate( viewMatrix, positionVector );
			
		glm::vec3 forwardVector = glm::vec3( viewMatrix[ 0 ][ 2 ], viewMatrix[ 1 ][ 2 ], viewMatrix[ 2 ][ 2 ] );
		glm::vec3 upVector = glm::vec3( viewMatrix[ 0 ][ 1 ], viewMatrix[ 1 ][ 1 ], viewMatrix[ 2 ][ 1 ] );
		glm::vec3 leftVector = glm::vec3( viewMatrix[ 0 ][ 0 ], viewMatrix[ 1 ][ 0 ], viewMatrix[ 2 ][ 0 ] );

		if ( input->GetKeyDown( Helheimr::Input::KEY_W ) )
		{
			positionVector += forwardVector * 100.0f * timeElapsed;
		}
		else if ( input->GetKeyDown( Helheimr::Input::KEY_S ) )
		{
			positionVector -= forwardVector * 100.0f * timeElapsed;
		}
		if ( input->GetKeyDown( Helheimr::Input::KEY_A ) )
		{
			positionVector += leftVector * 100.0f * timeElapsed;
		}
		else if ( input->GetKeyDown( Helheimr::Input::KEY_D ) )
		{
			positionVector -= leftVector * 100.0f * timeElapsed;
		}

		SetPosition( positionVector );

		{
			//m_mat4ViewMatrix = glm::mat4();

			//m_flYaw += 0.02f * g_pInput->GetMouseDeltaX() * g_pEngine->GetDeltaTime();
			//m_flPitch += 0.02f * g_pInput->GetMouseDeltaY() * g_pEngine->GetDeltaTime();

			//float flYawDegrees = glm::degrees( m_flYaw );
			//float flPitchDegrees = glm::degrees( m_flPitch );

			//if ( flYawDegrees < 0.0f )
			//	m_flYaw = glm::radians( flYawDegrees + 360.0f );
			//else if ( flYawDegrees > 360.0f )
			//	m_flYaw = glm::radians( flYawDegrees - 360.0f );

			//if ( flPitchDegrees < 0.0f )
			//	m_flPitch = glm::radians( flPitchDegrees + 360.0f );
			//else if ( flPitchDegrees > 360.0f )
			//	m_flPitch = glm::radians( flPitchDegrees - 360.0f );

			//m_mat4ViewMatrix = glm::rotate( m_mat4ViewMatrix, m_flYaw, Vector3f( 0.0f, 1.0f, 0.0f ) );

			//m_mat4ViewMatrix = glm::rotate( m_mat4ViewMatrix, m_flPitch, Vector3f( m_mat4ViewMatrix[ 0 ][ 0 ], m_mat4ViewMatrix[ 1 ][ 0 ], m_mat4ViewMatrix[ 2 ][ 0 ] ) );

			//m_mat4ViewMatrix = glm::translate( m_mat4ViewMatrix, GetPosition() );

			//m_vForward = Vector3f( m_mat4ViewMatrix[ 0 ][ 2 ], m_mat4ViewMatrix[ 1 ][ 2 ], m_mat4ViewMatrix[ 2 ][ 2 ] );
			//m_vUp = Vector3f( m_mat4ViewMatrix[ 0 ][ 1 ], m_mat4ViewMatrix[ 1 ][ 1 ], m_mat4ViewMatrix[ 2 ][ 1 ] );
			//m_vLeft = Vector3f( m_mat4ViewMatrix[ 0 ][ 0 ], m_mat4ViewMatrix[ 1 ][ 0 ], m_mat4ViewMatrix[ 2 ][ 0 ] );

			//m_vBackward = -m_vForward;
			//m_vRight = -m_vLeft;

			//if ( g_pInput->IsButtonPressed( "Forward" ) )
			//{
			//	SetPosition( GetPosition() + m_vForward * 0.01f * g_pEngine->GetDeltaTime() );
			//}
			//else if ( g_pInput->IsButtonPressed( "Backward" ) )
			//{
			//	SetPosition( GetPosition() + m_vBackward * 0.01f * g_pEngine->GetDeltaTime() );
			//}
			//if ( g_pInput->IsButtonPressed( "Left" ) )
			//{
			//	SetPosition( GetPosition() + m_vLeft * 0.01f * g_pEngine->GetDeltaTime() );
			//}
			//else if ( g_pInput->IsButtonPressed( "Right" ) )
			//{
			//	SetPosition( GetPosition() + m_vRight * 0.01f * g_pEngine->GetDeltaTime() );
			//}
		}
#if 0
		if ( nullptr != pInput )
		{
		//	//if( input->GetKeyDown( Helheimr::Input::KEY_ALT ) )
		//	{
		//		float arrowSpeed = 1.5f * timeElapsed;

		//		if ( input->GetKeyDown( Helheimr::Input::KEY_ARROW_DOWN ) )
		//		{
		//			Pitch( -arrowSpeed );
		//		}
		//		else if ( input->GetKeyDown( Helheimr::Input::KEY_ARROW_UP ) )
		//		{
		//			Pitch( arrowSpeed );
		//		}

		//		if ( input->GetKeyDown( Helheimr::Input::KEY_ARROW_LEFT ) )
		//		{
		//			Yaw( -arrowSpeed );
		//		}
		//		else if ( input->GetKeyDown( Helheimr::Input::KEY_ARROW_RIGHT ) )
		//		{
		//			Yaw( arrowSpeed );
		//		}
		//	}

			if( pInput->GetKeyDown( Helheimr::Input::KEY_MOUSE_RIGHT ) )
			{
				float mouseSpeed = 2.0f * timeElapsed;

				int deltaX;
				int deltaY;
				if ( pInput->GetMouse( deltaX, deltaY ) )
				{
					Pitch( deltaY * mouseSpeed );

					if ( input->GetKeyDown( Helheimr::Input::KEY_ALT ) )
					{
						Roll( deltaX * mouseSpeed );
					}
					else
					{
						Yaw( deltaX * mouseSpeed );
					}
				}
			}

			// if( true )
			{
				float MovementIncrement = 10.0f * timeElapsed;

				if ( pInput->GetKeyDown( Helheimr::Input::KEY_SHIFT ) )
				{
					MovementIncrement *= 10.0f;
				}

				if ( pInput->GetKeyDown( Helheimr::Input::KEY_W ) )
				{
					Translate( 0, 0, MovementIncrement );
					positionVector += ( ( glm::vec3( 0.0f, 0.0f, 1.0f ) * MovementIncrement ) * orientation );
				}
				else if ( pInput->GetKeyDown( Helheimr::Input::KEY_S ) )
				{
					Translate( 0, 0, -MovementIncrement );
					positionVector -= ( ( glm::vec3( 0.0f, 0.0f, 1.0f ) * MovementIncrement ) * orientation );
				}
				if ( pInput->GetKeyDown( Helheimr::Input::KEY_A ) )
				{
					Translate( MovementIncrement, 0, 0 );
					positionVector += ( ( glm::vec3( 1.0f, 0.0f, 0.0f ) * MovementIncrement ) * orientation );
				}
				else if ( pInput->GetKeyDown( Helheimr::Input::KEY_D ) )
				{
					Translate( -MovementIncrement, 0, 0 );
					positionVector -= ( ( glm::vec3( 1.0f, 0.0f, 0.0f ) * MovementIncrement ) * orientation );
				}
			}
		}
#endif
	}

	glm::mat4 FreeCamera::GetMatrix() const
	{
		// (eye, center, up)
		//return glm::lookAt( positionVector, positionVector + directionVector, up );
		return viewMatrix;
	}

#if 0
	FreeCamera::FreeCamera( const std::wstring& name ) : FixedCamera( name ), m_CurrentPitch( 0 ), m_CurrentYaw( 0 )
	{
	}

	void FreeCamera::Update( float timeElapsed, Helheimr::Input* input )
	{
		if( input != NULL )
		{
			//if( input->GetKeyDown( Helheimr::Input::KEY_ALT ) )
			{
				float arrowSpeed = 1.5f * timeElapsed;

				if( input->GetKeyDown( Helheimr::Input::KEY_ARROW_DOWN ) )
				{
					Pitch( -arrowSpeed );
				}
				else if( input->GetKeyDown( Helheimr::Input::KEY_ARROW_UP ) )
				{
					Pitch( arrowSpeed );
				}

				if( input->GetKeyDown( Helheimr::Input::KEY_ARROW_LEFT ) )
				{
					Yaw( -arrowSpeed );
				}
				else if( input->GetKeyDown( Helheimr::Input::KEY_ARROW_RIGHT ) )
				{
					Yaw( arrowSpeed );
				}
			}

			//if( input->GetKeyDown( Helheimr::Input::KEY_MOUSE_RIGHT ) )
			{
				float mouseSpeed = 2.0f * timeElapsed;

				int deltaX;
				int deltaY;
				if( input->GetMouse( deltaX, deltaY ) )
				{
					Pitch( deltaY * mouseSpeed );

					if( input->GetKeyDown( Helheimr::Input::KEY_ALT ) )
					{
						Roll( deltaX * mouseSpeed );
					}
					else
					{
						Yaw( deltaX * mouseSpeed );
					}
				}
			}

			// if( true )
			{
				float MovementIncrement = 10.0f * timeElapsed;

				if( input->GetKeyDown( Helheimr::Input::KEY_SHIFT ) )
				{
					MovementIncrement *= 10.0f;
				}

				if( input->GetKeyDown( Helheimr::Input::KEY_W ) )
				{
					Translate( 0, 0, MovementIncrement );
				}
				else if( input->GetKeyDown( Helheimr::Input::KEY_S ) )
				{
					Translate( 0, 0, -MovementIncrement );
				}
				if( input->GetKeyDown( Helheimr::Input::KEY_A ) )
				{
					Translate( MovementIncrement, 0, 0 );
				}
				else if( input->GetKeyDown( Helheimr::Input::KEY_D ) )
				{
					Translate( -MovementIncrement, 0, 0 );
				}
			}
		}
	}

	glm::mat4 FreeCamera::GetMatrix() const
	{
		return Object3D::GetMatrix();
	}
#endif
}