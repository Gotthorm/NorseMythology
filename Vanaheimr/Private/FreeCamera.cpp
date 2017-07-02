// FREECAMERA.CPP

#include <windows.h>
#include "FreeCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/compatibility.hpp>
#include "Helheimr.h"

namespace Vanaheimr
{
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
}